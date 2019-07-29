/*******************************************************************************
 * Class Name  : DAFIF_PARSER
 * File Name   : DAFIF_PARSER.h
 * Short Desc  : DAFIF 정보중 FIR 관련된 정보를 파
 * Description : DAFIF 정보 중에 FIR과 관련된 정보를 파싱한다.
 * Write date  : 2013.04.01
 * Author      : JJW
 *******************************************************************************/
#ifndef DAFIF_PARSER_H
#define DAFIF_PARSER_H

#include <QtGui>
#include <QMap>
#include <QGridLayout>
#include "MyLineEdit.h"


class DAFIF_PARSER : public QDialog
{
    Q_OBJECT

public:
    DAFIF_PARSER(QDialog *parent = 0);
    ~DAFIF_PARSER();
    void parsing(const QString& line);
    void init();
    void initLayout();
    void initBtn();
    QStringList parsingHeader(const QString header);
    int getHeader(const QString headername);
    QStringList openFile();
    QVBoxLayout * setCheckBox(QStringList listHeader);
    QGridLayout * initTableParam();
    void clear();
    QStringList chckHeaderList();
    QMap<QString, QString> parsingParam2(QStringList param2);
public slots:
	void runParsing();
	void runSelectAll();
	void runDeSelectAll();
	void runClear();
	void setHelpLabel(bool hasFocus);

protected :
	void resizeEvent(QResizeEvent * e);

private:
	QString fileName;
	int cnt;

	QLineEdit ** txtHeader;
	QCheckBox ** chkHeader;
	QLabel	** lblHeader;
	QLineEdit * txtTableName;
	MyLineEdit * txtParam1;
	MyLineEdit * txtParam2;
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

#endif // DAFIF_PARSER_H
