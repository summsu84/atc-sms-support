#ifndef WINDOW_H
#define WINDOW_H

#include <QtGui>
#include "RenderArea.h"
//#include "ui_WINDOW.h"

class RenderArea;

class WINDOW : public QWidget
{
    Q_OBJECT

public:
    WINDOW(QWidget *parent = 0);
    ~WINDOW();

private slots:
	void shapeChanged();
	void penChanged();
	void brushChanged();


private:
    //Ui::WINDOWClass ui;
	RenderArea * renderArea;
	QLabel * shapeLabel;
	QLabel * penWidthLabel;
	QLabel * penStyleLabel;
	QLabel * penCapLabel;
	QLabel * penJoinLabel;
	QLabel * brushStyleLabel;
	QLabel * otherOptionsLabel;
	QComboBox * shapeComboBox;
	QSpinBox * penWidthSPinBox;
	QComboBox * penStyleComboBox;
	QComboBox * penCapCombobox;
	QComboBox * penJoinComboBox;
	QComboBox * brushStyleComboBox;
	QCheckBox * antialiasingCheckBox;
	QCheckBox * transformationCheckBox;

};

#endif // WINDOW_H
