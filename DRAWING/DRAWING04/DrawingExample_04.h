/*
 * DrawingExample_04.h
 *
 *  Created on: 2013. 1. 17.
 *      Author: jiwon
 */

#ifndef DRAWINGEXAMPLE_04_H_
#define DRAWINGEXAMPLE_04_H_

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QMouseEvent>
#include "DrawingScene.h"

class DrawingExample_04 : public QGraphicsView
{
	Q_OBJECT
public:
	DrawingExample_04(QWidget *parent = 0);
	virtual ~DrawingExample_04();
	//void initLine();
	void SetCenter(const QPointF& centerPoint);
	QPointF GetCenter() { return CurrentCenterPoint; }

public slots:
	void printPos(const QString & prt);

protected:
	void resizeEvent(QResizeEvent* event);
	void wheelEvent(QWheelEvent* event);
private:
	//DrawingScene * scene;
	QPolygonF polygonF;
	QPointF pointF;
    //Holds the current centerpoint for the view, used for panning and zooming
    QPointF CurrentCenterPoint;

    //From panning the view
    QPoint LastPanPoint;

    QPointF pPoint;
    QPointF cPoint;
signals:
	//void emitDebug(const QString & prt);
	void emitPos(const QString & prt);
};

#endif /* DRAWINGEXAMPLE_04_H_ */
