#ifndef RKSIPARSER_H
#define RKSIPARSER_H

#include <QtGui>
#include <QXmlStreamReader>
#include <QVector>
#include "ConvertDataInfo.h"
#include "lib/StyleManager.h"
#include "lib/Function.h"
#include "ui_rksiparser.h"
#include <QTextCodec>
#include <QRadioButton>



class RKSIParser : public QWidget, public  Ui::RKSIParserClass
{
    Q_OBJECT

public:
	RKSIParser(QWidget *parent = 0);
	~RKSIParser();
	void setTargetName(QString nFileName);
	void makeMapFile(QString cLayer, QString nLayer, QTextStream & out, ConvertDataInfo &convertDataInfo);
	void processXmlParsing(QFile & file);
	void parseFirstFolder();
	void parseSecondFolder();
	void parseThirdFolder();
	void parsePlaceMark();
	QString ParseLatLon();
	//void parseFolder(QXmlStreamReader * reader);
	void processLatLon(ConvertDataInfo & convertDataInfo);
	Lon convertLon(QString lon);
	Lat convertLat(QString lat);
	QString getDegree(QString &var1);
	QString getMin(QString &var1);
	QString getSec(QString &var1);
	QString getMilliSec(QString &var1);
	void clearInfo();
	void openFile();


private:

	QVector<ConvertDataInfo> convertDataInfoList;	//맵 데이터를 생성하기 위한 클래스.. 해당 클래스는 하나의 맵 ID를 생성하고 좌표를 관리 한다.
	QXmlStreamReader * reader;							//Xml 파일을 읽기 위한 포인터 객
	QFile * openedFile;									//opened file
	QString mlayer;											//메인 레이어 이름..
	QString cFileDir;										//directory path
	QString fileName;										//생성될 map 파일이름. 디렉토리 path는 제외
	QString cfilename;										//full file name
	int idNum;												//생성될 좌표의 ID 번호, notitle에서만 적용됨.
	int layerLevel;											//라디오 버튼에 의해 선택되는 레이어 레벨

public slots:

	void openMapFile();
	void convertMapFile();
	void setLayerLevel();			//Layer 레벨을 결정한다..
	//test
	void test();
};

#endif // RKSIPARSER_H
