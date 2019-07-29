/*
 * QtServer.cpp
 *
 *  Created on: 2013. 1. 24.
 *      Author: jiwon
 */

#include "QtServer.h"

QtServer::QtServer(QObject *parent)
    : QTcpServer(parent)
{
	nextBlockSize = 0;
	qDebug()<<"QtServer is running..";
}

QtServer::~QtServer()
{
	close();
}


void QtServer::incomingConnection(int socketfd)
{
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketfd);
    clients.insert(client);

    qDebug() << "New client from:" << client->peerAddress().toString();

    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void QtServer::readyRead()
{
	qDebug()<<"Server receive message...";
	QTcpSocket *client = (QTcpSocket*)sender();

	QDataStream in(client);

    if(nextBlockSize == 0){
        if(client->bytesAvailable() < sizeof(quint16))
            ;
        else
            in>>nextBlockSize;
    }
    else if(client->bytesAvailable() < nextBlockSize)
        return;

    QString strRead;
    in>>strRead;

    nextBlockSize = 0;

    strRead.append(" by Server");

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out<<quint16(0);
    out<<strRead;
    out.device()->seek(0);
    out<<quint16(block.size() - sizeof(quint16));
    foreach(QTcpSocket *otherClient, clients)
		otherClient->write(block);
    //client->write(block);



	/*while(client->canReadLine())
    {
		QDataStream in(client);
    	if(nextBlockSize == 0){
    		if(client->bytesAvailable() < sizeof(quint16))
    			;
			else
				in>>nextBlockSize;
		}
		//else if(client->bytesAvailable() < nextBlockSize)
			//return;


    	QString tmp;
    	in>>tmp;

    	//QString line = QString::fromUtf8(client->readLine()).trimmed();
    	QString line = QString::fromUtf8(client->readLine()).trimmed();
    	qDebug() << "Read line:" << line<<", in : "<<tmp;



        QRegExp meRegex("^/me:(.*)$");

        if(meRegex.indexIn(line) != -1)
        {
            QString user = meRegex.cap(1);
            users[client] = user;
            foreach(QTcpSocket *client, clients)
                client->write(QString("Server:" + user + " has joined.\n").toUtf8());
            sendUserList();
        }
        else if(users.contains(client))
        {
            QString message = line;
            QString user = users[client];
            qDebug() << "User:" << user;
            qDebug() << "Message:" << message;

            foreach(QTcpSocket *otherClient, clients)
                otherClient->write(QString(user + ":" + message + "\n").toUtf8());
        }
        else
        {
            qWarning() << "Got bad message from client:" << client->peerAddress().toString() << line;
        }
    }*/
}

void QtServer::disconnected()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    qDebug() << "Client disconnected:" << client->peerAddress().toString();

    clients.remove(client);

    QString user = users[client];
    users.remove(client);

    sendUserList();
    foreach(QTcpSocket *client, clients)
        client->write(QString("Server:" + user + " has left.\n").toUtf8());
}

void QtServer::sendUserList()
{
    QStringList userList;
    foreach(QString user, users.values())
        userList << user;

    foreach(QTcpSocket *client, clients)
        client->write(QString("/users:" + userList.join(",") + "\n").toUtf8());
}

