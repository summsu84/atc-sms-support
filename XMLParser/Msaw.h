/*
 * Msaw.h
 *
 *  Created on: 2012. 3. 6.
 *      Author: jiwon
 */

#ifndef MSAW_H_
#define MSAW_H_

#include "MsawPolygon.h"
#include "MsawSector.h"

class Msaw {
public:
	Msaw();

	QString type;			// TP (P : POLYGON, S : SECTOR)
	QString activeflag;	// ACTIVE
	QString name;			// SAFETY_NM
	QString msawtype;		//CAT_TP
	QString cat;			//CAT	(MSAW : M)
	QString hmin;			//MIN_ALT
	QString hmax; 			//MAX_ALT
	QString active_tp;	//ACTIVE_TP		undesignated
	QString strt_tm;		//STRT_TM		undesignated
	QString end_tm;		//END_TM			undesignated
	QString day;			//DAY				undesignated
	int area;				//Area (MSAW Area : 5)

	//Polygon
	MsawPolygon polygon;
	//Sector
	MsawSector sector;

};

#endif /* MSAW_H_ */
