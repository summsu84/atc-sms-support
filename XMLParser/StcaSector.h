/*
 * StcaSector.h
 *
 *  Created on: 2012. 3. 6.
 *      Author: jiwon
 */

#ifndef STCASECTOR_H_
#define STCASECTOR_H_

#include "StcaPolygon.h"

class StcaSector {
public:
	StcaSector();

	QString az0;
	QString az1;
	QString rmin;
	QString rmax;
	QVector <LatLon> center;

};

#endif /* STCASECTOR_H_ */
