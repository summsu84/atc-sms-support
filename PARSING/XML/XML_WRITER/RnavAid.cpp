#include "RnavAid.h"
#include <QDebug>
RnavAid::RnavAid()
{

}

RnavAid::~RnavAid()
{

}

void RnavAid::printRnav()
{
	qDebug()<<"Name : "<<name<<", StationName : "<<station_name<<", Type : "<<type<<", Freq : "<<frequency<<", Lat : "<<lat<<", Lon : "<<lon;
}
