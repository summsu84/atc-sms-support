/*
 * MyLineEdit.h
 *
 *  Created on: 2013. 4. 1.
 *      Author: jiwon
 */

#ifndef MYLINEEDIT_H_
#define MYLINEEDIT_H_

#include <QLineEdit>
class MyLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	MyLineEdit(QWidget * parent = 0);
	virtual ~MyLineEdit();

signals:
	void focussed(bool hasFocus);
	void entered(bool isEnter);

protected :
	virtual void focusInEvent(QFocusEvent * e);
	virtual void focusOutEvent(QFocusEvent * e);
	virtual void enterEvent(QEvent * e);

};

#endif /* MYLINEEDIT_H_ */
