#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QtGui>
#include <QBrush>
#include <QPen>
#include <QPixmap>
#include "../lib/Function.h"
//#include "ui_RenderArea.h"

class RenderArea : public QWidget
{
    Q_OBJECT

public:
    RenderArea(QWidget *parent = 0);
    ~RenderArea();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
public slots:
	void setShape(Shape shape);
	void setPen(const QPen &pen);
	void setBrush(const QBrush & brush);
	void setAntialiased(bool antialiased);
	void setTransformed(bool transformed);
protected :
	void paintEvent(QPaintEvent * event);
private:
	Shape shape;
	QPen pen;
	QBrush brush;
	bool antialiased;
	bool transformed;
	QPixmap pixmap;
    //Ui::RenderAreaClass ui;
};

#endif // RENDERAREA_H
