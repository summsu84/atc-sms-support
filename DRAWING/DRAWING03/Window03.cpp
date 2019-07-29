/*
 * Window.cpp
 *
 *  Created on: 2012. 12. 20.
 *      Author: HIST
 */

#include "Window03.h"

#include <QtGui>

using namespace::std;
const int IdRole = Qt::UserRole;

Window03::Window03(QWidget *parent)
: QWidget(parent)
{
	qDebug()<<"start..";
	renderArea = new RenderArea03;

/*	shapeComboBox = new QComboBox;
	shapeComboBox->addItem(tr("Polygon"), Polygon);
	shapeComboBox->addItem(tr("Rectangle"), Rect);
	shapeComboBox->addItem(tr("Rounded Rectangle"), RoundedRect);
	shapeComboBox->addItem(tr("Ellipse"), Ellipse);
	shapeComboBox->addItem(tr("Pie"), Pie);
	shapeComboBox->addItem(tr("Chord"), Chord);
	shapeComboBox->addItem(tr("Path"), Path);
	shapeComboBox->addItem(tr("Line"), Line);
	shapeComboBox->addItem(tr("Polyline"), Polyline);
	shapeComboBox->addItem(tr("Arc"), Arc);
	shapeComboBox->addItem(tr("Points"), Points);
	shapeComboBox->addItem(tr("Text"), Text);
	shapeComboBox->addItem(tr("Pixmap"), Pixmap);*/

	shapeLabel = new QLabel(tr("&Shape:"));
//	shapeLabel->setBuddy(shapeComboBox);

	btnLine = new QPushButton(tr("Line"));
	btnLine->setMaximumSize(50,20);
	btnLine->setObjectName("btnLine");
	btnEllipse = new QPushButton(tr("Ellipse"));
	btnEllipse->setObjectName("btnEllipse");
	btnEllipse->setMaximumSize(50,20);
	btnPolygon = new QPushButton(tr("Polygon"));
	btnPolygon->setObjectName("btnPolygon");
	btnPolygon->setMaximumSize(50,20);

	penWidthSpinBox = new QSpinBox;
	penWidthSpinBox->setRange(0, 20);
	penWidthSpinBox->setSpecialValueText(tr("0 (cosmetic pen)"));

	penWidthLabel = new QLabel(tr("Pen &Width:"));
	penWidthLabel->setBuddy(penWidthSpinBox);

	penStyleComboBox = new QComboBox;
	penStyleComboBox->addItem(tr("Solid"), Qt::SolidLine);
	penStyleComboBox->addItem(tr("Dash"), Qt::DashLine);
	penStyleComboBox->addItem(tr("Dot"), Qt::DotLine);
	penStyleComboBox->addItem(tr("Dash Dot"), Qt::DashDotLine);
	penStyleComboBox->addItem(tr("Dash Dot Dot"), Qt::DashDotDotLine);
	penStyleComboBox->addItem(tr("None"), Qt::NoPen);

	penStyleLabel = new QLabel(tr("&Pen Style:"));
	penStyleLabel->setBuddy(penStyleComboBox);



	txtDebug = new QPlainTextEdit;
	scrollArea = new QScrollArea;
	lblPos = new QLabel(tr("&Pos:"));
	txtPos = new QLineEdit;

	connect(btnLine, SIGNAL(clicked()), this, SLOT(shapeChanged()));
	connect(btnEllipse, SIGNAL(clicked()), this, SLOT(shapeChanged()));
	connect(btnPolygon, SIGNAL(clicked()), this, SLOT(shapeChanged()));
	connect(penWidthSpinBox, SIGNAL(valueChanged(int)),
		 this, SLOT(penChanged()));
	connect(penStyleComboBox, SIGNAL(activated(int)),
		 this, SLOT(penChanged()));
	connect(renderArea, SIGNAL(emitPos(const QString &)),this, SLOT(printPos(const QString &)));

	scrollArea->setWidget(renderArea);
	scrollArea->setWidgetResizable(true);
	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->setColumnStretch(0, 1);
	mainLayout->setColumnStretch(3, 1);
	mainLayout->addWidget(scrollArea, 0, 0, 1, 4);
	mainLayout->setRowMinimumHeight(1, 6);
	mainLayout->addWidget(shapeLabel, 2, 1, Qt::AlignRight);
	mainLayout->addWidget(btnLine, 2, 2,1 ,1);
	mainLayout->addWidget(btnEllipse, 2, 3, 1, 1);
	mainLayout->addWidget(btnPolygon, 2, 4, 1, 1);
	mainLayout->addWidget(penWidthLabel, 3, 1, Qt::AlignRight);
	mainLayout->addWidget(penWidthSpinBox, 3, 2);
	mainLayout->addWidget(penStyleLabel, 4, 1, Qt::AlignRight);
	mainLayout->addWidget(penStyleComboBox, 4, 2);
	//mainLayout->addWidget(txtDebug, 5, 1, Qt::AlignRight);
	mainLayout->addWidget(lblPos, 5, 1, Qt::AlignRight);
	mainLayout->addWidget(txtPos, 5, 2);
	setLayout(mainLayout);

	//shapeChanged();
	penChanged();
	brushChanged();

	setWindowTitle(tr("Basic Drawing"));
	qDebug()<<"Window03 constructor closed..";
}

Window03::~Window03()
{
	//cout<<"Close"<<endl;
}

void Window03::shapeChanged()
{
	qDebug()<<"shapeChanged called..";
	Shape shape;
	QPushButton * tmp = (QPushButton *)sender();
	qDebug()<<"ObjectName : "<<tmp->objectName();
	if(tmp->objectName() == "btnLine"){
		shape = Line;
	}else if(tmp->objectName() == "btnEllipse"){
		shape = Ellipse;
	}else if(tmp->objectName() == "btnPolygon"){
		shape = Polygon;
	}
	qDebug()<<"Sahpe : "<<shape;
	renderArea->setShape(shape);
}

void Window03::penChanged()
{
	int width = penWidthSpinBox->value();
	Qt::PenStyle style = Qt::PenStyle(penStyleComboBox->itemData(
		 penStyleComboBox->currentIndex(), IdRole).toInt());


	//renderArea->setPen(QPen(Qt::blue, width, style, cap, join));
}

void Window03::brushChanged()
{

}

void Window03::printDebug(const QString & prt)
{
	QString line = prt + QChar('\n');
	txtDebug->setPlainText(txtDebug->toPlainText().append(line));
}
void Window03::printPos(const QString & prt)
{
	txtPos->setText(prt);
}
