/*
 * RenderArea.cpp
 *
 *  Created on: 2012. 12. 20.
 *      Author: HIST
 */


 #include <QtGui>

 #include "RenderArea02.h"

 RenderArea02::RenderArea02(QWidget *parent)
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
     setAutoFillBackground(false);
     setAttribute(Qt::WA_NoSystemBackground, true);
     setAttribute(Qt::WA_OpaquePaintEvent, true);
 }

 QSize RenderArea02::minimumSizeHint() const
 {
     return QSize(100, 100);
 }

 QSize RenderArea02::sizeHint() const
 {
     return QSize(400, 200);
 }

 void RenderArea02::setShape(Shape shape)
 {
     this->shape = shape;
     update();
 }

 void RenderArea02::setPen(const QPen &pen)
 {
     this->pen = pen;
     update();
 }

 void RenderArea02::setBrush(const QBrush &brush)
 {
     this->brush = brush;
     update();
 }

 void RenderArea02::setAntialiased(bool antialiased)
 {
     this->antialiased = antialiased;
     update();
 }

 void RenderArea02::setTransformed(bool transformed)
 {
     this->transformed = transformed;
     update();
 }

void RenderArea02::paintEvent(QPaintEvent *event )
{
	QPainter painter(this);
	painter.setPen(pen);
	painter.setBrush(brush);

	switch(shape){

	}


	painter.drawLine(pPoint, cPoint);
	//QPainter painter(this);
	if(isRealsed){
		QRect dirtyRect = event->rect();
		painter.drawImage(dirtyRect, image, dirtyRect);
	}

}

void RenderArea02::mousePressEvent(QMouseEvent *eve )
{
	if (eve->button() == Qt::LeftButton) {
		pPoint = eve->pos();
		scribbling = true;
		isRealsed = false;
	}
	QString txtpos = "X : ";
	txtpos.append(QString::number(pPoint.x(),10));
	txtpos.append(", Y : ");
	txtpos.append(QString::number(pPoint.y(), 10));
	emitDebug(txtpos);
}
void RenderArea02::mouseMoveEvent(QMouseEvent *event)
{
   if ((event->buttons() & Qt::LeftButton) && scribbling)
   {
	   this->setMouseTracking(true);
		QPoint nPos = event->pos();
		QString txtpos = "X : ";
		txtpos.append(QString::number(nPos.x(),10));
		txtpos.append(", Y : ");
		txtpos.append(QString::number(nPos.y(), 10));
		emitPos(txtpos);

		cPoint = event->pos();
		int myPenWidth = 1;
		int rad = (myPenWidth / 2) + 2;
		update(QRect(pPoint,cPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
   }
}

void RenderArea02::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && scribbling) {
        //drawLineTo(event->pos());
		cPoint = event->pos();
		scribbling = false;
		isRealsed = true;
		drawLineTo();
        if(shape == Ellipse)
        	{
        		cPoint = event->pos();
        		QString txtpos = "Mouse Relesed .. X : ";
        		txtpos.append(QString::number(cPoint.x(),10));
        		txtpos.append(", Y : ");
        		txtpos.append(QString::number(cPoint.y(), 10));
        		emitDebug(txtpos);
        		drawCircleTo();
        	}
    }

}
void RenderArea02::drawLineTo()
{
	//emitDebug("DrawLineTo : ");

	QPainter painter(&image);
	painter.setPen(pen);
	painter.setBrush(brush);
	painter.drawLine(pPoint, cPoint);
	painter.end();
	int myPenWidth = 100;
	int rad = (myPenWidth / 2) + 2;
	qDebug()<<"DrawLineTo Rect :"<<QRect(pPoint, cPoint).topLeft()<<", "<<QRect(pPoint, cPoint).bottomRight();
	qDebug()<<"QImage Rect : "<<image.rect();

	update(QRect(QPoint(0,0),QPoint(528,328)));

}
void RenderArea02::drawCircleTo()
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
	//update(QRect(pPoint,cPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
	update();
	pPoint = cPoint;
	ellipseCnt++;
}
void RenderArea02::resizeEvent(QResizeEvent *event)
{
	//emitDebug("ResizeEvent called..");
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void RenderArea02::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}
