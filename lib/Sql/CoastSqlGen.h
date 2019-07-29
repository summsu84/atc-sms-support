/*
 * CoasSqlGen.h
 *
 *  Created on: 2013. 1. 30.
 *      Author: jiwon
 */

#ifndef COASTSQLGEN_H_
#define COASTSQLGEN_H_

#include "SqlGen.h"
#include <QString>

class CoastSqlGen : public SqlGen
{
public:
	CoastSqlGen(QString tb_name);
	virtual ~CoastSqlGen();
	virtual QString insertMainTableSql(ConvertDataInfo data);
	virtual QString insertSubTableSql(ConvertDataInfo data, int idx);
	virtual QString getCountMainTableSql(ConvertDataInfo data);
	virtual QString getCountSubTableSql(ConvertDataInfo data, int idx);
};

#endif /* COASSQLGEN_H_ */
