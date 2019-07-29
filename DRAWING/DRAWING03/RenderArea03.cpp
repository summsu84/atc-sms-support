/*
 * RenderArea.cpp
 *
 *  Created on: 2012. 12. 20.
 *      Author: HIST
 */


 #include <QtGui>

 #include "RenderArea03.h"

 RenderArea03::RenderArea03(QWidget *parent)
     : QWidget(parent)
 {
	 cnt = 0;
	 ellipseCnt = 0;
	 isRealsed = false;
     shape = Polygon;
     antialiased = false;
     transformed = false;
     pixmap.load(":/images/qt-logo.png");

     setBackgroundRole(QPalette::Base);
     //setAutoFillBackground(false);
     setAttribute(Qt::WA_NoSystemBackground, true);
     setAttribute(Qt::WA_OpaquePaintEvent, true);
     this->setMouseTracking(true);
 }

 QSize RenderArea03::minimumSizeHint() const
 {
     return QSize(100, 100);
 }

 QSize RenderArea03::sizeHint() const
 {
     return QSize(400, 200);
 }

 void RenderArea03::setShape(Shape shape)
 {
     this->shape = shape;
     update();
 }

 void RenderArea03::setPen(const QPen &pen)
 {
     this->pen = pen;
     update();
 }

 void RenderArea03::setBrush(const QBrush &brush)
 {
     this->brush = brush;
     update();
 }

 void RenderArea03::setAntialiased(bool antialiased)
 {
     this->antialiased = antialiased;
     update();
 }

 void RenderArea03::setTransformed(bool transformed)
 {
     this->transformed = transformed;
     update();
 }

void RenderArea03::paintEvent(QPaintEvent *event )
{
	/*QPainter painter(this);
	painter.setPen(pen);
	painter.setBrush(brush);
	QRect dirtyRect = event->rect();
	painter.drawImage(dirtyRect, image, dirtyRect);*/
	QPainter imagePainter(&image);
	imagePainter.setRenderHint( QPainter::Antialiasing, true );
	imagePainter.eraseRect( rect() );
	draw( &imagePainter );
	imagePainter.end();

	QPainter painter( this );
	QRect dirtyRect = event->rect();
	painter.drawImage( dirtyRect, (image), dirtyRect );

}

void RenderArea03::mousePressEvent(QMouseEvent *eve )
{
/*	if (eve->button() == Qt::LeftButton) {
		if(shape == Polygon){
			QPoint tmp = eve->pos();
			polygonList.append(tmp);
		}else{
			pPoint = eve->pos();
		}
		scribbling = true;
		isRealsed = false;
	}else if(eve->button() == Qt::RightButton && shape == Polygon){
		drawPolygonTo();
		polygonList.clear();
	}*/
	if (eve->buttons() & Qt::LeftButton)
	{
		pPoint = eve->pos();
		scribbling = true;
		update();
	}
}
void RenderArea03::mouseMoveEvent(QMouseEvent *event)
{
	QPoint tmpPos = event->pos();
	QString txtpos = "X : ";
	txtpos.append(QString::number(tmpPos.x(),10));
	txtpos.append(", Y : ");
	txtpos.append(QString::number(tmpPos.y(), 10));
	emitPos(txtpos);
   if ((event->buttons() & Qt::LeftButton) && scribbling)
   {
		/*cPoint = event->pos();
		if(shape == Line)
			drawPaint();
		//update(QRect(pPoint,cPoint).normalized().adjusted(-rad, -rad, +rad, +rad));*/
	   cPoint = event->pos();
	   update();
   }
}

void RenderArea03::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && scribbling) {
        //drawLineTo(event->pos());
		cPoint = event->pos();
		scribbling = false;
		update();
		//event->accept();
		//cPoint = event->pos();
/*		scribbling = false;
		isRealsed = true;
		drawPaint();*/
	}
}


void RenderArea03::draw( QPainter *painter )
{
/*	painter->save();
	painter->translate( width() / 2, height() / 2 );
	painter->setPen( QColor( 0, 0, 0, 255 ) );
	painter->drawEllipse( QPoint( 0, 90 ), 90, 90 );
	painter->restore();*/

	//painter->save();
	painter->setPen( QPen( Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
	if(pointList.count() == 0){
		painter->drawLine( pPoint, cPoint );

	}else{
		for(int i = 0 ; i < pointList.count() ; i++){
			LINEPOINT point = (LINEPOINT)pointList.at(i);
			painter->drawLine(point.pPoint, point.cPoint);
		}
		painter->drawLine(pPoint, cPoint);
	}
	if(scribbling == false){
		LINEPOINT point;
		point.pPoint = pPoint;
		point.cPoint = cPoint;
		pointList.append(point);
	}
	//painter->restore();
}


void RenderArea03::drawPaint()
{
	switch(shape)
	{
		case Line:
		{
			drawLineTo();
			break;
		}
		case Ellipse:
		{
			drawCircleTo();
			break;
		}
		case Polygon:
		{
			//drawPolygonTo();
			break;
		}
	}
}

void RenderArea03::drawLineTo()
{
	QPainter painter(&image);
	painter.setPen(pen);
	painter.setBrush(brush);
	painter.drawLine(pPoint, cPoint);
	painter.end();
	int myPenWidth = 100;
	int rad = (myPenWidth / 2) + 2;
	update(QRect(pPoint,cPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
	pPoint = cPoint;
}
void RenderArea03::drawCircleTo()
{
	//emitDebug("DrawLineTo : ");
	QRect rect(pPoint, cPoint);
	QPainter painter(&image);
	painter.setPen(pen);
	painter.setBrush(brush);
	if(ellipseCnt == 0){
		pPoint = cPoint;
	}
	painter.drawEllipse(rect);
	int myPenWidth = 1;
	int rad = (myPenWidth / 2) + 2;
	update(QRect(pPoint,cPoint).normalized().adjusted(-rad, -rad, +rad, +rad));

}

void RenderArea03::drawPolygonTo()
{
	QPainter painter(&image);
	painter.setPen(pen);
	painter.setBrush(brush);

	QPointF * points = new QPointF[polygonList.count()];
	for(int i = 0 ; i < polygonList.count() ; i++)
	{
		points[i] = QPointF(polygonList[i]);
		qDebug()<<"Points : "<<points[i].toPoint().x()<<", "<<points[i].toPoint().y();
	}


	painter.drawPolygon(points, polygonList.count());
	int myPenWidth = 1;
	int rad = (myPenWidth / 2) + 2;
	update(QRect(image.rect().topLeft(), image.rect().bottomRight()).normalized().adjusted(-rad, -rad, +rad, +rad));

}

void RenderArea03::resizeEvent(QResizeEvent *event)
{
	//emitDebug("ResizeEvent called..");
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 512, image.width());
        int newHeight = qMax(height() + 512, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void RenderArea03::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}
