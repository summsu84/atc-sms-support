#include "mainwindow.h"

Mainwindow::Mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
	//ui.setupUi(this);
	qDebug()<<"test..";
	mdiArea = new QMdiArea(this);
	//leftLayout = 0;
	leftLayout = new LeftLayout(this);
	rightLayout = new RightLayout(this);
	leftDockWidget = new QDockWidget(tr("LAYER"));
	leftDockWidget->setObjectName("");
	leftDockWidget->setWidget(leftLayout);
	leftDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	leftDockWidget->setMinimumWidth(200);
	addDockWidget(Qt::LeftDockWidgetArea, leftDockWidget);

/*
	mdiArea->setWindowFlags(Qt::WindowTitleHint);
	mdiArea->setAttribute(Qt::WA_DeleteOnClose);
*/

	mdiArea->addSubWindow(rightLayout);
	this->setCentralWidget(mdiArea);

	QString title = "SMS Support Program";

	setWindowTitle(title);
	//rksiParser = new RKSIParser(this);

	createActions();
	createMenus();
	createToolBars();
	this->updateGeometry();
	createStatusBar();

	//setCurrentFile("");
	//connect
	//this->resize(900,600);
	showMaximized();
	connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(statusMessage()));
	connect(leftLayout, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(leftMenuChanged(QTreeWidgetItem *, int)));
	//connect(this,	SIGNAL(openKmlFile(QString )),	rightLayout, SLOT(processKmlFile(QString)));
	connect(rightLayout, SIGNAL(emitOpenKmlFile(QString )), this, SLOT(msgStatusMessage(QString )));

	rightLayout->showMaximized();
	status = 0;
	//qDebug()<<"MaximuWidth : "<<mdiArea->maximumSize();
	//this->update();
	/*layer_level = LAYER_LEVEL_3;
	layer_level_3->setChecked(true);*/
}

Mainwindow::~Mainwindow()
{

}

void Mainwindow::statusMessage()
{

}
void Mainwindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}


void Mainwindow::open()
{
   QFileDialog fdialog(this);
	qDebug()<<"File Mode" <<fdialog.fileMode();
	QString dir = QDir::currentPath();
	cfilename = fdialog.getOpenFileName(this, tr("Open the kml file"),
			dir, tr("dat file(*.kml *.TXT)"));

	if (!cfilename.isEmpty()) {
		qDebug() << "fileName : " << cfilename;
	}else{
		qDebug() <<"[Mainwindow] - oepn() - File is not selected..";
		return;
	}
	rightLayout->processKmlFile(cfilename);

	//현재 kml 파일의 레이어 레벨이 얼마 까지인지 판단..
	int layer = FolderManager::getInstance().processLayerParsing(cfilename);
	qDebug()<<"Layer : "<<layer;
	if(layer == LAYER_LEVEL_0){
		layer_level_1->setEnabled(false);
		layer_level_2->setEnabled(false);
		layer_level_3->setEnabled(false);
	}else if(layer == LAYER_LEVEL_1){
		layer_level_1->setEnabled(true);
		layer_level_2->setEnabled(false);
		layer_level_3->setEnabled(false);
	}else if(layer == LAYER_LEVEL_2){
		layer_level_1->setEnabled(true);
		layer_level_2->setEnabled(true);
		layer_level_3->setEnabled(false);
	}else if(layer == LAYER_LEVEL_3){
		layer_level_1->setEnabled(true);
		layer_level_2->setEnabled(true);
		layer_level_3->setEnabled(true);
	}else{
		qDebug()<<"error!..";
		return;
	}
}

void Mainwindow::initTreeWidget()
{
	if(leftLayout != 0){
		disconnect(leftLayout, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(leftMenuChanged(QTreeWidgetItem *, int)));
		delete leftLayout;
	}

	leftLayout = new LeftLayout(this);
	connect(leftLayout, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(leftMenuChanged(QTreeWidgetItem *, int)));
	leftDockWidget->setWidget(leftLayout);

}
void Mainwindow::convert()
{
	qDebug()<<"convert start..";

	//validation check
	if(layer_level_1->isChecked() == false && layer_level_2->isChecked() == false && layer_level_3->isChecked() == false){
		QMessageBox::warning(this, tr("Error"), tr("Please select layer level!"));
		return;
	}

	initTreeWidget();
	//FolderManager를 호출하여, map 형식으로 변환한다..
	FolderManager::getInstance().setProcessMode(FILE_MODE);
	QString targetFilename = FolderManager::getInstance().processXmlParsing(cfilename);

	//TreeMenu 생성

	leftLayout->genTreeMenu(FolderManager::getInstance().getMapFileName(), FolderManager::getInstance().getLayerNames());
	rightLayout->processKmlFile(targetFilename);

}

void Mainwindow::changeLayerLevel()
{
	QAction * action = (QAction *)sender();
	//qDebug()<<"action name "<<action->text();
	QString actionName = action->text();
	if(actionName == "Layer_Level_1"){
		layer_level = LAYER_LEVEL_1;
		layer_level_2->setChecked(false);
		layer_level_3->setChecked(false);
	}else if(actionName == "Layer_Level_2"){
		layer_level = LAYER_LEVEL_2;
		layer_level_1->setChecked(false);
		layer_level_3->setChecked(false);
	}else{
		layer_level = LAYER_LEVEL_3;
		layer_level_1->setChecked(false);
		layer_level_2->setChecked(false);
	}

	qDebug()<<"Layer Level :"<<layer_level;
	FolderManager::getInstance().setLayerLevel(layer_level);

}
void Mainwindow::changeMode()
{
	QAction * action = (QAction *)sender();
	//qDebug()<<"action name "<<action->text();
	QString actionName = action->text();
	if(actionName == "Replace"){
		mode = REPLACE_MODE;
		replace_mode->setChecked(true);
		insert_mode->setChecked(false);
	}else if(actionName == "Insert"){
		mode = INSERT_MODE;
		replace_mode->setChecked(false);
		insert_mode->setChecked(true);
	}
}


void Mainwindow::about()
{
    QMessageBox::about(this, tr("About SMS Support Util"),
            tr("<h2>Spreadsheet 1.1</h2>"
               "<p>Copyright &copy; 2008 Software Inc."
               "<p>Spreadsheet is a small application that "
               "demonstrates QAction, QMainWindow, QMenuBar, "
               "QStatusBar, QTableWidget, QToolBar, and many other "
               "Qt classes."));
}

void Mainwindow::createActions()
{

    openAction = new QAction(tr("&Open..."), this);
    openAction->setIcon(QIcon(":/open.png"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing spreadsheet file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

	convertAction = new QAction(tr("&Convert..."), this);
	convertAction->setIcon(QIcon(":/gotocell.png"));
	convertAction->setShortcut(QKeySequence::Open);
	convertAction->setStatusTip(tr("Convert kml to map type"));
	connect(convertAction, SIGNAL(triggered()), this, SLOT(convert()));

	//DB
	dbAction = new QAction(tr("&DB Process..."), this);
	dbAction->setIcon(QIcon(":/todb.png"));
	dbAction->setShortcut(QKeySequence::Open);
	dbAction->setStatusTip(tr("Insert kml file whicl is converted to map file to SMS DB"));
	connect(dbAction, SIGNAL(triggered()), this, SLOT(dbProcess()));


	//DAFIF
	dafif_apcc_icao = new QAction(tr("Run &APCC_ICAO"), this);
	dafif_apcc_icao->setStatusTip(tr("Run APCC_ICAO"));
	connect(dafif_apcc_icao, SIGNAL(triggered()), this, SLOT(runDafifApccIcao()));

	//XML PARSER
	xmlParser = new QAction(tr("Run &XML Parsing"), this);
	xmlParser->setStatusTip(tr("Parsing STCA, MSAW,.."));
	connect(xmlParser, SIGNAL(triggered()), this, SLOT(runXmlParser()));

	//XML PARSER IMPORT
	xmlParserImport = new QAction(tr("Run &XML Parsing and Import"), this);
	xmlParserImport->setStatusTip(tr("Parsing STCA, MSAW,.."));
	connect(xmlParserImport, SIGNAL(triggered()), this, SLOT(runXmlParserImport()));

	//XML PARSER IMPORT
	xmlReadWrite = new QAction(tr("Run &XML Read and Write"), this);
	xmlReadWrite->setStatusTip(tr("Parsing STCA, MSAW,.."));
	connect(xmlReadWrite, SIGNAL(triggered()), this, SLOT(runXmlReadWrite()));

	//DAFIF PARSER
	dafifParser = new QAction(tr("Run &DAFIF Parsing"), this);
	dafifParser->setStatusTip(tr("Parsing DAFIF data,.."));
	connect(dafifParser, SIGNAL(triggered()), this, SLOT(runDafifParser()));

	//Basic Drawing
	drawing_basic = new QAction(tr("Run Basic Drawing"), this);
	drawing_basic->setStatusTip("Draw basic components");
	connect(drawing_basic, SIGNAL(triggered()), this, SLOT(runDrawingBasic()));

	//Basic Drawing_2
	drawing_basic_2 = new QAction(tr("Run Basic Drawing_2"),this);
	drawing_basic_2->setStatusTip("Draw basic components");
	connect(drawing_basic_2, SIGNAL(triggered()), this, SLOT(runDrawingBasic_2()));

	//Drawing Scribble
	drawing_scribble = new QAction(tr("Run Scribble Drawing"),this);
	drawing_scribble->setStatusTip("Draw basic components");
	connect(drawing_scribble, SIGNAL(triggered()), this, SLOT(runDrawingScribble()));

	//Drawing_04
	drawing_04 = new QAction(tr("Run Drawing_04"),this);
	drawing_04->setStatusTip("Draw basic components");
	connect(drawing_04, SIGNAL(triggered()), this, SLOT(runDrawing_04()));

	//thread 01
	thread_0 = new QAction(tr("Run Thread_01"),this);
	thread_0->setStatusTip("run basic thread");
	connect(thread_0, SIGNAL(triggered()), this, SLOT(runThread_0()));



    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Show the application's About box"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    layer_level_1 = new QAction(tr("Layer_Level_1"), this);
    layer_level_2 = new QAction(tr("Layer_Level_2"), this);
    layer_level_3 = new QAction(tr("Layer_Level_3"), this);

    layer_level_1->setShortcut(tr("Ctrl_Shift+1"));
    layer_level_2->setShortcut(tr("Ctrl_Shift+2"));
    layer_level_3->setShortcut(tr("Ctrl_Shift+3"));

    layer_level_1->setCheckable(true);
    layer_level_2->setCheckable(true);
    layer_level_3->setCheckable(true);

    connect(layer_level_1, SIGNAL(triggered()), this, SLOT(changeLayerLevel()));
    connect(layer_level_2, SIGNAL(triggered()), this, SLOT(changeLayerLevel()));
    connect(layer_level_3, SIGNAL(triggered()), this, SLOT(changeLayerLevel()));
    //Replace and Insert Mode
    replace_mode = new QAction(tr("Replace"), this);
    insert_mode  = new QAction(tr("Insert"), this);

    replace_mode->setCheckable(true);
    insert_mode->setCheckable(true);
    connect(replace_mode, SIGNAL(triggered()), this, SLOT(changeMode()));
    connect(insert_mode, SIGNAL(triggered()), this, SLOT(changeMode()));

}

void Mainwindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    convertMenu = menuBar()->addMenu(tr("&Convert"));
    convertMenu->addAction(convertAction);
    convertMenu->addSeparator();
    convertMenu->addAction(layer_level_1);
    convertMenu->addAction(layer_level_2);
    convertMenu->addAction(layer_level_3);
    convertMenu->addSeparator();
    convertMenu->addAction(dbAction);

    DAFIF = menuBar()->addMenu(tr("&Dafif"));
    DAFIF->addAction(dafif_apcc_icao);
    DAFIF->addSeparator();
    DAFIF->addAction(insert_mode);
    DAFIF->addAction(replace_mode);
    //Parser
    parserMenu = menuBar()->addMenu(tr("&Parser"));
    parserMenu->addAction(xmlParser);
    parserMenu->addAction(xmlParserImport);
    parserMenu->addAction(xmlReadWrite);
    parserMenu->addAction(dafifParser);

    //Drawing
    drawingMenu = menuBar()->addMenu(tr("DRAWING"));
    drawingMenu->addAction(drawing_basic);
    drawingMenu->addAction(drawing_basic_2);
    drawingMenu->addAction(drawing_scribble);
    drawingMenu->addAction(drawing_04);

    //thread
	 threadMenu = menuBar()->addMenu(tr("Thread"));
	 threadMenu->addAction(thread_0);


    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

void Mainwindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(convertAction);
    fileToolBar->addAction(dbAction);

}
bool Mainwindow::okToContinue()
{
   /* if (isWindowModified()) {
        int r = QMessageBox::warning(this, tr("Spreadsheet"),
                        tr("The document has been modified.\n"
                           "Do you want to save your changes?"),
                        QMessageBox::Yes | QMessageBox::No
                        | QMessageBox::Cancel);
        if (r == QMessageBox::Yes) {
            return true;
        } else if (r == QMessageBox::Cancel) {
            return false;
        }
    }*/
    return true;
}
void Mainwindow::writeSettings()
{
    QSettings settings("Software Inc.", "Spreadsheet");

    settings.setValue("geometry", saveGeometry());
    /*settings.setValue("recentFiles", recentFiles);
    settings.setValue("showGrid", showGridAction->isChecked());
    settings.setValue("autoRecalc", autoRecalcAction->isChecked());*/
}

void Mainwindow::createStatusBar()
{
	QString msg = "Lines : ";
	infoLabel = new QLabel(msg);
	infoLabel->setAlignment(Qt::AlignHCenter);
	infoLabel->setMinimumSize(infoLabel->sizeHint());
	msgLabel = new QLabel;
	msgLabel->setIndent(3);

	statusBar()->addWidget(infoLabel);
	statusBar()->addWidget(msgLabel);
}

void Mainwindow::msgStatusMessage(QString msg)
{
	msgLabel->setText(msg);
}


void Mainwindow::leftMenuChanged(QTreeWidgetItem * item, int column)
{
	qDebug()<<"leftMenuChanged start..";
	QString layer = "LAYER:";
	layer.append(item->text(column));
	if(!item->parent()){
		qDebug()<<"Top level : "<<item->text(column);
		QTextCursor * cursor = new QTextCursor();
		cursor->selectionStart();
		rightLayout->moveCursor(QTextCursor::Start);
	}else{
		qDebug()<<"parent : "<<item->parent()->text(column)<<"selected layer : "<<layer;
		if(!rightLayout->find(layer)){
			rightLayout->moveCursor(QTextCursor::Start);
			rightLayout->find(layer);
		}
	}
}

void Mainwindow::runDafifApccIcao()
{
	if(replace_mode->isChecked() == false && insert_mode->isChecked() == false)
	{
		QMessageBox::warning(this, tr("Error"), tr("Select Repalce or Insert mode"));
		return;
	}
	dafif = new Dafif;
	bool chk;
	if (dafif->readFile(cfilename, DA_APCC_ICAO) == false) {
		QMessageBox::warning(this, tr("File format error"), dafif->errorMessage);
		return;
	}
	QMessageBox::information(this, tr("Success"), tr("Reading file is completed"));

	dafif->PrtDafif();
	if(mode == REPLACE_MODE){
		if(dafif->deleteAll("TB_DA_CNTRYCD") == false){
			QMessageBox::warning(this, tr("Database error"), dafif->errorMessage);
			return;
		}
	}
	//트랜 잭션 시작..
	QSqlDatabase::database(UserInfo::getInstance().getDbname()).transaction();
	if(dafif->insertTables(DA_APCC_ICAO) == false){
		QMessageBox::warning(this, tr("Database error"), dafif->errorMessage);
		return;
	}

	QSqlDatabase::database(UserInfo::getInstance().getDbname()).commit();
	QMessageBox::information(this, tr("Success"), tr("Insert process is completed"));
}

/**
 * XML Parser를 호출 한다.
 */
void Mainwindow::runXmlParser()
{
	int result;
	XMLParser * parser = new XMLParser;
	parser->setModal(true);
	result = parser->exec();
	qDebug()<<"XML Parser is closed... "<<result;
	if(result == QDialog::Rejected){
		delete parser;
	}else{
		delete parser;
	}
}



/**
 * DAFIF Parser를 호출 한다.
 */
void Mainwindow::runDafifParser()
{
	int result;

	DAFIF_PARSER * parser = new DAFIF_PARSER;
	parser->setModal(true);
	result = parser->exec();
	qDebug()<<"XML Parser is closed... "<<result;
	if(result == QDialog::Rejected){
		delete parser;
	}else{
		delete parser;
	}
}


/**
 *  XML ParserImport 를 호출 한다.
 */
void Mainwindow::runXmlParserImport()
{
	int result;

	/*XMLImportWidget * xmlWidget = new XMLImportWidget;
	xmlWidget->setModal(true);
	result = xmlWidget->exec();
	qDebug()<<"XML Parser is closed... "<<result;
	if(result == QDialog::Rejected){
		delete xmlWidget;
	}else{
		delete xmlWidget;
	}*/
	XMLImport * xml = new XMLImport;

}

void Mainwindow::runXmlReadWrite()
{
	XMLReadWriter * rw = new XMLReadWriter;
}

void Mainwindow::runDrawingBasic()
{
	WINDOW * w = new WINDOW;
	w->show();
}

void Mainwindow::runDrawingBasic_2()
{
	Window02 * w = new Window02;
	w->show();
}

void Mainwindow::runDrawingScribble()
{
	Window03 * w = new Window03;

	w->resize(512,512);
	QWidget * desktop = QApplication::desktop();
	QRect * rect = new QRect(desktop->geometry());
	QPoint * form = new QPoint(w->width(),w->height());
	int cx = ((rect->center().x()) - (form->x()/2));
	int cy = ((rect->center().y()) - (form->y()/2));
	w->move(cx, cy);
	w->show();
}

void Mainwindow::runDrawing_04()
{
	Window04 * w = new Window04;
	w->show();
	//w->setParent(this);
}

void Mainwindow::runThread_0()
{
	MyThread * thread = new MyThread;
	//thread->setRightLayout(rightLayout);
	connect(thread, SIGNAL(emitMsg(QString)), this, SLOT(setMsg(QString)));
	thread->start();

}

void Mainwindow::setMsg(QString msg)
{
	rightLayout->setPlainText(rightLayout->toPlainText().append('\n'+msg));
}

/**
 * kml 파일을 읽고, 읽은 내용을 smsdev에 저장한다..
 */
void Mainwindow::dbProcess()
{
	qDebug()<<"DB process is enabled..";

	QChar prefix;
	bool chk;

	initTreeWidget();
	//FolderManager를 호출하여, map 형식으로 변환한다..
	FolderManager::getInstance().setProcessMode(DB_MODE);
	QString targetFilename = FolderManager::getInstance().processXmlParsing(cfilename);

	//TreeMenu 생성
	leftLayout->genTreeMenu(FolderManager::getInstance().getMapFileName(), FolderManager::getInstance().getLayerNames());
	qDebug()<<"[Mainwindow] dbProcess - file name : "<<targetFilename;
	rightLayout->processKmlFile(targetFilename);



	//DB Open
	DbManager * d_manager = new DbManager(FolderManager::getInstance().getConvertDataInfoList());

	//connect(d_manager, SIGNAL(emitDuplication(QString )), this, SLOT(genDuplication(QString )));

	chk = d_manager->dbOpen();
	if(!chk){
		initTreeWidget();
		rightLayout->setPlainText("");
		QMessageBox::critical(0, QObject::tr("DataBase Open Error"), QObject::tr("DataBase Open Error!"));
		return;
	}


	//DB 작업
	QSqlDatabase::database(UserInfo::getInstance().getDbname()).transaction();

	chk = d_manager->insertProcess(prefix);
	QString msg;

	if(chk == false){
		QMessageBox::critical(this, "Error", d_manager->errorMessage);
		QSqlDatabase::database(UserInfo::getInstance().getDbname()).rollback();
	}else{
		msg = "It is completed\n";
		if(prefix == 'C'){
			msg += "TB_COAST count : "+QString().setNum(d_manager->mainCnt)+", TB_COASTBNDRY count : "+QString().setNum(d_manager->subCnt)+"";
		}else if(prefix == 'S'){
			msg += "TB_SIDSTAR count : "+QString().setNum(d_manager->subCnt)+"";
		}else if(prefix == 'A'){
			msg += "TB_CLSMAP count : "+QString().setNum(d_manager->mainCnt)+", TB_CLSMAP count : "+QString().setNum(d_manager->subCnt)+"";
		}
		QMessageBox::information(this, "Success", msg);
		QSqlDatabase::database(UserInfo::getInstance().getDbname()).commit();
	}

	delete d_manager;
	if(!QFile::remove(targetFilename))
	{
		qDebug()<<"Deleting file is error";
	}else{
		qDebug()<<"Deleting file is success";
	}
}


