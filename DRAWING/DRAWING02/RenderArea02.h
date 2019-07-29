
/*
 * RenderArea02.h
 *
 *  Created on: 2012. 12. 18.
 *      Author: jiwon
*/


#ifndef RENDERAREA02_H_
#define RENDERAREA02_H_

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include "../../lib/Function.h"

class RenderArea02 : public QWidget
{
	Q_OBJECT

public:
	RenderArea02(QWidget *parent = 0);
	 QSize minimumSizeHint() const;
	 QSize sizeHint() const;
	 void drawLineTo();
	 void drawCircleTo();
public slots:
	 void setShape(Shape shape);
	 void setPen(const QPen &pen);
	 void setBrush(const QBrush &brush);
	 void setAntialiased(bool antialiased);
	 void setTransformed(bool transformed);
	 void resizeImage(QImage *image, const QSize &newSize);
protected:
	 void paintEvent(QPaintEvent *event);
	 void mousePressEvent(QMouseEvent *eve);
	 void mouseMoveEvent(QMouseEvent *event);
	 void mouseReleaseEvent(QMouseEvent *event);
	 void resizeEvent(QResizeEvent *event);
private:
	Shape shape;
	QPen pen;
	QBrush brush;
	bool antialiased;
	bool transformed;
	QPixmap pixmap;
	QImage image;
	QPoint cPoint;
	//prev
	QPoint pPoint;
	bool scribbling;
	bool isRealsed;		//false : Press,  true: Released;
	int cnt;
	int ellipseCnt;
signals:
	void emitDebug(const QString & prt);
	void emitPos(const QString & prt);
};
#endif  RENDERAREA02_H_

