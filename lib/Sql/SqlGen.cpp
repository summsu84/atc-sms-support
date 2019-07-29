/*
 * SqlGen.cpp
 *
 *  Created on: 2013. 1. 30.
 *      Author: jiwon
 */

#include "SqlGen.h"

using namespace std;

SqlGen::SqlGen(QString tb_name) {
	// TODO Auto-generated constructor stub
	this->tb_name = tb_name;
}

SqlGen::~SqlGen() {
	// TODO Auto-generated destructor stub
}


QString SqlGen::insertMainTableSql(ConvertDataInfo data)
{

	return "";
}

QString SqlGen::insertSubTableSql(ConvertDataInfo data, int idx)
{
/*	QString sql;
	QString sub_table = "TB_COASTBNDRY";

	QVector<Lat> lat_list = data.getLatList();
	QVector<Lon>

	sql  = "INSERT INTO "+ sub_table + " ( ";
	sql += "BNDRY_IDX, LAT, LON, COAST_ID ) VALUES (";
	sql +=*/
}
