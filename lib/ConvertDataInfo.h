/*
 * ConvertDataInfo.h
 *
 *  Created on: 2012. 10. 22.
 *      Author: jiwon
 */

#ifndef CONVERTDATAINFO_H_
#define CONVERTDATAINFO_H_

#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "Lat.h"
#include "Lon.h"
#include "lib/Function.h"
#include "lib/StyleManager.h"

enum {
	POLYGON,
	POLYLINE
};



class ConvertDataInfo {
public:
	ConvertDataInfo();
	virtual ~ConvertDataInfo();
	void processConvertation();

	inline QVector<Lat> getLatList(){ return latList;}
	inline QVector<Lon> getLonList(){ return lonList;}
	inline QString getId(){ return id;}
	inline int getCnt(){return cnt;}
	inline QString getLayer(){return layer;}
	inline QString getStyleMapKey(){return styleMapId;}
	inline int getType(){return type;}
	inline LineType getLineType(){return lineType;}
	inline FillPattern getFillPattern(){return fillPattern;}
	inline Style getStyle(){ return style;}

	inline void setLatList(QVector<Lat> _latList){ latList = _latList; }
	inline void setLonList(QVector<Lon> _lonList){ lonList = _lonList; }
	inline void setId(QString _id){ id = _id;}
	inline void setCnt(int _cnt){cnt = _cnt;}
	inline void setLayer(QString _layer) {layer = _layer;}
	inline void setStyleMapKey(QString _styleMapId){styleMapId = _styleMapId;}
	inline void setType(int _type) {type = _type;}
	inline void setLineType(LineType _lineType){lineType = _lineType;}
	inline void setFillPattern(FillPattern _fillPattern) {fillPattern = _fillPattern;}
	inline void setStyle(Style _style) {style = _style;}

	void setIdLayer(QString layer, QString id);
	void setCoordinate(QString coordinates);
	QString getCoordinate(){ return coordinates;}
	void printMapFile();
	void makeMapFile(QTextStream &out);
	void makeMapFile();

	//style map key를 이용하여 normal 속성을 가져온다..
	void findLineColor();
	void findLineWidth();
	void findFillColor();
	void findStyle();

private :
	int cnt;									//좌표의 갯수들..
	int type;								//Polygon or LineString..
	QString id;								//
	QString layer;							//Layer
	QString coordinates;					//.kml 파일에서 읽어온 좌표 값..
	QString styleMapId;					//StyleMap을 찾기 위한 key 값..
	LineType lineType;						//LineType
	FillPattern fillPattern;				//FillPattern

	Style style;								//스타일에 관한 정보 관리..

	QVector<Lat>	latList;					//위도 목록
	QVector<Lon> lonList;					//경도 목록
};

#endif /* CONVERTDATAINFO_H_ */
