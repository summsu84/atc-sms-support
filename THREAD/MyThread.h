/*
 * MyThread.h
 *
 *  Created on: 2013. 1. 22.
 *      Author: jiwon
 */

#ifndef MYTHREAD_H_
#define MYTHREAD_H_

#include <QThread>
#include <QMutex>
#include "../RightLayout.h"

class MyThread : public QThread
{
	Q_OBJECT
public:
	MyThread();
	virtual ~MyThread();

    void setMessage(const QString &message);
    void setRightLayout(RightLayout * layout){this->rightLayout = layout;}
    void run();
    void stop();

private:
    QString messageStr;
    volatile bool stopped;
    QMutex mutex;
    RightLayout * rightLayout;

signals:
	void emitMsg(QString msg);
};

#endif /* MYTHREAD_H_ */
