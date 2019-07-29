/*
 * XMLImport.cpp
 *
 *  Created on: 2013. 12. 4.
 *      Author: jiwon
 */

#include "XMLImport.h"

XMLImport::XMLImport() {
	// TODO Auto-generated constructor stub

	qDebug()<<"[XMLImport] XMLImport..";

	dbManager = new DbManager;
	bool chk = dbManager->dbOpen();
	if(!chk){
		qDebug()<<"DB Open Fail...";
		return;
	}

	openFile();

}

XMLImport::~XMLImport() {
	// TODO Auto-generated destructor stub
}



//파일을 열고 헤더를 읽어온다..
QStringList XMLImport::openFile()
{
	qDebug()<<"Open File is called..";
	QString dir = QDir::currentPath();
	QFileDialog fdialog;
	QStringList tmp;
	//fdialog.setFileMode(QFileDialog::ExistingFiles);



	qDebug()<<"h1";
	fileName = fdialog.getOpenFileName(0, QString("Open the xml file"),
			dir, QString("xml file(*.xml)"));



	QFile file(fileName);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		errorMessage = QString(QObject::tr("Cannot read file %1:\n%2")
									.arg(file.fileName())
									.arg(file.errorString()));
		qDebug()<<"h2";
		return tmp;
	}

	tmp = parsingElement(file);

	file.close();
	return tmp;
}

/**
 * Header들을 검출 한다.
 */

QStringList XMLImport::parsingElement(QFile &file)
{
	QDomDocument doc("myDocument");
	if(!doc.setContent(&file)){
		file.close();
		return QStringList()<<"error";
	}

	//Root 검출
	QDomElement docElem = doc.documentElement();

	qDebug()<<"root : "<<docElem.tagName();

	QDomNode n = docElem.firstChild();
	while(!n.isNull()){
		if(n.isElement()){
			QDomElement e = n.toElement();
			qDebug()<<"E NAME : "<<e.tagName();
			QString ad_id;
			QString icao_cd;
			QDomNode node = n.firstChild();
			while(!node.isNull()) {
				QDomElement ne = node.toElement();
				if(ne.tagName() == "IcaoAdCode")
				{
					ad_id = ne.text();
				}else if(ne.tagName() == "CountryCode")
				{
					icao_cd = ne.text();
				}
				//qDebug()<<"TAG : "<<ne.tagName();
				node = node.nextSibling();
			}
			//qDebug()<<"AD : "<<ad_id<<", ICAO : "<<icao_cd;
			AD_INFO ad_info;
			ad_info.ad_id = ad_id;
			ad_info.icao_cd = icao_cd;
			ad_list.append(ad_info);
		}

		n = n.nextSibling();
	}

	bool chk;
	QVector<AD_INFO> var1;
	QVector<AD_INFO> var2;
	QVector<AD_INFO> var3;
	int cnt = 0;

	for(int i = 0 ; i < ad_list.size() ; i++)
	{
		int type = 0;
		QString ad_id = ad_list.at(i).ad_id;
		QString icao_cd = ad_list.at(i).icao_cd;
		//qDebug()<<"AD ID : "<<ad_list.at(i).ad_id<<", ICAO : "<<ad_list.at(i).icao_cd;
		chk = dbManager->insertProcess(ad_id, icao_cd, "", type);
		AD_INFO ad_info;
		ad_info.ad_id = ad_id;
		ad_info.icao_cd = icao_cd;
		if(chk == false)
		{
			if(type == 1){
				var1.append(ad_info);
			}else if(type == 2){
				var2.append(ad_info);
			}
		}else{
			var3.append(ad_info);
			cnt++;
		}
	}

	qDebug()<<"INSERT SUCCESS CNT : "<<cnt;

	qDebug()<<"NO TB_CNTRYCD...";
	for(int i = 0 ; i < var1.size() ; i++)
	{
		qDebug()<<"["<<i<<"] NO TB_CNTRCYCD AD_ID : "<<var1.at(i).ad_id<<", ICAO_CD : "<<var1.at(i).icao_cd;
	}
	qDebug()<<"EXIST ON TB_AD...";
	for(int i = 0 ; i < var2.size() ; i++)
	{
		qDebug()<<"["<<i<<"] DUP ON TB_AD.. AD_ID : "<<var2.at(i).ad_id<<", ICAO_CD : "<<var2.at(i).icao_cd;
	}
	qDebug()<<"INSERT INTO TB_AD...";
	for(int i = 0 ; i < var3.size() ; i++)
	{
		qDebug()<<"["<<i<<"] INSERT INTO TB_AD.. AD_ID : "<<var3.at(i).ad_id<<", ICAO_CD : "<<var3.at(i).icao_cd;
	}


	//Insert

	return QStringList()<<"result..";
}





XMLImportWidget::XMLImportWidget(QDialog * parent)
: QDialog(parent)
{
	qDebug()<<"XML Import Widget is called..";
	importer = new XMLImport;
	stackedLayout = new QStackedLayout;
	init();
	QVBoxLayout * mainLayout = new QVBoxLayout;
	mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	mainLayout->addLayout(stackedLayout);

	this->setLayout(mainLayout);
	setWindowTitle(tr("DAFIF Parser"));
}

XMLImportWidget::~XMLImportWidget()
{

}


void XMLImportWidget::initLayout()
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

void XMLImportWidget::initBtn()
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


void XMLImportWidget::clear()
{
	qDebug()<<"Clear is called..";

	fileName = "";
	cnt = 0;


	txtTableName = 0;
//	txtParam1 = 0;
//	txtParam2 = 0;
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


void XMLImportWidget::init()
{
	clear();
	initBtn();
	initLayout();
}


QGridLayout * XMLImportWidget::initTableParam()
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
//	txtParam1 = new MyLineEdit;
//	txtParam2 = new MyLineEdit;
	tableLayout->addWidget(lblTable, 0, 0);
	tableLayout->addWidget(lblTableName,1,0);
	tableLayout->addWidget(txtTableName, 1, 1);
	tableLayout->addWidget(lblParam1, 2, 0);
	//tableLayout->addWidget(txtParam1, 2, 1);
	tableLayout->addWidget(lblParam2, 3, 0);
	//tableLayout->addWidget(txtParam2, 3, 1);
	tableLayout->addWidget(lblHelp, 4, 1);

//	txtParam1->setObjectName("txtParam1");
//	txtParam2->setObjectName("txtParam2");
//	connect(txtParam1, SIGNAL(focussed(bool )), this, SLOT(setHelpLabel(bool )));
//	connect(txtParam2, SIGNAL(focussed(bool )), this, SLOT(setHelpLabel(bool )));


	return tableLayout;

}


/**
 * 헤더 리스트를 파싱하여 헤더의 순서와 이름을 표시한다.
 */
QVBoxLayout * XMLImportWidget::setCheckBox(QStringList listHeader)
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


//파일을 열고 헤더를 읽어온다..
QStringList XMLImportWidget::openFile()
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

/**
 * 컬럼 이름을 검출 한다..
 * @param header
 * @return
 */
QStringList XMLImportWidget::parsingHeader(const QString header)
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
