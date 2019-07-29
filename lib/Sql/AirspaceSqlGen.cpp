/*
 * AirspaceSqlGen.cpp
 *
 *  Created on: 2013. 1. 30.
 *      Author: jiwon
 */

#include "AirspaceSqlGen.h"

AirspaceSqlGen::AirspaceSqlGen(QString tb_name)
: SqlGen(tb_name)
{
	// TODO Auto-generated constructor stub

}

AirspaceSqlGen::~AirspaceSqlGen() {
	// TODO Auto-generated destructor stub
}



/**
 * TB_CLSMAP에 Insert
 */
QString AirspaceSqlGen::insertMainTableSql(ConvertDataInfo data)
{
	QString sql;

	QString areaId = getAreaId(data.getId());
	QString rng = getRng(data.getId());


	sql  = "INSERT INTO "+tb_name+" ( ";
	sql += " AREA_ID, RNG ) VALUES ('"+areaId+"', '"+rng+"')";

	return sql;
}

/**
 * TB_CLSMAPBNDRY에 Insert
 */
QString AirspaceSqlGen::insertSubTableSql(ConvertDataInfo data, int idx)
{
	QString sql;
	QString sub_table = "TB_CLSMAPBNDRY";

	Lat lat = data.getLatList().value(idx);
	Lon lon = data.getLonList().value(idx);

	QString bndry_idx;

	bndry_idx = QString().setNum((idx+1));
	bndry_idx.append('0');

	//bndry_idx 앞에 0 붙이기
	for(int i = bndry_idx.length() ; i < 4 ; i++){
		bndry_idx.prepend('0');
	}
	QString areaId = getAreaId(data.getId());
	QString rng = getRng(data.getId());


	sql  = "INSERT INTO "+ sub_table + " ( ";
	sql += "BNDRY_IDX, LAT, LON, AREA_ID, RNG ) VALUES ";
	sql += "( '"+bndry_idx+"', '"+lat.printMapValues()+"', '"+lon.printMapValues()+"', '"+areaId+"', '"+rng+"')";

	return sql;
}

/**
 * TB_CLSMAP에 레코드 존재 확인 sql
 */
QString AirspaceSqlGen::getCountMainTableSql(ConvertDataInfo data)
{
	QString sql;

	QString areaId = getAreaId(data.getId());
	QString rng = getRng(data.getId());


	sql  = " SELECT COUNT(*) FROM "+tb_name+" WHERE AREA_ID = '"+areaId+"' AND RNG = '"+rng+"'";

	return sql;
}

/**
 * TB_CLSMAPBNDRY에 레코드 존재 확인 sql
 */
QString AirspaceSqlGen::getCountSubTableSql(ConvertDataInfo data, int idx)
{

	QString sql;
	QString sub_table = "TB_COASTBNDRY";

	Lat lat = data.getLatList().value(idx);
	Lon lon = data.getLonList().value(idx);

	QString bndry_idx;

	bndry_idx = QString().setNum((idx+1));
	bndry_idx.append('0');

	//bndry_idx 앞에 0 붙이기
	for(int i = bndry_idx.length() ; i < 4 ; i++){
		bndry_idx.prepend('0');
	}

	sql  = " SELECT COUNT(*) FROM "+tb_name+" WHERE COAST_ID = '"+data.getId()+"' AND BNDRY_IDX = '"+bndry_idx+"'";

	return sql;
}

/**
 * Area ID를 얻어 온다..
 */

QString AirspaceSqlGen::getAreaId(QString id)
{
	QString areaId = id.split(" ").at(0);

	return areaId;
}


/**
 * RNG 를 얻어 온다..
 */

QString AirspaceSqlGen::getRng(QString id)
{
	QString rng = id.split(" ").at(1);

	return rng;
}
