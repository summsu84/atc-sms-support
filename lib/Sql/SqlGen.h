/*
 * SqlGen.h
 *
 *  Created on: 2013. 1. 30.
 *      Author: jiwon
 */

#ifndef SQLGEN_H_
#define SQLGEN_H_

#include <iostream>
#include "../ConvertDataInfo.h"

class SqlGen {
public:
	SqlGen(QString tb_name);
	virtual ~SqlGen();
	virtual QString insertMainTableSql(ConvertDataInfo data);
	virtual QString insertSubTableSql(ConvertDataInfo data , int idx);
	virtual QString getCountMainTableSql(ConvertDataInfo data) = 0;
	virtual QString getCountSubTableSql(ConvertDataInfo data, int idx) = 0;

protected:
	QString tb_name;			//테이블 이름
};

#endif /* SQLGEN_H_ */
