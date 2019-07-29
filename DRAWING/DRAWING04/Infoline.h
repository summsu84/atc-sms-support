/*
 * Infoline.h
 *
 *  Created on: 2013. 2. 4.
 *      Author: jiwon
 */

#ifndef INFOLINEX_H_
#define INFOLINEX_H_

#include <QGraphicsLineItem>

class Node;

class InfolineX : public QGraphicsLineItem
{
public:
	InfolineX(Node * startNode, Node * endNode, QGraphicsItem * parent = 0, QGraphicsScene * scene= 0);
	virtual ~InfolineX();
	QRectF boundingRect() const;
	QPainterPath shape() const;
	Node *startItem() const
	{ return myStartItem; }
	Node *endItem() const
	{ return myEndItem; }

public slots:
	void updatePosition();

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
private:
     Node *myStartItem;
     Node *myEndItem;
     QColor myColor;
     QPolygonF arrowHead;
};

#endif /* INFOLINE_H_ */

