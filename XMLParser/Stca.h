/*
 * Stca.h
 *
 *  Created on: 2012. 3. 6.
 *      Author: jiwon
 */

#ifndef STCA_H_
#define STCA_H_

#include "StcaPolygon.h"
#include "StcaSector.h"

class Stca {
public:
	Stca();

	QString type;			// TP (P : POLYGON, S : SECTOR)
	QString activeflag;	// ACTIVE
	QString name;			// SAFETY_NM
	QString stcatype;		//CAT_TP
	QString cat;			//CAT	(STCA : S)
	QString hmin;			//MIN_ALT
	QString hmax; 			//MAX_ALT
	QString active_tp;	//ACTIVE-TP		undesignated
	QString strt_tm;		//STRT_TM		undesignated
	QString end_tm;		//END_TM			undesignated
	QString day;			//DAY				undesignated
	int area;				//Area (STCA Area : 4)

	//Polygon
	StcaPolygon polygon;
	//Sector
	StcaSector sector;
};

#endif /* STCA_H_ */
