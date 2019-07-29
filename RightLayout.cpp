/*
 * RightLayout.cpp
 *
 *  Created on: 2012. 11. 1.
 *      Author: jiwon
 */

#include "RightLayout.h"

RightLayout::RightLayout(QWidget * parent) : QPlainTextEdit(parent){
	// TODO Auto-generated constructor stub
	//connect(FolderManager::getInstance(), SIGNAL(emitSuccess(QString )), this, SLOT(processKmlFile(QString )));

	process_mode = FILE_MODE;
}

RightLayout::~RightLayout() {
	// TODO Auto-generated destructor stub
}


void RightLayout::processKmlFile(QString fileName)
{
	this->setPlainText("");
	/*if(process_mode == FILE_MODE){
		cFileName = fileName;
	}else if(process_mode == DB_MODE){
		cFileName = fileName + ".tmp";
	}*/
	cFileName = fileName;

	qDebug()<<"[RightLayout] : filename : "<<cFileName;
	int count = 0;
	openFile();
	QTextStream out(openedFile);
	QString strTmp;
	while(!out.atEnd()){
		strTmp.append(out.readLine());
		strTmp.append('\n');
		count++;
	}
	this->setPlainText(strTmp);
	QString msg = QString().setNum(count, 10);
	emit emitOpenKmlFile(msg);

}


void RightLayout::openFile()
{
	openedFile = new QFile(cFileName);
	if(!openedFile->open(QIODevice::ReadWrite | QIODevice::Text)){
		qDebug()<<"[RightLayout] Cannot open the file";
		return;
	}
}

void RightLayout::test()
{
	int i = 0;
	while(1)
	{
		qDebug()<<i;
		this->setPlainText(QString().setNum(i));
		i++;
	}
}
