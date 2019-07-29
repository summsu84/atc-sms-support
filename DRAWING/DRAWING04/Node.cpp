/*
 * Node.cpp
 *
 *  Created on: 2013. 1. 21.
 *      Author: jiwon
 */

#include "Node.h"
#include <QDebug>
Node::Node(qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent)
:QGraphicsEllipseItem(x, y, width, height, parent)
{
	// TODO Auto-generated constructor stub
	info = 0;
	menu = new QMenu;
	menu->addAction("test1");
	node_nm = "";
	setFlag(QGraphicsItem::ItemIsMovable);
	setBrush(Qt::blue);

	updateTimer = new QTimer();
	updateTimer->setInterval(100);
	QObject::connect(updateTimer, SIGNAL(timeout()), this, SLOT(move()));
	updateTimer->start();

	count = 0;
	leftCnt = 0;
	rightCnt = 0;
	upCnt = 0;
	downCnt = 0;


}

Node::~Node() {
	// TODO Auto-generated destructor stub
}

void Node::move()
{
	//qDebug()<<"Move is called..";
	qreal xPos = pos().x();
	qreal yPos = pos().y();
	//qDebug()<<"SceneBounding Rect : "<<sceneBoundingRect()<<", X : "<<QString().setNum(sceneBoundingRect().center().x())<<", Y : "<<QString().setNum(sceneBoundingRect().center().y());
	float fx, fy;

	if(count < 50){
		xPos -= 1.0;
		isLeft = true;
		isRight = false;
		isUp = false;
		isDown = false;
	}
	else if (count < 100 ){
		yPos += 1.0;
		isDown = true;
		isRight = false;
		isUp = false;
		isLeft = false;
	}
	else if (count < 150 )
	{
		xPos += 1.0;
		isRight = true;
		isLeft = false;
		isUp = false;
		isDown = false;
	}
	else if (count < 200 )
	{
		yPos -= 1.0;
		isUp = true;
		isRight = false;
		isLeft = false;
		isDown = false;
	}
	else if (count < 250 )
	{
		yPos += 1.0;
		xPos -= 1.0;
	}
	else if (count < 300 )
	{
		yPos -= 1.0;
		xPos += 1.0;
	}else{
		count = 0;
		return;
	}

	count++;
	fx = xPos;
	fy = yPos;
	setPos(fx, fy);
	//setChildPos(fx, fy);
	if(info != 0){
		node_x = sceneBoundingRect().center().x();
		node_y = sceneBoundingRect().center().y();
		info->setPos(node_x, node_y);
	}
	emitFindNode();


}

void Node::setChildPos(qreal x, qreal y)
{
	x = 0;
	y = 0;
	//자식 노드의 위치를 변경 시킨다..
	QGraphicsEllipseItem * item = child_node_list[0];
	if(isLeft && leftCnt == 0){
		qDebug()<<"X Pos : "<<QString().setNum(x)<<", Y Pos : "<<QString().setNum(y);
		item->setPos(x+16, y);
		leftCnt++;
	}
	if(isRight && rightCnt == 0){
		qDebug()<<"X Pos : "<<QString().setNum(x)<<", Y Pos : "<<QString().setNum(y);
		item->setPos(x-16, y);
		rightCnt++;
	}
	if(isUp && upCnt == 0){
		qDebug()<<"X Pos : "<<QString().setNum(x)<<", Y Pos : "<<QString().setNum(y);
		item->setPos(x, y + 16);
		upCnt++;
	}
	if(isDown && downCnt == 0){
		qDebug()<<"X Pos : "<<QString().setNum(x)<<", Y Pos : "<<QString().setNum(y);
		item->setPos(x, y - 16);
		downCnt++;
	}
}

//자식 노드를 그린다.
QVector<QGraphicsEllipseItem *> Node::drawingChild()
{
	qDebug()<<"DrawingChildNode is called.";
	/*for(int i = 0 ; i < CHILD_MAX ; i++){
		qreal x = sceneBoundingRect().center().x();
		qreal y = sceneBoundingRect().center().y();
		//qDebug()<<"X : "<<QString().setNum(x)<<", Y : "<<QString().setNum(y);
		//QGraphicsEllipseItem * child = new QGraphicsEllipseItem(x+40+(i * 2), y+40+(i * 2), (10/i), (10/i), this);
		QGraphicsEllipseItem * child = new QGraphicsEllipseItem(x+40+(i*2), y+40+(i*2), 10, 10, this);
		qreal xPos = pos().x();
		qreal yPos = pos().y();
		setPos(xPos, yPos);
		child_node_list.append(child);
	}*/

	qreal x = sceneBoundingRect().center().x();
	qreal y = sceneBoundingRect().center().y();
	//qDebug()<<"X : "<<QString().setNum(x)<<", Y : "<<QString().setNum(y);
	//QGraphicsEllipseItem * child = new QGraphicsEllipseItem(x+40+(i * 2), y+40+(i * 2), (10/i), (10/i), this);
	QGraphicsEllipseItem * child = new QGraphicsEllipseItem(x, y, 10, 10, this);
	child_node_list.append(child);

	qDebug()<<"DrawingChildNode is closed..";

	return child_node_list;
}


void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug()<<"[Node] mouse press event is called..";
	drawDesc();
	if (event->buttons() & Qt::LeftButton)
	{
		//menu->popup(event->pos());
		qDebug()<<"Item : "<<node_nm<<" is clicked..";
		//event->
	}

	//menu->popup(QPoint(500,500));
	QGraphicsItem::mousePressEvent(event);



}
/*
void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}*/

void Node::drawDesc()
{
	qDebug()<<"[Node]drawDesc is called..";
	qDebug()<<"name : "<<node_nm;
}


void Node::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
 {
     QMenu menu;
     QAction *removeAction = menu.addAction("Remove");
     connect(removeAction, SIGNAL(triggered()), this, SLOT(removeNode()));
     QAction *markAction = menu.addAction("Info");
     connect(markAction, SIGNAL(triggered()), this, SLOT(infoNode()));
     QAction *selectedAction = menu.exec(event->screenPos());
     // ...
 }

void Node::removeNode()
{
	qDebug()<<"remove Node...";
	emitRemoveNode(this);

}

void Node::infoNode()
{
	qDebug()<<"Information of Node is called..";
	node_x = sceneBoundingRect().center().x();
	node_y = sceneBoundingRect().center().y();
	info = new NodeInfo;
	info->setInfo(node_nm, node_x, node_y);
	if (info->exec()) {
		delete info;
		info = 0;
	}else {
		delete info;
		info = 0;
	}

}

/*
QString Node::getPos()
{
	qreal x = sceneBoundingRect().center().x();
	qreal y = sceneBoundingRect().center().y();

	qreal left_x = x - 100;
	qreal right_x = x + 100;
	qreal up_y = y - 100;
	qreal down_y = y + 100;
}*/
