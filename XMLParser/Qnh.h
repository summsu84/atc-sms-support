/*
 * Qnh.h
 *
 *  Created on: 2012. 3. 2.
 *      Author: jiwon
 */

#ifndef QNH_H_
#define QNH_H_

#include "QnhPolygon.h"
#include "QnhSector.h"

class Qnh {
public:
	Qnh();

	QString icaocode;
	QString trlevel;
	QString activeflag;
	QString meteosystemcoordwgs;
	QString meteoLAT; //METO_LAT Record
	QString meteoLON; //METEO_LON Record
	QString meteoupdatepriod;
	// tempaerature, qnh..variable
	QString temperature;
	QString qnh;
	QString meteotimestamp;
	QString date;
	QString time;
	QString name;
	int area;								//AREA (QNH : 1)
	QString type; 	// P : sector,   S : polygon

	QnhPolygon polygon;
	QString hmin;
	QString hmax;

	QnhSector  sector;
};

#endif /* QNH_H_ */
