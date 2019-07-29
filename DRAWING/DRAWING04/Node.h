/*
 * Node.h
 *
 *  Created on: 2013. 1. 21.
 *      Author: jiwon
 */

#ifndef NODE_H_
#define NODE_H_

#define CHILD_MAX 3

#include <QGraphicsItem>
#include <QString>
#include <QPainter>
#include <QTimer>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include "NodeInfo.h"
#include "Infoline.h"

class Node : public QObject, public QGraphicsEllipseItem
{
	Q_OBJECT
public:
	Node(qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent = 0);
	virtual ~Node();
	 //QPainterPath shape() const;
	void setNodeNm(QString nm){ this->node_nm = nm;}
	QString getNodeNm(){return this->node_nm;}
	void drawDesc();
	void setChildPos(qreal x, qreal y);
	QVector<QGraphicsEllipseItem *> drawingChild();
	QTimer * updateTimer;
	int count;

public slots:
	void move();
	void removeNode();
	void infoNode();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	//void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
	InfolineX *infoline;
	QVector<QGraphicsEllipseItem *> child_node_list;
	NodeInfo * info;						//Node Info
	QVector<QPointF> point_list;
	QString node_nm;

	bool isUp;
	bool isDown;
	bool isLeft;
	bool isRight;
	QMenu * menu;
	qreal node_x;
	qreal node_y;
	int leftCnt;
	int rightCnt;
	int upCnt;
	int downCnt;

signals:
	void emitChildNode(QVector<QGraphicsEllipseItem *> child_list);
	void emitDeleteChild(QGraphicsEllipseItem ** child);
	void emitRemoveNode(Node * node);
	void emitFindNode();
};

#endif /* NODE_H_ */
