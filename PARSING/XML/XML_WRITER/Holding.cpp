#include "Holding.h"
#include <QDebug>

Holding::Holding()
{

}

Holding::~Holding()
{

}

void Holding::printHolding()
{
	qDebug()<<"FixName : "<<holding_fix_name<<", DupId : "<<dup_id<<", InTrack :"<<inbound_track<<", Direction : "<<direction<<", MaxIas : "<<max_ias<<", MinAlt : "<<min_alt<<", maxAlt : "<<max_alt<<", CPTYPE : "<<cp_type<<", CPValue : "<<cp_value<<", Type : "<<type;
}
