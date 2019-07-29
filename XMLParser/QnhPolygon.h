/*
 * QnhPolygon.h
 *
 *  Created on: 2012. 3. 2.
 *      Author: jiwon
 */

#ifndef QNHPOLYGON_H_
#define QNHPOLYGON_H_

#include <QString>
#include <QVector>

struct LatLon_T {
	QString lat;
	QString lon;
	QString alt;
} ;

typedef struct LatLon_T LatLon;

class QnhPolygon {
public:
	QnhPolygon();

	QVector<LatLon> pointList;
};

#endif /* QNHPOLYGON_H_ */
