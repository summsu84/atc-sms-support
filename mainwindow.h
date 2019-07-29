#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
//#include "ui_mainwindow.h"
//#include "rksiparser.h"
#include "LeftLayout.h"
#include "RightLayout.h"
#include "lib/StyleManager.h"
#include "lib/FolderManager.h"
#include "DAFIF/Dafif.h"
#include "XMLParser/XMLParser.h"
#include "PARSING/PARSER/DAFIF_PARSER.h"
#include "PARSING/XML/XML_IMPORT/XMLImport.h"
#include "PARSING/XML/XML_WRITER/XMLReadWriter.h"
#include "DRAWING/WINDOW.h"
#include "DRAWING/DRAWING02/Window02.h"
#include "DRAWING/DRAWING03/Window03.h"
#include "DRAWING/DRAWING04/Window04.h"
#include "THREAD/MyThread.h"
#include "lib/DbManager.h"

class QAction;
class QLabel;

class Mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    Mainwindow(QWidget *parent = 0);
    ~Mainwindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
	void open();
	void convert();
	void dbProcess();
	void about();
	void statusMessage();
	void msgStatusMessage(QString msg);
	void leftMenuChanged(QTreeWidgetItem * item, int column);
	void changeLayerLevel();
	void changeMode();

	//XML Parser
	void runXmlParser();
	void runDafifParser();
	void runXmlParserImport();
	void runXmlReadWrite();
	//Dafif Method
	void runDafifApccIcao();
	//Drawing Basic
	void runDrawingBasic();
	void runDrawingBasic_2();
	void runDrawingScribble();
	void runDrawing_04();
	void runThread_0();
	void setMsg(QString msg);
private:
    //Ui::MainwindowClass ui;
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	bool okToContinue();
	void writeSettings();
	void initTreeWidget();

	QLabel * infoLabel;
	QLabel * msgLabel;

	QMenu * fileMenu;						//fileMenu
	QMenu * convertMenu;					//Convert Menu
	QMenu * helpMenu;
	QMenu * DAFIF;							// DAFIF Menu
	QMenu * parserMenu;					// Parser Menu
	QMenu * drawingMenu;					//Drawing Menu
	QMenu * threadMenu;					//thread menu

	QToolBar *fileToolBar;
	QAction * openAction;					//kml 파일을 오픈한다..
	QAction * convertAction;				//kml을 map파일로 컨버터 하는 액션..
	QAction * dbAction;					//kml을 파일을 adaptaton DB에 저장하는 액션..
	QAction * exitAction;
	QAction * aboutAction;
	QAction * aboutQtAction;
	QAction * dafif_apcc_icao;			//APCC_ICAO 국가 코드 실행
	QAction * xmlParser;					//XMLParser
	QAction * dafifParser;				//DAFIF 관련 Parser
	QAction * xmlParserImport;			//XML파싱 후 데이터베이스에 Import
	QAction * xmlReadWrite;

	QAction * drawing_basic;				//Basic Drawing
	QAction * drawing_basic_2;			//Basic Drawing_2
	QAction * drawing_scribble;			//Scribble Drawing
	QAction * drawing_04;					//Drawing_04

	QAction * thread_0;					//Thead


	//Layer Option
	QAction * layer_level_1;
	QAction * layer_level_2;
	QAction * layer_level_3;
	//Replace all Insert Mode;
	QAction * replace_mode;
	QAction * insert_mode;



	//RKSIParser * rksiParser;
	LeftLayout * leftLayout;
	RightLayout * rightLayout;

	QDockWidget * leftDockWidget;			//left dock
	QMdiArea	*	mdiArea;
	int status;
	int layer_level;
	Mode mode;
	QString cfilename;
	Dafif * dafif;

};

#endif // MAINWINDOW_H
