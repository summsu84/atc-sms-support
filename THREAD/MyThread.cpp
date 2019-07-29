/*
 * MyThread.cpp
 *
 *  Created on: 2013. 1. 22.
 *      Author: jiwon
 */

#include "MyThread.h"
#include <QDebug>
#include <iostream>

using namespace std;


MyThread::MyThread()
{
    stopped = false;
}

MyThread::~MyThread() {
	// TODO Auto-generated destructor stub
}

void MyThread::setMessage(const QString &message)
{
    mutex.lock();
    messageStr = message;
    mutex.unlock();
}

void MyThread::run()
{
	int i = 0;
	QString msg;
    while (!stopped){
    	msg = QString().setNum(i);
    	qDebug()<<"msg : "<<msg;
    	i++;
    	//rightLayout->setPlainText(msg);
    	emitMsg(msg);
    	sleep(1);
    }

    stopped = false;
    cerr << endl;
}

void MyThread::stop()
{
    mutex.lock();
    stopped = true;
    mutex.unlock();
}
