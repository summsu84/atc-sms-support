/*
 * Lat.h
 *
 *  Created on: 2012. 10. 22.
 *      Author: jiwon
 */

#ifndef LAT_H_
#define LAT_H_

#include <QString>
class Lat {
public:
	Lat();
	virtual ~Lat();
	void printValues();
	QString printMapValues();
	inline QString getDegree(){ return degree;}
	inline QString getMin(){ return min;}
	inline QString getSec(){ return sec;}
	inline QString getMilliSec() { return millisec;}

	inline void setDegree(QString _degree){ degree = _degree;}
	inline void setMin(QString _min){ min = _min;}
	inline void setSec(QString _sec){ sec = _sec;}
	inline void setMilliSec(QString _millisec) { millisec = _millisec;}
private:
	QString degree;
	QString min;
	QString sec;
	QString millisec;
};

#endif /* LAT_H_ */
