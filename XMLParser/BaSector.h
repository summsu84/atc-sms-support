/*
 * BaSector.h
 *
 *  Created on: 2012. 3. 6.
 *      Author: jiwon
 */

#ifndef BASECTOR_H_
#define BASECTOR_H_

#include "BaPolygon.h"

class BaSector {
public:
	BaSector();

	QString az0;
	QString az1;
	QString rmin;
	QString rmax;
	QVector <LatLon> center;


};

#endif /* BASECTOR_H_ */
