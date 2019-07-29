/*
 * BaPolygon.h
 *
 *  Created on: 2012. 3. 6.
 *      Author: jiwon
 */

#ifndef BAPOLYGON_H_
#define BAPOLYGON_H_

#include "QnhPolygon.h"
class BaPolygon {
public:
	BaPolygon();
	QVector<LatLon> point;
};

#endif /* BAPOLYGON_H_ */
