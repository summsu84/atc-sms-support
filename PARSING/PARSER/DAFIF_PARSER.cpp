#include "DAFIF_PARSER.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <QtSql>
#include "../lib/UserInfo.h"
#include <QMessageBox>
#include <QBoxLayout>

DAFIF_PARSER::DAFIF_PARSER(QDialog *parent)
    : QDialog(parent)
{
	qDebug()<<"DAFIF_PARSER is called..";
	stackedLayout = new QStackedLayout;
	init();
	QVBoxLayout * mainLayout = new QVBoxLayout;
	mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	mainLayout->addLayout(stackedLayout);

	this->setLayout(mainLayout);
	setWindowTitle(tr("DAFIF Parser"));
}

void DAFIF_PARSER::initLayout()
{
	qDebug()<<"initLayout is called..";
	QVBoxLayout * gridLayout = setCheckBox(openFile());
	QBoxLayout * box = new QBoxLayout(QBoxLayout::TopToBottom);
	box->addWidget(btnSelectAll);
	box->addWidget(btnDeselectAll);
	box->addWidget(btnStart);
	box->addWidget(btnDb);
	box->addWidget(btnClear);
	QGridLayout * centerLayout = new QGridLayout();
	QGridLayout * tableLayout = initTableParam();

	centerLayout->addLayout(gridLayout,0,0);
	centerLayout->addLayout(box, 0, 1);
	centerLayout->addLayout(tableLayout, 1, 0);

	QWidget * test = new QWidget;
	test->setLayout(centerLayout);

	test->adjustSize();
	if(stackedLayout->count() == 1){
		stackedLayout->removeWidget(stackedLayout->widget(0));
	}

	stackedLayout->addWidget(test);
	qDebug()<<"current Index : "<<stackedLayout->currentIndex()<<", count : "<<stackedLayout->count();
	/*if(stackedLayout->count() == 2){
		stackedLayout->removeWidget(stackedLayout->widget(0));
		stackedLayout->setCurrentIndex(1);
	}*/
	//stackedLayout->setCurrentIndex(stackedLayout->count()-1);

}

void DAFIF_PARSER::initBtn()
{
	qDebug()<<"initBtn is called..";
	btnStart = new QPushButton(tr("Start"));
	btnDb = new QPushButton(tr("Gen"));
	btnSelectAll = new QPushButton(tr("Select All"));
	btnDeselectAll = new QPushButton(tr("DeSelect All"));
	btnClear = new QPushButton(tr("Clear"));
	connect(btnSelectAll, SIGNAL(clicked()), this, SLOT(runSelectAll()));
	connect(btnDb, SIGNAL(clicked()), this, SLOT(runParsing()));
	connect(btnDeselectAll, SIGNAL(clicked()), this, SLOT(runDeSelectAll()));
	connect(btnClear, SIGNAL(clicked()), this, SLOT(runClear()));
}


void DAFIF_PARSER::clear()
{
	qDebug()<<"Clear is called..";

	fileName = "";
	cnt = 0;


	txtTableName = 0;
	txtParam1 = 0;
	txtParam2 = 0;
	btnSelectAll = 0;
	btnDeselectAll = 0;
	btnStart = 0;
	btnDb = 0;
	btnClear = 0;
	headerList.clear();			//맵 자료형의 HeaderList
	headerNameList.clear();				//헤더 이름 목록
	paramList.clear();

	lineNo = 0;
	errorMessage = "";

}

DAFIF_PARSER::~DAFIF_PARSER()
{
	qDebug()<<"Desconstruction..";

/*	for(int i = 0 ; i < cnt ; i++){
		delete [] * txtHeader;
		delete [] * chkHeader;
		delete [] * lblHeader;
	}
	delete [] txtHeader;
	delete [] chkHeader;
	delete [] lblHeader;*/
}

void DAFIF_PARSER::init()
{
	clear();
	initBtn();
	initLayout();
}

//파일을 열고 헤더를 읽어온다..
QStringList DAFIF_PARSER::openFile()
{
	qDebug()<<"Open File is called..";
	QString dir = QDir::currentPath();
	QFileDialog fdialog(this);
	QStringList tmp;
	//fdialog.setFileMode(QFileDialog::ExistingFiles);


	qDebug()<<"h1";
	fileName = fdialog.getOpenFileName(0, tr("Open the dat file"),
			dir, tr("dat file(*.TXT)"));

	QFile file(fileName);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		errorMessage = QString(QObject::tr("Cannot read file %1:\n%2")
									.arg(file.fileName())
									.arg(file.errorString()));
		qDebug()<<"h2";
		return tmp;
	}

	qDebug()<<"h3";
	lineNo = 0;
	QTextStream in(&file);

	//파일을 읽어 들인다..
	QString line = in.readLine();
	qDebug() << "Read Line : " << line;
	lineNo++;


	tmp = parsingHeader(line);
	qDebug()<<"HeaderList : "<<tmp;

	//QMessageBox::warning(this, "Information", "Parsing is completed");
	qDebug() << "File end read....";
	file.close();

	return tmp;
}

void DAFIF_PARSER::runParsing()
{
	qDebug()<<"Run Parsing is called..";

	//TB_CHECK
	if(txtTableName->text().length() == 0){
		QMessageBox::warning(this, tr("Input Data Error"), tr("Input Table Name"));
		return;
	}
	//PARAMETER CHECK
	if(txtParam1->text().length() == 0){
		QMessageBox::warning(this, tr("Input Data Error"), tr("Input Parameter 1"));
		return;
	}

	//PARAMETER PARSING
	paramList = txtParam1->text().split('/');
	qDebug()<<"Param1 : "<<paramList;


	QFile file(fileName);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		errorMessage = QString(QObject::tr("Cannot read file %1:\n%2")
									.arg(file.fileName())
									.arg(file.errorString()));
		return;
	}

	lineNo = 0;
	QTextStream in(&file);
	//파일을 읽어 들인다..

	while(!in.atEnd()){
		QString line = in.readLine();
		qDebug() << "Read Line : " << line;
		lineNo++;

		parsing(line);
	}
	//QMessageBox::warning(this, "Information", "Parsing is completed");
	qDebug() << "File end read....";
	file.close();
}


/**
 * 컬럼 이름을 검출 한다..
 * @param header
 * @return
 */
QStringList DAFIF_PARSER::parsingHeader(const QString header)
{
	qDebug()<<"Parsing Header.... data : "<<header;
	QStringList headername = header.split('\t');

	headerList.clear();
	for(int i = 0 ; i < headername.count() ; i++)
	{
		//qDebug()<<"headername : "<<headername.at(i)<<", value :"<<i;
		headerList.insert(headername.at(i), i);
		//qDebug()<<"Header :"<<headerList;
	}
	qDebug()<<"Header : "<<headerList;
	return headername;
}

/**
 * 컬럼의 이름으로 컬럼의 인덱스를 검출 한다..
 * @param headername
 * @return
 */
int DAFIF_PARSER::getHeader(const QString headername)
{
	int num = headerList.value(headername);

	return num;
}


void DAFIF_PARSER::parsing(const QString& line)
{
	qDebug() << "parsing .........";
	QStringList listStrList = line.split('\t');
	qDebug()<<"ListStrList : "<<listStrList;
	QStringList param;
	//체크 부분에 해당하는 정보 얻기
	for(int i = 0 ; i < cnt; i++){
		if(chkHeader[i]->isChecked() == true){
			QString pr = listStrList.at(this->getHeader(txtHeader[i]->text()));
			param.append(pr);
		}
	}

/*
	idx = listStrList.at(1);

	idx = idx.remove(idx.length()-1, 1);
	for(int i = idx.length() ; i < 3 ; i++){
		idx.prepend('0');
	}
	idx.append('0');
*/

	//Param2 parsing
	QMap<QString, QString> param2Map;
	if(txtParam2->text().length() != 0){
	 param2Map= this->parsingParam2(txtParam2->text().split("/"));
	}

	QString sql;

	sql = "INSERT INTO "+txtTableName->text()+ " ( ";
	for(int i = 0 ;i < paramList.count() ; i++){
		sql.append(paramList.at(i));
		if(i < paramList.count() - 1){
			sql.append(", ");
		}
	}
	if(txtParam2->text().length() != 0){
		QList<QString> param2Keys = param2Map.keys();
		sql.append(", ");
		for(int i = 0 ; i < param2Keys.count() ; i++){
			sql.append(param2Keys.at(i));
			if(i < param2Keys.count() - 1){
				sql.append(", ");
			}
		}
	}

	sql.append(") VALUES(");
	for(int i = 0 ; i < param.count() ; i++){
		sql.append("'"+param.at(i)+"'");
		if(i < param.count() - 1){
			sql.append(", ");
		}
	}
	if(txtParam2->text().length() != 0){
		QList<QString> param2Values = param2Map.values();
		sql.append(", ");
		for(int i = 0 ; i < param2Values.count() ; i++){
			sql.append("'"+param2Values.at(i)+"'");
			if(i < param2Values.count() - 1){
				sql.append(", ");
			}
		}
	}

	sql.append(") ");

	qDebug()<<"sql : "<<sql;

	/*QSqlQuery * query = new QSqlQuery(QSqlDatabase::database(UserInfo::getInstance().getDbname()));

	QString sql;

	sql = "INSERT INTO TB_FIRBNDRY (FIR_ID, FIR_ADDR, BNDRY_IDX, BNDRY_VIA, LAT, LON)";
	sql += " VALUES('XXXX', 'XXXX', '"+idx+"', 'G', '"+lat+"', '"+lon+"')";
	qDebug()<<"sql : "<<sql;
	query->prepare(sql);*/
	/*query->exec();
	if (query->lastError().type() != QSqlError::NoError) {
		QMessageBox::critical(this, "SQL Error", query->lastError().text());
	}else {
		//QMessageBox::information(this, tr("Success message"), tr("Success"));
	}*/
	//delete query;
}


/**
 * 헤더 리스트를 파싱하여 헤더의 순서와 이름을 표시한다.
 */
QVBoxLayout * DAFIF_PARSER::setCheckBox(QStringList listHeader)
{
	qDebug()<<"setCheckBox is called..";
	cnt = listHeader.count();

	int numColumn = 3;

	QLabel * headerName = new QLabel(tr("<h3>Header Name</h3>"));
	//QCheckBox * chkHeader[cnt];
	chkHeader = new QCheckBox*[cnt];
	txtHeader = new QLineEdit*[cnt];
	lblHeader = new QLabel * [cnt];
	//QLabel * lblHeader[cnt];

	QVBoxLayout * boxLayout = new QVBoxLayout;
	boxLayout->addWidget(headerName);

	QGridLayout * chkLayout = new QGridLayout;
	boxLayout->addLayout(chkLayout);
	//chkLayout->addWidget(headerName, 0, 0);
	int row = 1;
	int col = 0;
	for(int i = 0 ; i < cnt ; i++){
		if(i > 0){
			if(i%3 == 0){
				row++;
				col = 0;
			}
		}

		if(row - 1 == 0){
			chkLayout->addWidget(new QLabel(tr("<h4>No.</h4>")),row-1,col * numColumn,Qt::AlignCenter);
			chkLayout->addWidget(new QLabel(tr("<h4>Name</h4>")),row-1,col * numColumn + 1, Qt::AlignCenter);
		}
		lblHeader[i] = new QLabel(QString().setNum(i, 10));
		txtHeader[i] = new QLineEdit(listHeader.at(i));
		txtHeader[i]->resize(140,20);
		chkHeader[i] = new QCheckBox;
		chkLayout->addWidget(lblHeader[i],row,col * numColumn);
		chkLayout->addWidget(txtHeader[i],row,col * numColumn + 1);
		chkLayout->addWidget(chkHeader[i],row,col * numColumn + 2);
		qDebug()<<"HeaderName : "<<txtHeader[i]->text();

		col++;
	}

	return boxLayout;
}

QGridLayout * DAFIF_PARSER::initTableParam()
{
	qDebug()<<"initTableParam is called..";
	//테이블과 파라미터에 관련된 레이아웃 설정
	QGridLayout * tableLayout = new QGridLayout;

	QLabel * lblTable = new QLabel(tr("<h3>Table Name</h3>"));
	QLabel * lblParam1 = new QLabel(tr("Param1 : "));
	QLabel * lblParam2 = new QLabel(tr("Param2 : "));
	QLabel * lblTableName = new QLabel(tr("Table Name : "));
	lblHelp = new QLabel;
	txtTableName = new QLineEdit;
	txtParam1 = new MyLineEdit;
	txtParam2 = new MyLineEdit;
	tableLayout->addWidget(lblTable, 0, 0);
	tableLayout->addWidget(lblTableName,1,0);
	tableLayout->addWidget(txtTableName, 1, 1);
	tableLayout->addWidget(lblParam1, 2, 0);
	tableLayout->addWidget(txtParam1, 2, 1);
	tableLayout->addWidget(lblParam2, 3, 0);
	tableLayout->addWidget(txtParam2, 3, 1);
	tableLayout->addWidget(lblHelp, 4, 1);

	txtParam1->setObjectName("txtParam1");
	txtParam2->setObjectName("txtParam2");
	connect(txtParam1, SIGNAL(focussed(bool )), this, SLOT(setHelpLabel(bool )));
	connect(txtParam2, SIGNAL(focussed(bool )), this, SLOT(setHelpLabel(bool )));


	return tableLayout;

}

void DAFIF_PARSER::runSelectAll()
{
	qDebug()<<"runSelectAll is called..";

	for(int i = 0 ; i < cnt; i++){
		chkHeader[i]->setChecked(true);
	}

}

void DAFIF_PARSER::runDeSelectAll()
{

	for(int i = 0 ; i < cnt; i++){
		chkHeader[i]->setChecked(false);
	}
}

void DAFIF_PARSER::runClear()
{
	init();
	repaint();
}


void DAFIF_PARSER::resizeEvent(QResizeEvent * e)
{
	qDebug()<<"Resize Event is called..";
	this->adjustSize();
}

QMap<QString, QString> DAFIF_PARSER::parsingParam2(QStringList param2)
{
	QMap<QString, QString> paramMap;
	qDebug()<<"ParsingParam2 .. "<<param2;
	for(int j = 0 ; j < param2.count() ; j++){
		QStringList paramList = param2.at(j).split('=');
		paramMap.insert(paramList.at(0), paramList.at(1));
	}

	qDebug()<<"ParamMap : "<<paramMap;

	return paramMap;
}


void DAFIF_PARSER::setHelpLabel(bool hasFocus)
{
	qDebug()<<"[DAFIF_PARSER] setHelpLabel is called..";
	MyLineEdit * temp = (MyLineEdit *)sender();
	qDebug()<<temp->objectName();
	QString str = "";
	if(temp->objectName() == "txtParam1"){
		str = "usage : column name/column name/column name...../column name";
	}else if(temp->objectName() == "txtParam2"){
		str = "usage : column name=value/column name=value...";
	}
	if(hasFocus ==  true)
	{
		lblHelp->setText(str);
	}else{
		//temp->setCursorPosition(0);
		lblHelp->setText("");
	}

}
