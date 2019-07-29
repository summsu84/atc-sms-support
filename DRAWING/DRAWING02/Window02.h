/*
 * Window02.h
 *
 *  Created on: 2012. 12. 21.
 *      Author: jiwon
 */

#ifndef WINDOW02_H_
#define WINDOW02_H_

#include <QtGui>
#include <QWidget>
#include "RenderArea02.h"

class Window02 : public QWidget
{
	Q_OBJECT

public:
	Window02();
	virtual ~Window02();

private slots:
	void shapeChanged();
	void penChanged();
	void brushChanged();
	void printDebug(const QString & prt);
	void printPos(const QString & prt);
private:
	RenderArea02 *renderArea;
	QLabel *shapeLabel;
	QLabel *penWidthLabel;
	QLabel *penStyleLabel;
	QLabel *penCapLabel;
	QLabel *penJoinLabel;
	QLabel *brushStyleLabel;
	QLabel *otherOptionsLabel;
	QComboBox *shapeComboBox;
	QSpinBox *penWidthSpinBox;
	QComboBox *penStyleComboBox;
	QComboBox *penCapComboBox;
	QComboBox *penJoinComboBox;
	QComboBox *brushStyleComboBox;
	QCheckBox *antialiasingCheckBox;
	QCheckBox *transformationsCheckBox;
	QPlainTextEdit * txtDebug;
	QScrollArea * scrollArea;
	QLineEdit * txtPos;
};

#endif /* WINDOW02_H_ */
