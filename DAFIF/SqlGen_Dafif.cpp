/*
 * SqlGen_Dafif.cpp
 *
 *  Created on: 2012. 12. 17.
 *      Author: jiwon
 */

#include "SqlGen_Dafif.h"

SqlGen_Dafif::SqlGen_Dafif() {
	// TODO Auto-generated constructor stub

}

SqlGen_Dafif::~SqlGen_Dafif() {
	// TODO Auto-generated destructor stub
}


// char를 oracle sql에 사용하기 위해서 변환
QString SqlGen_Dafif::sqlStr(const char* src,const bool trim)
{
	QString s;
	bool allSpace = true;

	QString ss = QString(src);
	for(int i=0;i<ss.length();i++){
		if(ss[i] != QChar(' ')){
			allSpace = false; break;
		}
	}
	if(allSpace && trim){
		s = QString("null");
	}else{
		if(trim){
			s = "'"+QString(src).trimmed().replace("'", "''") + "'";
		}else{
			s = "'"+QString(src).replace("'", "''") + "'";
		}
	}
	return s;
}

QString SqlGen_Dafif::insertSql(const TB_DA_CNTRYCD * cntryCd)
{
	QString sql;
	sql =  "insert into TB_DA_CNTRYCD (" ;
	sql += "CNTRY_CD, CNTRY_NM, ICAO_CD ";
	sql += ")values(";
	sql += sqlStr(cntryCd->COUNTRY) + ",";
	sql += sqlStr(cntryCd->COUNTRY_NAME) + ",";
	sql += sqlStr(cntryCd->ICAO_RGN) +" ";
	sql += " );";

	return sql;

}
QString SqlGen_Dafif::deleteSql(const TB_DA_CNTRYCD * cntryCd)
{
	QString sql;
	sql  = "delete from TB_DA_CNTRYCD ;";
	sql += "where ICAO_CD = "+sqlStr(cntryCd->ICAO_RGN)+", ";
	sql += " CNTRY_CD = "+sqlStr(cntryCd->COUNTRY)+" ;";

	return sql;
}
QString SqlGen_Dafif::updateSql(const TB_DA_CNTRYCD * cntryCd)
{
	QString sql;


	sql =  "update TB_DA_CNTRYCD  set ";
	sql += " CNTRY_NM = " + sqlStr(cntryCd->COUNTRY_NAME) +"";
	sql += " where ";
	sql += " CNTRY_CD = " + sqlStr(cntryCd->COUNTRY) +" and ";
	sql += " ICAO_CD = " + sqlStr(cntryCd->ICAO_RGN) + ";";

	return sql;
}
QString SqlGen_Dafif::getCount(const TB_DA_CNTRYCD * cntryCd)
{

	QString sql;
	sql =  "select count(*) as count from TB_DA_CNTRYCD where ";
	sql += " CNTRY_CD = " + sqlStr(cntryCd->COUNTRY) + "and";
	sql += " ICAO_CD = " + sqlStr(cntryCd->ICAO_RGN) + ";";

	return sql;
}
