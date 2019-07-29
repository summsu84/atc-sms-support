/*
 * Window02.h
 *
 *  Created on: 2012. 12. 21.
 *      Author: jiwon
 */

#ifndef WINDOW03_H_
#define WINDOW03_H_

#include <QtGui>
#include <QWidget>
#include "RenderArea03.h"

class Window03 : public QWidget
{
	Q_OBJECT

public:
	Window03(QWidget * parent = 0);
	virtual ~Window03();

private slots:
	void shapeChanged();
	void penChanged();
	void brushChanged();
	void printDebug(const QString & prt);
	void printPos(const QString & prt);
private:
	RenderArea03 *renderArea;
	QLabel *shapeLabel;
	QLabel *penWidthLabel;
	QLabel *penStyleLabel;
	QLabel *lblPos;
	QComboBox *shapeComboBox;
	QPushButton * btnLine;
	QPushButton * btnEllipse;
	QPushButton * btnPolygon;
	QSpinBox *penWidthSpinBox;
	QComboBox *penStyleComboBox;
	QPlainTextEdit * txtDebug;
	QScrollArea * scrollArea;
	QLineEdit * txtPos;


};

#endif /* WINDOW03_H_ */
