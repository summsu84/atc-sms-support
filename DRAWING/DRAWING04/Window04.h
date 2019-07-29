/*
 * Window04.h
 *
 *  Created on: 2013. 1. 17.
 *      Author: jiwon
 */

#ifndef WINDOW04_H_
#define WINDOW04_H_
#include <QtGui>
#include <QWidget>
#include "DrawingExample_04.h"
#include "DrawingScene.h"
#include <QGraphicsView>

class Window04 :  public QWidget
{
	Q_OBJECT

public:
	Window04(QWidget * parent = 0);
	virtual ~Window04();
public slots:
	void printPos(const QString & prt);
	void shapeChanged();
protected:
	void closeEvent(QCloseEvent *event);
private:
	DrawingExample_04 * drawingArea;
	//QGraphicsView * view;
	DrawingScene * scene;
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

#endif /* WINDOW04_H_ */
