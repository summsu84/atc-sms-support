/*
 * SqlGen_Dafif.h
 *
 *  Created on: 2012. 12. 17.
 *      Author: jiwon
 */

#ifndef SQLGEN_DAFIF_H_
#define SQLGEN_DAFIF_H_

#include <QString>
#include <QStringList>
#include "tables.h"

class SqlGen_Dafif {
public:
	SqlGen_Dafif();
	virtual ~SqlGen_Dafif();
   QString insertSql(const TB_DA_CNTRYCD * cntryCd) ;
   QString deleteSql(const TB_DA_CNTRYCD * cntryCd);
   QString updateSql(const TB_DA_CNTRYCD * cntryCd) ;
   QString getCount(const TB_DA_CNTRYCD * cntryCd) ;
   QString sqlStr(const char* src, const bool trim=true);

private :

};

#endif /* SQLGEN_DAFIF_H_ */
