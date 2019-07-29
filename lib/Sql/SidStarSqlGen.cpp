/*
 * SidStarSqlGen.cpp
 *
 *  Created on: 2013. 1. 30.
 *      Author: jiwon
 */

#include "SidStarSqlGen.h"

SidStarSqlGen::SidStarSqlGen(QString tb_name)
 : SqlGen(tb_name)
{
	// TODO Auto-generated constructor stub

}

SidStarSqlGen::~SidStarSqlGen() {
	// TODO Auto-generated destructor stub
}


QString SidStarSqlGen::insertMainTableSql(ConvertDataInfo data)
{
	QString sql;
/*

	sql  = "INSERT INTO "+tb_name+" ( ";
	sql += " COAST_ID ) VALUES ('"+data.getId()+"')";
*/

	return sql;
}

QString SidStarSqlGen::insertSubTableSql(ConvertDataInfo data, int idx)
{

	QString sql;
	QString sub_table = "TB_SIDSTARMAP";

	Lat lat = data.getLatList().value(idx);
	Lon lon = data.getLonList().value(idx);

	//Bndry Idx 구하기
	QString bndry_idx;

	bndry_idx = QString().setNum((idx+1));
	bndry_idx.append('0');

	//bndry_idx 앞에 0 붙이기
	for(int i = bndry_idx.length() ; i < 3 ; i++){
		bndry_idx.prepend('0');
	}
	//SIDSTARAPCH 구하기

	QString sidstarapch = data.getLayer();
	if(sidstarapch == "SID")
	{
		sidstarapch = "S";
	}else if(sidstarapch == "STAR")
	{
		sidstarapch = "T";
	}


	sql  = "INSERT INTO "+ sub_table + " ( ";
	sql += "IDX, LAT, LON, SIDSTAR_KEY, BNDRY_VIA, SIDSTARAPCH ) VALUES ";
	sql += "( '"+bndry_idx+"', '"+lat.printMapValues()+"', '"+lon.printMapValues()+"', '"+data.getId()+"', ";
	sql += "'G', '"+sidstarapch+"')";


	return sql;

}

QString SidStarSqlGen::getCountMainTableSql(ConvertDataInfo data)
{
	QString sql;

//	sql  = " SELECT COUNT(*) FROM "+tb_name+" WHERE COAST_ID = '"+data.getId()+"'";

	return sql;
}

QString SidStarSqlGen::getCountSubTableSql(ConvertDataInfo data, int idx)
{

	QString sql;
	QString sub_table = "TB_SIDSTARMAP";

	Lat lat = data.getLatList().value(idx);
	Lon lon = data.getLonList().value(idx);

	QString bndry_idx;

	bndry_idx = QString().setNum((idx+1));
	bndry_idx.append('0');

	//bndry_idx 앞에 0 붙이기
	for(int i = bndry_idx.length() ; i < 3 ; i++){
		bndry_idx.prepend('0');
	}

	sql  = " SELECT COUNT(*) FROM "+sub_table+" WHERE SIDSTAR_KEY = '"+data.getId()+"' AND IDX = '"+bndry_idx+"'";

	return sql;
}
