/*
 * FolderManager.h
 *
 *  Created on: 2012. 11. 1.
 *      Author: jiwon
 */

#ifndef FOLDERMANAGER_H_
#define FOLDERMANAGER_H_

#include <QtGui>
#include <QXmlStreamReader>
#include <QVector>
#include "ConvertDataInfo.h"
#include "StyleManager.h"
#include "Function.h"
#include <QRadioButton>

enum{
	KML,
	MAP
};

/*
 * 해당 클래스는 kml 파일의 Folder 엘리먼트를 관리 하는 클래스 이다..
 */

class FolderManager {

public:
	FolderManager();
	virtual ~FolderManager();
	//singltone
	static FolderManager & getInstance(){
		if(pInstance == NULL){
			pInstance = new FolderManager;
		}
		return * pInstance;
	}
	void initPointer();
	void openFile();
	QString processXmlParsing(QString kmlfileName);
	int processLayerParsing(QString kmlfileName);
	void parseFirstFolder();
	void parseSecondFolder();
	void parseThirdFolder();
	void parsePlaceMark();
	QString ParseLatLon();
	void processLatLon(ConvertDataInfo & convertDataInfo);
	Lon convertLon(QString lon);
	Lat convertLat(QString lat);
	QString getDegree(QString &var1);
	QString getMin(QString &var1);
	QString getSec(QString &var1);
	QString getMilliSec(QString &var1);
	void clearInfo();
	void makeMapFile(QString cLayer, QString nLayer, QTextStream & out, ConvertDataInfo &convertDataInfo);
	void convertMapFile();
	void setTargetName(QString nFileName);
	QStringList getLayerNames(){return layerNames;}
	QString getMapFileName(){ return mapfilename;}
	void setLayerLevel(int _layerLevel);
	void setProcessMode(Mode mode) {this->process_mode = mode;}
	QVector<ConvertDataInfo> getConvertDataInfoList(){ return convertDataInfoList;}
private:

	static FolderManager * pInstance;

	QVector<ConvertDataInfo> convertDataInfoList;	//맵 데이터를 생성하기 위한 클래스.. 해당 클래스는 하나의 맵 ID를 생성하고 좌표를 관리 한다.
	QXmlStreamReader * reader;							//Xml 파일을 읽기 위한 포인터 객
	QFile * openedFile;									//opened file
	QString mlayer;											//메인 레이어 이름..
	QString nfilename;										//생성될 map 파일이름. 디렉토리 path 포함
	QString mapfilename;									//생성될 map 파일이름. 디렉토리 path는 제외
	QString cfilename;										//kml 파일 이름. 디렉토리 포함.
	int idNum;												//생성될 좌표의 ID 번호, notitle에서만 적용됨.
	int layerLevel;											//라디오 버튼에 의해 선택되는 레이어 레벨*/
	QStringList layerNames;
	Mode process_mode;
	bool firstFolderName;

};

#endif /* FOLDERMANAGER_H_ */
