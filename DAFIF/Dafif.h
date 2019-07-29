/*
 * Dafif.h
 *
 *  Created on: 2012. 12. 17.
 *      Author: jiwon
 */

#ifndef DAFIF_H_
#define DAFIF_H_

#include <QFile>
#include <QDebug>
#include <QtSql>
#include <QTextStream>
#include <QString>
#include <QVector>
#include <QProgressDialog>
#include <QMessageBox>
#include "tables.h"
#include "SqlGen_Dafif.h"
#include "../lib/UserInfo.h"
#include <iostream>
#include <cstring>
//#include "../lib/UserInfo.h"

class Dafif {
public:
	Dafif();
	virtual ~Dafif();

	bool readFile(const QString& fileName, TableType _tableType);
	void parsing(const QString& line, const TableType tableType);
	QStringList parsingHeader(const QString header);
	int getHeader(const QString headername);
	void copy(char * dest, QString source);
	void copy(UsageType * dest, UsageType source);
	void PrtDafif();
	bool insertTables(TableType tableType);
	bool deleteAll(const QString tableName);
	QString refineSql(const QString& sql);
	QString errorMessage;
	int  jobCount;
private:
	QVector<TB_DA_CNTRYCD *> tb_da_cntrycd;
	QMap<QString, int> headerList;
	int lineNo;
	TableType tableType;
	SqlGen_Dafif sqlgen;

};

#endif /* DAFIF_H_ */
