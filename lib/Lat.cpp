/*
 * Lat.cpp
 *
 *  Created on: 2012. 10. 22.
 *      Author: jiwon
 */

#include "Lat.h"
#include <QDebug>
Lat::Lat() {
	// TODO Auto-generated constructor stub
	degree = "";
	min = "";
	sec = "";
	millisec = "";
}

Lat::~Lat() {
	// TODO Auto-generated destructor stub
}
void Lat::printValues()
{
	qDebug()<<"degree : "<<degree<<", min :"<<min<<", sec : "<<sec<<", millsec :"<<millisec;
}

QString Lat::printMapValues()
{
	QString mlat = "N";			//맵 표기 형식의 경도로 리턴함.
	mlat.append(degree);
	//mlat.append('.');
	mlat.append(min);
	mlat.append(sec);
	mlat.append(millisec);



	if(mlat.size() != 9){
		qDebug()<<"Incorrect Lat Value : "<<mlat<<" Size : "<<mlat.size();
		while(mlat.size() < 9){
			//qDebug()<<"Size : "<<mlat.size();
			mlat.append("0");
		}
	}




	return mlat;
}
