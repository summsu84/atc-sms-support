/*
 * AoiPolygon.h
 *
 *  Created on: 2012. 3. 5.
 *      Author: jiwon
 */

#ifndef AOIPOLYGON_H_
#define AOIPOLYGON_H_
#include <QString>
#include <QVector>
#include "QnhPolygon.h"

/*
struct LatLon_T {
	QString lat;
	QString lon;
} ;

typedef struct LatLon_T LatLon;
*/


class AoiPolygon {
public:
	AoiPolygon();

	QVector<LatLon> point;
};

#endif /* AOIPOLYGON_H_ */
