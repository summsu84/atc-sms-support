/*
 * UserInfo.cpp
 *
 *  Created on: 2009. 10. 20.
 *      Author: root
 */
#include "UserInfo.h"
#include <QMessageBox>
UserInfo* UserInfo::pinstance = NULL;

UserInfo::UserInfo() {

	QSqlDatabase::removeDatabase("smsd");
	{
		QSqlDatabase db = QSqlDatabase::addDatabase("QOCI", "smsd");
		db.setHostName("localhost");
		db.setDatabaseName("smsd");
		db.setUserName("adapt");
		db.setPassword("adapt");
		//QMessageBox::warning(this, tr("DataBase Open Error"), tr("%1  %2  %3  %4 Driver %5").arg(parser->getDatabase()).arg(parser->getDbUser()).arg(parser->getDbPass()).arg(parser->getHost()).arg(parser->getDriver()));
		if (!db.open()) {
			//QMessageBox::critical(this, QObject::tr("DataBase Open Error"), QObject::tr("DataBase Open Error!"));
			//QDialog::reject();
			qDebug()<<"Database Open Error!";
		}
	}
	this->_databaseName = "smsd";

}

UserInfo::~UserInfo()
{
	if (pinstance != NULL) delete pinstance;
}
