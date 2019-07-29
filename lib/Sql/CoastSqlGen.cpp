/*
 * CoasSqlGen.cpp
 *
 *  Created on: 2013. 1. 30.
 *      Author: jiwon
 */

#include "CoastSqlGen.h"

CoastSqlGen::CoastSqlGen(QString tb_name)
: SqlGen(tb_name)
{

	// TODO Auto-generated constructor stub

}

CoastSqlGen::~CoastSqlGen() {
	// TODO Auto-generated destructor stub
}


QString CoastSqlGen::insertMainTableSql(ConvertDataInfo data)
{
	QString sql;

	sql  = "INSERT INTO "+tb_name+" ( ";
	sql += " COAST_ID ) VALUES ('"+data.getId()+"')";

	return sql;
}

/**
 * SIDSTARMAP은 테이블을 하나만 작업하기 때문에 아래 subTableSql만을 이용한다
 */
QString CoastSqlGen::insertSubTableSql(ConvertDataInfo data, int idx)
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

	sql  = "INSERT INTO "+ sub_table + " ( ";
	sql += "BNDRY_IDX, LAT, LON, COAST_ID ) VALUES ";
	sql += "( '"+bndry_idx+"', '"+lat.printMapValues()+"', '"+lon.printMapValues()+"', '"+data.getId()+"')";

	return sql;
}

QString CoastSqlGen::getCountMainTableSql(ConvertDataInfo data)
{
	QString sql;

	sql  = " SELECT COUNT(*) FROM "+tb_name+" WHERE COAST_ID = '"+data.getId()+"'";

	return sql;
}

QString CoastSqlGen::getCountSubTableSql(ConvertDataInfo data, int idx)
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

	sql  = " SELECT COUNT(*) FROM "+sub_table+" WHERE COAST_ID = '"+data.getId()+"' AND BNDRY_IDX = '"+bndry_idx+"'";

	return sql;
}
