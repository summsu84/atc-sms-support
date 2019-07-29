/*
 * DbManager.cpp
 *
 *  Created on: 2013. 1. 29.
 *      Author: jiwon
 */

#include "DbManager.h"
#include <QMessageBox>

DbManager::DbManager(QVector<ConvertDataInfo> dataInfo) {
	// TODO Auto-generated constructor stub
	convertDataInfo = dataInfo;

	//test
	for(int i = 0 ; i < dataInfo.count() ; i++){
		ConvertDataInfo data = (ConvertDataInfo)dataInfo.value(i);\
		data.makeMapFile();
	}


	init();
}

DbManager::DbManager()
{
	init();
}

DbManager::~DbManager() {
	// TODO Auto-generated destructor stub
	qDebug()<<"[DbManager] db close..";
}

void DbManager::init()
{
	UserInfo::getInstance().setDbname("smsd");
	UserInfo::getInstance().setId("adapt");
	UserInfo::getInstance().setPass("adapt");
}

bool DbManager::dbOpen()
{
	qDebug()<<"[DbManager] dbOpen is called..";
	QSqlDatabase::removeDatabase("smsd");
	{
		QSqlDatabase db = QSqlDatabase::addDatabase("QOCI", "smsd");
		db.setHostName("sms02");
		db.setDatabaseName("smsd");
		db.setUserName("adapt");
		db.setPassword("adapt");
		//QMessageBox::warning(this, tr("DataBase Open Error"), tr("%1  %2  %3  %4 Driver %5").arg(parser->getDatabase()).arg(parser->getDbUser()).arg(parser->getDbPass()).arg(parser->getHost()).arg(parser->getDriver()));
		bool chk = db.open();
		qDebug()<<"Chk : "<<chk;
		if (chk == false) {
			qDebug()<<"Database Open Error!";
			return false;
		}else{
			qDebug()<<"Database Open Success!.";
			return true;
		}
	}
}

/**
 * SMS 데이터베이스에  KML 파일을 읽은 맵 데이터를 Insert 하는 작업을 수행한다..
 * 1. ConvertDataInfo에 저장된 Layer 이름 --> 테이블 명 (retriveLayer를 통해 결정된다..)
 * 2. ConvertDataInfo에 저장된 ID 이름 --> 각 테이블의 PK
 * 3. Lat, Lon의 갯수 만큼 각 메인 테이블의 서브테이블에 Insert
 *   - 2에서 얻은 PK와 Lat, Lon 의 갯수를 인덱스화 (010, 020..) 그리고 Lat, Lon 데이터를 Insert
 */
bool DbManager::insertProcess(QChar & prefix)
{
	qDebug()<<"insert Process is called..";

	bool chk;
	QString sql;
	//QChar prefix;

	mainCnt = 0;				//main table에 insert 갯수
	subCnt =0;				//sub table에 insert 갯

	QSqlQuery * query = new QSqlQuery(QSqlDatabase::database(UserInfo::getInstance().getDbname()));

	for(int i = 0 ; i < convertDataInfo.count() ; i ++){
		ConvertDataInfo data = (ConvertDataInfo)convertDataInfo.at(i);

		//파싱한 데이터로 부터 레이어 이름을 검출한다.. --> 추후 테이블명으로 사용
		QString tb_name = retriveTableName(data.getLayer());

		//SqlGen 객체를 생성한다..
		if(tb_name == "TB_COAST"){
			sqlgen = new CoastSqlGen(tb_name);
			prefix = 'C';
		}else if(tb_name == "TB_SIDSTARMAP"){
			sqlgen = new SidStarSqlGen(tb_name);
			prefix = 'S';
		}else if(tb_name == "TB_CLSMAP"){
			sqlgen = new AirspaceSqlGen(tb_name);
			prefix = 'A';
		}

		//Main table 작업은 레이어가 SIDSTAR가 아닌 경우에만 실시한다.. SIDSTAR 인 경우에는 Subtable Insert만 수행한다..

		if(tb_name != "TB_SIDSTARMAP"){
			//main table count 구하기
			sql = sqlgen->getCountMainTableSql(data);

			chk = getCount4Insert(sql, query);
			if(!chk){
				errorMessage = data.getId() + " already exist.";
				//return false;
				continue;
			}

			//main table insert
			sql = sqlgen->insertMainTableSql(data);

			qDebug()<<"insert sql : "<<sql;

			query->prepare(sql);

			query->exec();
			if (query->lastError().type() != QSqlError::NoError) {
				qDebug()<<query->lastError().text();
				errorMessage = query->lastError().text();
				errorMessage.prepend(prefix);
				return false;
			}else {
				mainCnt++;
				qDebug()<<"Inserting is success";
			}
		}

		qDebug()<<"bndry count : "<<data.getLatList().count();

		//sub table insert
		int latlonCnt = data.getLatList().count();

		for(int j = 0 ; j < latlonCnt ; j++){

			//sub table count 구하기
			sql = sqlgen->getCountSubTableSql(data, j);

			chk = getCount4Insert(sql, query);
			if(!chk){
				errorMessage = data.getId() + " already exist.";
				//return false;
				continue;
			}

			//sub table insert
			sql = sqlgen->insertSubTableSql(data, j);

			qDebug()<<"insert sub table sql : "<<sql;

			query->prepare(sql);
			query->exec();
			if (query->lastError().type() != QSqlError::NoError) {
				qDebug()<<query->lastError().text();
				errorMessage = query->lastError().text();
				return false;
			}else {
				subCnt++;
				qDebug()<<"Inserting sub talbe is success";
			}
		}

	}
	return true;
}

//type 1 -> NO ICAO_CD IN TB_CNTRYCD;
//type 2 -> EXIST IN TB_AD;
bool DbManager::insertProcess(QString ad, QString icao, QString tb_name, int & type)
{
	QSqlQuery * query = new QSqlQuery(QSqlDatabase::database(UserInfo::getInstance().getDbname()));

	QString sql;

	//공항이 TB_CNTRYCD에 존재 하는지 판단
	sql = "SELECT COUNT(*) FROM TB_CNTRYCD WHERE ICAO_CD = '"+icao+"'";
	query->prepare(sql);
	query->exec();
	if (query->next()) {
		int cnt = query->value(0).toInt();
		if (cnt == 0) {
			qDebug()<<"No ICAO_CD in TB_CNTRYCD!..";
			type = 1;
			return false;
		}
	}

	//TB_AD에 AD_ID및 ICAO_CD가 있는지 없는지 판단.. 있는 경우 Pass
	sql = "SELECT COUNT(*) FROM TB_AD WHERE AD_ID = '"+ad+"' AND ICAO_CD = '"+icao+"'";
	query->prepare(sql);
	query->exec();
	if (query->next()) {
		int cnt = query->value(0).toInt();
		if (cnt > 0) {
			qDebug()<<"Data already exist!..";
			type = 2;
			return false;
		}
	}

	//Insert

	sql = "INSERT INTO TB_AD (AD_ID, ICAO_CD) VALUES('"+ad+"', '"+icao+"')";
	qDebug()<<"insert sql : "<<sql;
	query->prepare(sql);
	query->exec();
	if (query->lastError().type() != QSqlError::NoError) {
		qDebug()<<query->lastError().text();
		return false;
	}

	query->clear();
	return true;
}



/**
 * Layer 이름을 바탕으로 adaptation 에 저장할 테이블을 이름을 가져온다..
 */
QString DbManager::retriveTableName(QString layer)
{
	QString tb_name = "TB_";
	if(layer == "SID" || layer == "STAR"){
		tb_name.append("SIDSTARMAP");
	}else if(layer == "AIRSPACE"){
		tb_name.append("CLSMAP");
	}
	else{
		tb_name.append(layer);
	}

	return tb_name;
}

//Count

/**
 * Insert 작업을 위해서 레코드가 존재하는지 확인한다..
 * 만약 레코드가 존재 하는 경우 false를 리턴..
 * 레코드가 존재 하지 않을 경우 true를 리턴..
 */
bool DbManager::getCount4Insert(QString sql, QSqlQuery * query)
{

	qDebug()<<"[DbManager] getCount4Insert is called.. sql :"<<sql;
	query->prepare(sql);

	query->exec();

	if(query->next())
	{
		int cnt = query->value(0).toInt();
		if(cnt == 0){
			qDebug()<<"There is no record..";
			return true;
		}else{
			qDebug()<<"Record does exist..";
			return false;
		}
	}
	if (query->lastError().type() != QSqlError::NoError) {
		qDebug()<<query->lastError().text();
		errorMessage = query->lastError().text();
		return false;
	}

	return false;
}
