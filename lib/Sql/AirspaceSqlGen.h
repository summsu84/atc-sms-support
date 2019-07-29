/*
 * AirspaceSqlGen.h
 *
 *  Created on: 2013. 1. 30.
 *      Author: jiwon
 */

#ifndef AIRSPACESQLGEN_H_
#define AIRSPACESQLGEN_H_

#include <QString>
#include "SqlGen.h"

class AirspaceSqlGen : public SqlGen
{
public:
	AirspaceSqlGen(QString tb_name);
	virtual ~AirspaceSqlGen();
	virtual QString insertMainTableSql(ConvertDataInfo data);
	virtual QString insertSubTableSql(ConvertDataInfo data, int idx);
	virtual QString getCountMainTableSql(ConvertDataInfo data);
	virtual QString getCountSubTableSql(ConvertDataInfo data, int idx);

	QString getAreaId(QString id);
	QString getRng(QString id);
};

#endif /* AIRSPACESQLGEN_H_ */
