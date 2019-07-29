/*
 * SidStarSqlGen.h
 *
 *  Created on: 2013. 1. 30.
 *      Author: jiwon
 */

#ifndef SIDSTARSQLGEN_H_
#define SIDSTARSQLGEN_H_

#include <QString>
#include "SqlGen.h"

class SidStarSqlGen : public SqlGen
{
public:
	SidStarSqlGen(QString tb_name);
	virtual ~SidStarSqlGen();
	virtual QString insertMainTableSql(ConvertDataInfo data);
	virtual QString insertSubTableSql(ConvertDataInfo data, int idx);
	virtual QString getCountMainTableSql(ConvertDataInfo data);
	virtual QString getCountSubTableSql(ConvertDataInfo data, int idx);
};

#endif /* SIDSTARSQLGEN_H_ */
