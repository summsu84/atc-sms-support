/*
 * Aoi.h
 *
 *  Created on: 2012. 3. 5.
 *      Author: jiwon
 */

#ifndef AOI_H_
#define AOI_H_

#include "AoiPolygon.h"
#include "AoiSector.h"

class Aoi {
public:
	Aoi();
	QString mask;
	QString activeflag;
	QString name;
	int area;					//Area (Output : 2)
	QString type;
	QString cat;
	//Polygon
	AoiPolygon polygon;

	//Sector
	AoiSector sector;

	QString hmin;
	QString hmax;
};

#endif /* AOI_H_ */
