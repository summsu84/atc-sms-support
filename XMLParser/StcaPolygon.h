/*
 * StcaPolygon.h
 *
 *  Created on: 2012. 3. 6.
 *      Author: jiwon
 */

#ifndef STCAPOLYGON_H_
#define STCAPOLYGON_H_
#include "QnhPolygon.h"

class StcaPolygon {
public:
	StcaPolygon();

	QVector<LatLon> point;
};

#endif /* STCAPOLYGON_H_ */
