/*
 * DrawingScene.cpp
 *
 *  Created on: 2013. 1. 21.
 *      Author: jiwon
 */

#include "DrawingScene.h"
#include <QGraphicsLineItem>
#include <math.h>

#define PI 3.14
#define NODE_RAD 20

DrawingScene::DrawingScene(QObject * parent)
: QGraphicsScene(parent)
{
	// TODO Auto-generated constructor stub
	mode = -1;

	qDebug()<<"[DrawingScene] constructor is called.";
}

DrawingScene::~DrawingScene() {
	// TODO Auto-generated destructor stub
}

void DrawingScene::initLine()
{

	qDebug()<<"pPoint : "<<pPoint<<", cPoint : "<<cPoint;
	QGraphicsLineItem * item = new QGraphicsLineItem(QLineF(pPoint, cPoint));
	pPoint = cPoint;

	addItem(item);


}

void DrawingScene::mousePressEvent(QGraphicsSceneMouseEvent *eve )
{


	//qDebug()<<"Mouse Press Event called..";
	if (eve->button() != Qt::LeftButton)
		 return;
	if (eve->buttons() & Qt::LeftButton)
	{
		//qDebug()<<"Mouse Button Clicked..";
		//QPointF tmpPos = eve->posF();
		//polygonF<<tmpPos;
		pPoint = eve->scenePos();
		if(mode == 0){
			//Ellipse
			//drawingEllipse();
			drawingChart();
		}

	}
	QGraphicsScene::mousePressEvent(eve);

}

void DrawingScene::mouseMoveEvent(QGraphicsSceneMouseEvent *eve)
{
	QPointF tmpPos = eve->scenePos();
	QString txtpos = "X : ";
	txtpos.append(QString::number(tmpPos.x()));
	txtpos.append(", Y : ");
	txtpos.append(QString::number(tmpPos.y()));

	//qDebug()<<"Pos : "<<txtpos;

	emitPos(txtpos);
	//displayToCoordinate(tmpPos, 1);
	if(eve->buttons() & Qt::LeftButton){
		cPoint = eve->scenePos();
		//initLine();
	}
	QGraphicsScene::mouseMoveEvent(eve);
}



void DrawingScene::drawingEllipse()
{
	//Draw Ellipse
	qreal x = pPoint.x() - 20;
	qreal y = pPoint.y() - 20;
	qreal width = 40;
	qreal height = 40;
	QGraphicsEllipseItem * item = new QGraphicsEllipseItem(x, y, width, height);
	item->setFlag(QGraphicsItem::ItemIsMovable, true);
	item->setFlag(QGraphicsItem::ItemIsSelectable, true);
	item->setBrush(Qt::blue);
	QGraphicsLineItem * lineItem = new QGraphicsLineItem(x + 20, y + 20, x + 40, y - 60, item);
	/*item->setFlag(QGraphicsItem::ItemIsMovable, true);
	item->setFlag(QGraphicsItem::ItemIsSelectable, true);*/
	addItem(item);
	//addItem(lineItem);

}

void DrawingScene::drawingChart()
{
	QPointF strPoint(500,500);
	qreal x = 700;
	qreal y = 700;
	qreal gx = 0;
	qreal gy = 0;
	static int j = 0;
	/*for(int i = 0 ; i < 6 ; i++){
		QGraphicsLineItem * lx = new QGraphicsLineItem(x , y - gy, x + 100, y - gy);
		QGraphicsLineItem * ly = new QGraphicsLineItem(x + gx, y, x + gx, y - 100);
		if(i == 0){

		}
		gx += 20;
		gy += 20;
		addItem(lx);
		addItem(ly);

	}*/
	Node * node = new Node(pPoint.x() -20, pPoint.y() - 20, 20, 20);
	node->setNodeNm("test_"+QString().setNum(j++, 10));
	addItem(node);
	//connect(node, SIGNAL(emitChildNode(QVector<QGrapchisEllipseItem *>)), this, SLOT(drawingChildNode(QVector<QGraphicsEllipseItem *>)));
	//connect(node, SIGNAL(emitDeleteChild(QGraphicsEllipseItem **)), this, SLOT(deleteChildNode(QGraphicsEllipseItem ** )));
	connect(node, SIGNAL(emitRemoveNode(Node * )), this, SLOT(removeNode(Node *)));
	connect(node, SIGNAL(emitFindNode()), this, SLOT(findNode()));
	node_list.append(node);
	//this->findNode();
	//this->drawingChildNode(node->drawingChild());
}


QString DrawingScene::displayToCoordinate(const QPointF& point, int zoom)
{
    // longitude
	zoom = 1;
    double longitude = (point.x()*(360/(pow(2,zoom)*256)))-180;
    // latitude
    double latitude = point.y()*(2/(pow(2,zoom)*256));
    latitude = 1-latitude;
    latitude = latitude*PI;
    latitude = (atan(sinh(latitude))) * 180 / PI;

    //Coordinate coord = Coordinate(longitude, latitude);
    //return coord;
    qDebug()<<"Lat : "<<QString().setNum(latitude)<<", Lon : "<<QString().setNum(longitude);
    return "";
}

//자식 노드들을 그린다..

void DrawingScene::drawingChildNode(QVector<QGraphicsEllipseItem *> child_list)
{
	qDebug()<<"[DrawingScene] drawingChildNode is called..";

	for(int i = 0 ; i < child_list.count() ; i++){
		addItem(child_list[i]);
	}
}

//테스트
void DrawingScene::deleteChildNode(QGraphicsEllipseItem ** child)
{
	qDebug()<<"deleteChildNode is called..";


	for(int i = 0 ; i < 3 ; i++){
		if(!child[i])
		{
			qDebug()<<"remove process..";
			removeItem(child[i]);
		}

	}

}

/**
 * Node를 제거 한다.
 */
void DrawingScene::removeNode(Node * node)
{
	qDebug()<<"removeNode is called..";

	removeItem(node);
}

bool DrawingScene::findNode()
{
	qDebug()<<"Find Node is called..";

	QList<QGraphicsItem * > item_list = this->items();

	/*qDebug()<<"Item Count : "<<item_list.count();
	for(int i = 0 ; i  < item_list.count() ; i++){
		Node * item = (Node *)item_list.at(i);
		QRectF itemBR = item->boundingRect();
		//QRectF sceneBR = item->mapToScene(itemBR);
		qDebug()<<"Node Name : "<<item->getNodeNm()<<", Pos : "<<item->pos()<<", BR : "<<itemBR;
	}*/

	//Collide test
	for(int i = 0 ; i < item_list.count() ; i++){
		Node * item = (Node *)item_list.at(i);
		for(int j = i + 1 ; j < item_list.count() ; j++){
			Node * target_item = (Node *)item_list.at(j);
			bool chk = item->collidesWithItem(target_item);
			if(chk == true){
				qDebug()<<"Collision Detection.. "<<item->getNodeNm()<<" with "<<target_item->getNodeNm();
				item->updateTimer->stop();
				target_item->updateTimer->stop();
			}
		}
	}

	qDebug()<<"Find Node is close..";
	return true;
}
