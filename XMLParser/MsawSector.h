/*
 * MsawSector.h
 *
 *  Created on: 2012. 3. 6.
 *      Author: jiwon
 */

#ifndef MSAWSECTOR_H_
#define MSAWSECTOR_H_

#include "MsawPolygon.h"
class MsawSector {
public:
	MsawSector();

	QString az0;
	QString az1;
	QString rmin;
	QString rmax;
	QVector <LatLon> center;

};

#endif /* MSAWSECTOR_H_ */
