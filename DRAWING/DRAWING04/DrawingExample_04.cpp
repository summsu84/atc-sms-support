/*
 * DrawingExample_04.cpp
 *
 *  Created on: 2013. 1. 17.
 *      Author: jiwon
 */

#include "DrawingExample_04.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QPainter>

DrawingExample_04::DrawingExample_04(QWidget *parent)
: QGraphicsView(parent)
{
	qDebug()<<"DrawingExample_04 is called..";
	// TODO Auto-generated constructor stub

	//setGeometry(QRect(0, 0, 250, 250));

	/*scene = new QGraphicsScene();
	scene->setSceneRect(0,0,250,250);
	setScene(scene);*/
	this->setMouseTracking(true);

	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);


	//Set-up the view
	setSceneRect(0, 0, 1000, 1000);
	SetCenter(QPointF(500.0, 500.0)); //A modified version of centerOn(), handles special cases
	setInteractive(true);
	setViewportUpdateMode(BoundingRectViewportUpdate);
	//setCursor(Qt::OpenHandCursor);
	//show();

	qDebug()<<"DrawingExample_04 constructor is close..";
}

DrawingExample_04::~DrawingExample_04() {
	// TODO Auto-generated destructor stub
}


void DrawingExample_04::SetCenter(const QPointF& centerPoint) {
    //Get the rectangle of the visible area in scene coords
    QRectF visibleArea = mapToScene(rect()).boundingRect();

    //Get the scene area
    QRectF sceneBounds = sceneRect();

    double boundX = visibleArea.width() / 2.0;
    double boundY = visibleArea.height() / 2.0;
    double boundWidth = sceneBounds.width() - 2.0 * boundX;
    double boundHeight = sceneBounds.height() - 2.0 * boundY;

    //The max boundary that the centerPoint can be to
    QRectF bounds(boundX, boundY, boundWidth, boundHeight);

    if(bounds.contains(centerPoint)) {
        //We are within the bounds
        CurrentCenterPoint = centerPoint;
    } else {
        //We need to clamp or use the center of the screen
        if(visibleArea.contains(sceneBounds)) {
            //Use the center of scene ie. we can see the whole scene
            CurrentCenterPoint = sceneBounds.center();
        } else {

            CurrentCenterPoint = centerPoint;

            //We need to clamp the center. The centerPoint is too large
            if(centerPoint.x() > bounds.x() + bounds.width()) {
                CurrentCenterPoint.setX(bounds.x() + bounds.width());
            } else if(centerPoint.x() < bounds.x()) {
                CurrentCenterPoint.setX(bounds.x());
            }

            if(centerPoint.y() > bounds.y() + bounds.height()) {
                CurrentCenterPoint.setY(bounds.y() + bounds.height());
            } else if(centerPoint.y() < bounds.y()) {
                CurrentCenterPoint.setY(bounds.y());
            }

        }
    }

    //Update the scrollbars
    centerOn(CurrentCenterPoint);
}

/**
  * Zoom the view in and out.
  */
void DrawingExample_04::wheelEvent(QWheelEvent* event) {

    //Get the position of the mouse before scaling, in scene coords
    QPointF pointBeforeScale(mapToScene(event->pos()));

    //Get the original screen centerpoint
    QPointF screenCenter = GetCenter(); //CurrentCenterPoint; //(visRect.center());

    //Scale the view ie. do the zoom
    double scaleFactor = 1.15; //How fast we zoom
    if(event->delta() > 0) {
        //Zoom in
        scale(scaleFactor, scaleFactor);
        qDebug()<<"Zoom in..";
    } else {
        //Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }

    //Get the position after scaling, in scene coords
    QPointF pointAfterScale(mapToScene(event->pos()));

    //Get the offset of how the screen moved
    QPointF offset = pointBeforeScale - pointAfterScale;

    //Adjust to the new center for correct zooming
    QPointF newCenter = screenCenter + offset;
    SetCenter(newCenter);
}



void DrawingExample_04::printPos(const QString & prt)
{
	qDebug()<<"[DrawingExample_04] printPos called..";
	emitPos(prt);
}

void DrawingExample_04::resizeEvent(QResizeEvent* event)
{

	QRectF visibleArea = mapToScene(rect()).boundingRect();
	QPointF test = mapToScene(10,10);
	QPoint test1 = mapFromScene(QPointF(200,200));
	qDebug()<<"Resize Event is called.."<<visibleArea<<", boundingRect() : "<<rect()<<", Test Point : "<<test<<", Test1 Point : "<<test1;



}

/**
  * Need to update the center so there is no jolt in the
  * interaction after resizing the widget.
  */
/*void DrawingExample_04::resizeEvent(QResizeEvent* event) {
    //Get the rectangle of the visible area in scene coords
	qDebug()<<"Resize Event is called..";
    QRectF visibleArea = mapToScene(rect()).boundingRect();
    SetCenter(visibleArea.center());

    //Call the subclass resize so the scrollbars are updated correctly
    QGraphicsView::resizeEvent(event);
}*/
