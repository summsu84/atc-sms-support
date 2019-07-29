/*
 * RightLayout.h
 *
 *  Created on: 2012. 11. 1.
 *      Author: jiwon
 */

#ifndef RIGHTLAYOUT_H_
#define RIGHTLAYOUT_H_
#include <QtGui>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "lib/FolderManager.h"
#include "lib/Function.h"

class RightLayout : public QPlainTextEdit{
	Q_OBJECT
public:


	RightLayout(QWidget * parent = 0);
	virtual ~RightLayout();
	void openFile();
	void genKmlFile();					//파일을 읽은 내용을 창에 띄운다.
	void setProcessMode(Mode _mode) {this->process_mode = _mode;}
	void test();
public slots:
	void processKmlFile(QString fileName);
	//void processMapFile(QString fileName);
private:
	QString cFileName;			//현재 파일 풀 네임..
	QFile * openedFile;
	Mode process_mode;
signals:
	void emitOpenKmlFile(QString msg);
};

#endif /* RIGHTLAYOUT_H_ */
