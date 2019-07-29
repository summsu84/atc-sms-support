/*
 * UserInfo.h
 *
 *  Created on: 2009. 10. 20.
 *      Author: root
 */

#ifndef USERINFO_H_
#define USERINFO_H_

#include <QtCore>
#include <QObject>
#include <QtSql>

class UserInfo
{
public:
	static UserInfo &getInstance()
	{
		if (pinstance == NULL) {
			pinstance = new UserInfo;
		}
		return *pinstance;
	}

	inline void setId(const QString &id) { _id = id; }
	inline const QString getId() { return _id; }
	inline void setPass(const QString &pass) { _pass = pass; }
	inline const QString getPass() { return _pass; }
	inline void setMode(const QString &mode) { _mode = mode; }
	inline const QString getMode() { return _mode; }
	inline void setAuth(const QString &auth) { _auth = auth; }
	inline const QString getAuth() { return _auth; }
	inline void setDbname(const QString &db) { _databaseName = db; }
	inline const QString getDbname() { return _databaseName; }
	UserInfo();
	~UserInfo();


private:
	static UserInfo *pinstance;
	QString _id;
	QString _pass;
	QString _mode;
	QString _auth;
	QString _databaseName;
};
#endif /* USERINFO_H_ */
