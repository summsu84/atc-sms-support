/*
 * ConvertDataInfo.cpp
 *
 *  Created on: 2012. 10. 22.
 *      Author: jiwon
 */

#include "ConvertDataInfo.h"
#include <QDebug>

ConvertDataInfo::ConvertDataInfo() {
	// TODO Auto-generated constructor stub
	/*id = 0;
	cnt = 0;
*/
}

ConvertDataInfo::~ConvertDataInfo() {
	// TODO Auto-generated destructor stub
}

void ConvertDataInfo::processConvertation()
{

}

void ConvertDataInfo::printMapFile()
{
	//qDebug()<<"PrintMapFile is called..";
	QString idLine = "ID:";
	//QString strId = QString().setNum(id, 10);
	QString strCnt = QString().setNum(cnt, 10);
	idLine.append(strCnt);
	idLine.append(':');
	idLine.append(strCnt);
	idLine.append('P');
	qDebug()<<idLine;
	for(int i = 0 ; i < latList.count(); i++){
		QString latlon = "G:";
		Lat lat = (Lat)latList.value(i);
		Lon lon = (Lon)lonList.value(i);
		latlon.append(lat.printMapValues());
		latlon.append(lon.printMapValues());
		//qDebug()<<"G:"<<lat.printMapValues()<<lon.printMapValues();
		qDebug()<<latlon;
	}
}
void ConvertDataInfo::makeMapFile(QTextStream &out)
{
	//qDebug()<<"PrintMapFile is called..";
	//Layer 생성

	QString idLine = "ID:";
	QString strId = this->id;
	QString strCnt = QString().setNum(lonList.count(), 10);
	idLine.append(strId);
	idLine.append(':');
	idLine.append(strCnt);
	idLine.append(':');
	if(this->type == POLYGON){
		idLine.append('G');
	}else if(this->type == POLYLINE){
		idLine.append('P');
	}
	out<<idLine<<endl;

	for(int i = 0 ; i < latList.count(); i++){
		QString latlon = "G:";
		Lat lat = (Lat)latList.value(i);
		Lon lon = (Lon)lonList.value(i);
		latlon.append(lat.printMapValues());
		latlon.append(lon.printMapValues());
		//qDebug()<<"G:"<<lat.printMapValues()<<lon.printMapValues();
		out<<latlon<<endl;
	}
}

void ConvertDataInfo::setIdLayer(QString layer, QString id)
{
	this->id =id;
	this->layer = layer;
}

void ConvertDataInfo::setCoordinate(QString coordinates)
{
	this->coordinates = coordinates;
}

void ConvertDataInfo::findFillColor()
{
	//StyleManager 클래스를 이용하여 키값으로 가져온다..
	//Style style = StyleManager::getInstance().findNormalStyle4Style(this->styleMapId);

}

void ConvertDataInfo::findLineColor()
{

}

void ConvertDataInfo::findLineWidth()
{

}

void ConvertDataInfo::findStyle()
{
	//StyleManager 클래스를 이용하여 키값으로 가져온다..
	style = StyleManager::getInstance().findNormalStyle4Style(this->styleMapId);
}


//Debug
void ConvertDataInfo::makeMapFile()
{
	//qDebug()<<"PrintMapFile is called..";
	//Layer 생성

	QString idLine = "ID:";
	QString strId = this->id;
	QString strCnt = QString().setNum(lonList.count(), 10);
	idLine.append(strId);
	idLine.append(':');
	idLine.append(strCnt);
	idLine.append(':');
	if(this->type == POLYGON){
		idLine.append('G');
	}else if(this->type == POLYLINE){
		idLine.append('P');
	}
	qDebug()<<idLine;

	for(int i = 0 ; i < latList.count(); i++){
		QString latlon = "G:";
		Lat lat = (Lat)latList.value(i);
		Lon lon = (Lon)lonList.value(i);
		latlon.append(lat.printMapValues());
		latlon.append(lon.printMapValues());
		//qDebug()<<"G:"<<lat.printMapValues()<<lon.printMapValues();
		qDebug()<<latlon;
	}
}
