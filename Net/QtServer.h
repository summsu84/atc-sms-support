/*
 * QtServer.h
 *
 *  Created on: 2013. 1. 24.
 *      Author: jiwon
 */

#ifndef QTSERVER_H_
#define QTSERVER_H_
#include <QtGui/QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QVector>
#include <QDataStream>

class QtServer: public QTcpServer
{
	Q_OBJECT

public:
	QtServer(QObject *parent = 0);
	virtual ~QtServer();

private slots:
	void readyRead();
	void disconnected();
	void sendUserList();

protected:
	void incomingConnection(int socketfd);

private:
	QSet<QTcpSocket*> clients;
	QMap<QTcpSocket*,QString> users;
	quint16 nextBlockSize;
};

#endif /* QTSERVER_H_ */
