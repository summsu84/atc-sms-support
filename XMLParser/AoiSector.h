/*
 * AoiSector.h
 *
 *  Created on: 2012. 3. 5.
 *      Author: jiwon
 */

#ifndef AOISECTOR_H_
#define AOISECTOR_H_

#include "AoiPolygon.h"

class AoiSector {
public:
	AoiSector();

	QString az0;
	QString az1;
	QString rmin;
	QString rmax;
	QVector<LatLon> center;
};

#endif /* AOISECTOR_H_ */
