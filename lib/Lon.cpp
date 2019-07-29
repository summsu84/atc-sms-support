/*
 * Lon.cpp
 *
 *  Created on: 2012. 10. 22.
 *      Author: jiwon
 */

#include "Lon.h"
#include <QDebug>
Lon::Lon() {
	// TODO Auto-generated constructor stub
	degree = "";
	min = "";
	sec = "";
	millisec = "";
}

Lon::~Lon() {
	// TODO Auto-generated destructor stub
}

void Lon::printValues()
{
	qDebug()<<"degree : "<<degree<<", min :"<<min<<", sec : "<<sec<<", millsec :"<<millisec;
}

QString Lon::printMapValues()
{
	QString mlon = "E";			//맵 표기 형식의 경도로 리턴함.
	mlon.append(degree);
	//mlon.append('.');
	mlon.append(min);
	mlon.append(sec);
	mlon.append(millisec);

/*	while(mlon.size() == 9){
		mlon.append("0");
	}*/

	if(mlon.size() != 10){
		qDebug()<<"Incorrect Lon Value : "<<mlon<<" Size : "<<mlon.size();
		while(mlon.size() < 10){
			//qDebug()<<"Lon Size : "<<mlon.size();
			mlon.append("0");
		}
}

	return mlon;
}
