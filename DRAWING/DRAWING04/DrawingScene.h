/*
 * DrawingScene.h
 *
 *  Created on: 2013. 1. 21.
 *      Author: jiwon
 */

#ifndef DRAWINGSCENE_H_
#define DRAWINGSCENE_H_

 #include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QWheelEvent>
#include <QGraphicsEllipseItem>

#include "Node.h"


class DrawingScene : public QGraphicsScene
{
	Q_OBJECT

public:
	DrawingScene(QObject * parent = 0);
	virtual ~DrawingScene();
	void initLine();
	void drawingEllipse();
	void setMode(int mode){ this->mode = mode;}
	void drawingChart();
	QString displayToCoordinate(const QPointF& point, int zoom);
	void drawingChildNode(QVector<QGraphicsEllipseItem *> child_list);
public slots:
	bool findNode();
	void deleteChildNode(QGraphicsEllipseItem ** child);
	void removeNode(Node * node);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *eve);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *eve);
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
private:
	//Holds the current centerpoint for the view, used for panning and zooming
	QPointF CurrentCenterPoint;
	QVector<Node *> node_list;
	//From panning the view
	QPoint LastPanPoint;
	QPointF pPoint;
	QPointF cPoint;

	int mode;
signals:
	//void emitDebug(const QString & prt);
	void emitPos(const QString & prt);
};

#endif /* DRAWINGSCENE_H_ */
