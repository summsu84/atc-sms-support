/*
 * Ba.h
 *
 *  Created on: 2012. 3. 6.
 *      Author: jiwon
 */

#ifndef BA_H_
#define BA_H_

#include "BaPolygon.h"
#include "BaSector.h"

class Ba {
public:
	Ba();

	QString name;			// BA_NM
	QString type;			// P : Polygon, S : Sector
	QString mask;			// MASK
	QString hmin;			//MIN_ALT
	QString hmax;			//MAX_ALT
	QString activeflag;	//ACTIVE
	int area;				//AREA (BA : 3)

	//Polygon
	BaPolygon polygon;	//Polygon
	//Setor
	BaSector sector;		//Sector


};

#endif /* BA_H_ */
