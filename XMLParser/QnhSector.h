/*
 * QnhSector.h
 *
 *  Created on: 2012. 3. 2.
 *      Author: jiwon
 */

#ifndef QNHSECTOR_H_
#define QNHSECTOR_H_

#include <QString>
#include <QVector>
#include "QnhPolygon.h"


typedef struct LatLon_T LatLon;

class QnhSector {
public:
	QnhSector();
	QString az0;
	QString az1;
	QString rmin;
	QString rmax;
	QVector <LatLon> center;
};

#endif /* QNHSECTOR_H_ */
