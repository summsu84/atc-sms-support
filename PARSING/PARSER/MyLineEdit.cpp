/*
 * MyLineEdit.cpp
 *
 *  Created on: 2013. 4. 1.
 *      Author: jiwon
 */

#include "MyLineEdit.h"
#include <QDebug>
MyLineEdit::MyLineEdit(QWidget * parent)
: QLineEdit(parent)
{
	// TODO Auto-generated constructor stub

}

MyLineEdit::~MyLineEdit() {
	// TODO Auto-generated destructor stub
}

void MyLineEdit::focusInEvent(QFocusEvent * e)
{
	qDebug()<<"[MyLineEdit] focusInEvent is called..";
	QLineEdit::focusInEvent(e);
	emit(focussed(true));
}

void MyLineEdit::focusOutEvent(QFocusEvent * e)
{
	qDebug()<<"[MyLineEdit] focusOutEvent is called..";
	QLineEdit::focusOutEvent(e);
	emit(focussed(false));
}

void MyLineEdit::enterEvent(QEvent *e)
{
	qDebug()<<"[MyLineEdit] enterEvent is called..";
	//this->setToolTip("Here is my tooltip");
	//emit(isEnter());
	//this->setWhatsThis("This is parameter..");
}
