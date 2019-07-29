/*
 * tables.h
 *
 *  Created on: 2012. 12. 17.
 *      Author: jiwon
 */

#ifndef TABLES_H_
#define TABLES_H_

const int SIZE_OF_NUMBER = 20;

enum UsageType {ALL_RECORDS = 100,
					NO_ALL_RECORDS = 101};

enum TableType {DA_APCC_ICAO
};

typedef struct tagTB_DA_CNTRYCD{
    unsigned int  LineNo;
    char COUNTRY[2+1];   //CHAR,2, PilSu
    char COUNTRY_NAME[30+1];   //CHAR,1, PilSu
    char ICAO_RGN[2+1];   //NUMBER,Null OK
    UsageType usageType;
} TB_DA_CNTRYCD;

#endif /* TABLES_H_ */
