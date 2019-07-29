/*
 * Dafif.cpp
 *
 *  Created on: 2012. 12. 17.
 *      Author: jiwon
 */

#include "Dafif.h"

using namespace::std;

Dafif::Dafif() {
	// TODO Auto-generated constructor stub
	cout<<"Dafif is generated.."<<endl;

}

Dafif::~Dafif() {
	// TODO Auto-generated destructor stub
}


bool Dafif::readFile(const QString& fileName, TableType _tableType)
{
	//QList<String> tableList;


	QFile file(fileName);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		errorMessage = QString(QObject::tr("Cannot read file %1:\n%2")
			                        .arg(file.fileName())
					                .arg(file.errorString()));
        return false;
	}
	tableType = _tableType;
	lineNo = 0;
	QTextStream in(&file);
	//파일을 읽어 들인다..

	while(!in.atEnd()){
		QString line = in.readLine();
		qDebug() << "Read Line : " << line;
		lineNo++;

		parsing(line, tableType);
	}
	//QMessageBox::warning(this, "Information", "Parsing is completed");
	qDebug() << "File end read....";
	file.close();

	return true;
}

/**
 * Arinc424 file format parsing
 */
void Dafif::parsing(const QString& line, const TableType tableType)
{
	qDebug() << "parsing .........";

	//LineType g = gubun(line, tableList);

	switch(tableType){
		case DA_APCC_ICAO:
		{
			TB_DA_CNTRYCD * cntryCd = new TB_DA_CNTRYCD;
			memset(cntryCd, 0, sizeof(TB_DA_CNTRYCD));
			cntryCd->LineNo = lineNo;
			//header 검출
			if(cntryCd->LineNo == 1){
				QStringList header = parsingHeader(line);
			}else{
				QStringList listStrList = line.split('\t');
				qDebug()<<"ListStrList : "<<listStrList;
				copy(cntryCd->COUNTRY, listStrList.at(getHeader("COUNTRY")));
				copy(cntryCd->COUNTRY_NAME, listStrList.at(getHeader("COUNTRY_NAME")));
				copy(cntryCd->ICAO_RGN, listStrList.at(getHeader("ICAO_RGN")));
				if(listStrList.at(getHeader("USAGE")).trimmed() == QString("ALL RECORDS").trimmed())
				{
					copy(&cntryCd->usageType, ALL_RECORDS);
				}else{
					copy(&cntryCd->usageType, NO_ALL_RECORDS);
				}
				tb_da_cntrycd.append(cntryCd);
			}
			break;
		}

	}

}

void Dafif::copy(char * dest, QString source)
{
	strcpy(dest,source.toStdString().c_str()); //tmpBuffer의 내용을 destinatio으로 옮긴다
}

void Dafif::copy(UsageType * dest, UsageType source)
{
	*dest = source;
}
void Dafif::PrtDafif()
{
	int cnt = tb_da_cntrycd.count();

	for(int i = 0 ; i < cnt ; i++){
		TB_DA_CNTRYCD * cd = tb_da_cntrycd.at(i);
		QString country = QString::fromStdString(cd->COUNTRY);
		QString country_name = QString::fromStdString(cd->COUNTRY_NAME);
		QString rgn = QString::fromStdString(cd->ICAO_RGN);

		qDebug()<<"Count : "+country+"  CountryName : "+country_name+"  rgn :"+rgn;
	}
}
QString Dafif::refineSql(const QString& sql)
{
	qDebug()<<"[Dafif - refineSql] start.. sql : "<<sql;

	QString newsql;
	int semipos = sql.indexOf(";");
	if(semipos >= 0 ){
		newsql = sql.mid(0,semipos);
	}

	qDebug()<<"[Dafif - refineSql] finish.. sql : "<<newsql;
	return newsql.trimmed();
}


/**
 * It insert contents in Database that the memory bring [Vector[Object]].
 */
bool Dafif::insertTables(TableType tableType)
{
	int insertCount = 0;
	int updateCount = 0;
	QSqlError sqlerror;
	int step = 0;
	jobCount = 0;
	QSqlQuery* query = new QSqlQuery(QSqlDatabase::database(UserInfo::getInstance().getDbname()));

	QProgressDialog progress;
	progress.setLabelText("Processing....");
	progress.setCancelButton(0);
	progress.setRange(0, lineNo);
	progress.setModal(true);

	qDebug() << "insertTables start ";
	// TODO: This implements the INSERT AND UPDATE in TB_FIR table.
	if(tb_da_cntrycd.count() > 0) {
		insertCount = 0; updateCount=0;
		qDebug()<<"TB_DA_CNTRYCD insert start";
		for(int i=0; i<tb_da_cntrycd.count();i++){
			if (jobCount > lineNo) {
				if ( step > lineNo) {
					step = 0;
				}else {
					step++;
				}
			}else {
				step = jobCount;
			}
			progress.setValue(step);
			qApp->processEvents();

			TB_DA_CNTRYCD * obj = tb_da_cntrycd[i];
			if(obj->usageType == NO_ALL_RECORDS){
				continue;
			}
			QString sql =  sqlgen.getCount(obj);
			sql = refineSql(sql);
			query->exec(sql);
			//테이블에 레코드가 존재하는지 판단한다..
			if(query->next()){
				int cnt = query->value(0).toInt();
				if(cnt == 1){
					sql = sqlgen.updateSql(obj); updateCount++;
					qDebug()<<"Update Process will be performed..";
				}else if(cnt == 0){
					sql = sqlgen.insertSql(obj); insertCount++;
					qDebug()<<"Insert Process will be performed..";
				}else{
				//	log.print(QString("Maybe TB_FIR pk was changed.lineno:%1,count:%2").arg(obj->LineNo).arg(cnt));
				//	log.print(QString("rollbacked"));
					QSqlDatabase::database(UserInfo::getInstance().getDbname()).rollback();
					errorMessage = QString("Maybe TB_DA_CNTRYCD pk was changed. LineNo:%1, Count:%2").arg(obj->LineNo).arg(cnt);
					return false;
				}
			}
			if (query->lastError().type() != QSqlError::NoError) {
			//	log.print(QString("fail sql:[%1]").arg(sql));
			//	log.print(QString("rollbacked"));
				QSqlDatabase::database(UserInfo::getInstance().getDbname()).rollback();
				errorMessage = QString("Fail Sql : [%1], Reason : [%2]").arg(sql).arg(query->lastError().text());
				return false;
			}
			//Insert 또는 Update Mode
			sql = refineSql(sql);
			query->exec(sql);
			sqlerror = query->lastError();
			if( sqlerror.type() != QSqlError::NoError){
				//log.print(QString("TB_FIRisert fail (LineNo:%1) : %2").arg(obj->LineNo).arg(sqlerror.text()));
				//log.print(QString("sql:[%1]").arg(sql));
				QSqlDatabase::database(UserInfo::getInstance().getDbname()).rollback();
				//log.print(QString("rollbacked"));
				errorMessage = QString("The error is occurred, database is rollbacked. [TB_FIR (LineNo:%1) : %2 ]").arg(obj->LineNo).arg(sqlerror.text());
				return false;
			}
			jobCount++;
		}
		//log.print(QString("TB_FIR succesfully end. read: %1 insert: %2 update:%3").arg(tb_fir_list.count()).arg(insertCount).arg(updateCount));
		qDebug() << "TB_DA_CNTRYCD Count : " << jobCount;
	}

	return true;
}

bool Dafif::deleteAll(const QString tableName)
{
	QSqlQuery* query = new QSqlQuery(QSqlDatabase::database(UserInfo::getInstance().getDbname()));
	QString sql = "";
	sql  = " DELETE FROM "+tableName;
	qDebug() << "printDeleteAll : " << sql;
	query->prepare(sql);
	query->exec();

	if (query->lastError().type() != QSqlError::NoError) {
		errorMessage = query->lastError().text();
		//QSqlDatabase::database(UserInfo::getInstance().getDbname()).rollback();
		delete query;
		return false;
	}else {
		delete query;
		return true;
	}
	//delete query;
}

QStringList Dafif::parsingHeader(const QString header)
{
	qDebug()<<"Parsing Header.... data : "<<header;
	QStringList headername = header.split('\t');

	headerList.clear();
	for(int i = 0 ; i < headername.count() ; i++)
	{
		//qDebug()<<"headername : "<<headername.at(i)<<", value :"<<i;
		headerList.insert(headername.at(i), i);
		qDebug()<<"Header :"<<headerList;
	}
	qDebug()<<"Header : "<<headerList;
	return headername;
}

int Dafif::getHeader(const QString headername)
{
	int num = headerList.value(headername);

	return num;
}
