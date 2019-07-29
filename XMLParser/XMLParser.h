/*******************************************************************************
 * Class Name  : XMLParser
 * File Name   : XMLParser.h
 * Short Desc  : Safety Area 관련된 정보를 관리
 * Description : Safety Area 경고 메시지와 QNH, BA 와 같은 정보(xml) 를 데이터 베이스로 저장하거나, 데이터 베이스로 부터
 * XML 파일로 뽑아 내는 기능
 * Write date  : 2013.04.01
 * Author      : JJW
 *******************************************************************************/

#ifndef XMLPARSER_H
#define XMLPARSER_H


#include <QtGui>
#include "ui_XMLParser.h"
#include <QtSql>
#include <QtXml>
#include "Aoi.h"
#include "Qnh.h"
#include "Stca.h"
#include "Msaw.h"
#include "Ba.h"

class XMLParser : public QDialog,public Ui::XMLParserClass
{
    Q_OBJECT

public:
    XMLParser(QWidget *parent = 0);
    ~XMLParser();
    bool checkType(QString & type);
private slots:
	void openXmlFile();
	void quit();
	void close();
	void saveXml();
	bool deleteCreatedRecords();
	void generationXmlFile();

private:
   QStringList cfilename;
	int count;
	QSqlDatabase db;
	QSqlQuery *query;

	QString setDirectory();
	bool saveQnhXmlFile(const Qnh & qnhClass, const QString dirname);
	bool saveOutputXmlFile(const Aoi & aoiClass, const QString dirname);
	bool saveBaXmlFile(const Ba & aoiClass, const QString dirname);
	bool saveStcaXmlFile(const Stca & aoiClass, const QString dirname);
	bool saveMsawXmlFile(const Msaw & aoiClass, const QString dirname);


	bool openFile(QFile & file);
	bool dbOpen();
	bool qnhXmlProcess(QFile & file);
	bool outputXmlProcess(QFile & file);
	bool baXmlProcess(QFile & file);
	bool stcaXmlProcess(QFile & file);
	bool msawXmlProcess(QFile & file);


	bool checkDomDocument(QFile & file, QDomDocument & doc);
	bool checkDuplicationPK(const QString &name, const int area);

	bool insertMainRecord(const Qnh & qnhClass);
	bool insertMainRecord(const Aoi & aoiclass);
	bool insertMainRecord(const Ba & baclass);
	bool insertMainRecord(const Stca & stcaclass);
	bool insertMainRecord(const Msaw & msawClass);

	bool insertPolygonRecord(const Qnh &qnhClass);
	bool insertPolygonRecord(const Aoi &aoiClass);
	bool insertPolygonRecord(const Ba &baClass);
	bool insertPolygonRecord(const Stca &stcaClass);
	bool insertPolygonRecord(const Msaw &msawClass);

	bool insertSectorRecord(const Qnh &qnhClass);
	bool insertSectorRecord(const Aoi &aoiClass);
	bool insertSectorRecord(const Ba &baClass);
	bool insertSectorRecord(const Stca &stcaClass);
	bool insertSectorRecord(const Msaw &msawClass);

	bool  retrieverecords(QVector<QStringList> & records, QString & sql);

	bool readQnhTable();
	bool readOutputTable();
	bool readBaTable();
	bool readStcaTable();
	bool readMsawTable();
};

#endif // XMLPARSER_H

