#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <QtGui>
#include <QtSql>
#include <QtXml>
#include "ui_xml_parser.h"
#include "Aoi.h"
#include "Qnh.h"
#include "Stca.h"
#include "Msaw.h"
#include "Ba.h"

class XML_Parser : public QWidget, public Ui::XML_ParserClass
{
    Q_OBJECT

public:
    XML_Parser(QWidget *parent = 0);
    ~XML_Parser();
    bool checkType(QString & type);

private slots:
	void openXmlFile();
	void saveXml();
	bool deleteCreatedRecords();
	void generationXmlFile();

	//void okXm

private:
    //Ui::XML_ParserClass ui;

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

#endif // XML_PARSER_H
