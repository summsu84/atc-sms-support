/*
 * XMLImport.h
 *
 *  Created on: 2013. 12. 4.
 *      Author: jiwon
 */

#ifndef XMLIMPORT_H_
#define XMLIMPORT_H_
#include <QtGui>
#include <QMap>
#include <QGridLayout>
#include <QObject>
#include <QDomDocument>
#include "../../../lib/DbManager.h"
//#include "MyLineEdit.h"

/**
 *  XML Parsing 이후 SMSD에 Insert
 */

typedef struct _AD_INFO
{
	QString ad_id;
	QString icao_cd;
}AD_INFO;

class XMLImport {
public:
	XMLImport();
	virtual ~XMLImport();
	QStringList openFile();
	QStringList parsingElement(QFile & file);

private:
	QString fileName;
	QString errorMessage;
	DbManager * dbManager;
	QVector<AD_INFO> ad_list;
};

class XMLImportWidget : public QDialog
{
	Q_OBJECT

public :
	XMLImportWidget(QDialog *parent = 0);
	~XMLImportWidget();
	void init();
	void initLayout();
	void initBtn();
	void clear();
   QStringList openFile();
	QVBoxLayout * setCheckBox(QStringList listHeader);
	QGridLayout * initTableParam();
	QStringList parsingHeader(const QString header);
private:
	XMLImport * importer;
	QString fileName;
	int cnt;

	QLineEdit ** txtHeader;
	QCheckBox ** chkHeader;
	QLabel	** lblHeader;
	QLineEdit * txtTableName;
//	MyLineEdit * txtParam1;
//	MyLineEdit * txtParam2;
	QLabel * lblHelp;
	QPushButton * btnSelectAll;
	QPushButton * btnDeselectAll;
	QPushButton * btnStart;
	QPushButton * btnDb;
	QPushButton * btnClear;
	QMap<QString, int> headerList;			//맵 자료형의 HeaderList
	QStringList	headerNameList;				//헤더 이름 목록
	QStringList  paramList;

	int lineNo;
	QString errorMessage;

	QStackedLayout * stackedLayout;
};




#endif /* XMLIMPORT_H_ */
