/*
 * Window04.cpp
 *
 *  Created on: 2013. 1. 17.
 *      Author: jiwon
 */

#include "Window04.h"
#include "DrawingScene.h"

Window04::Window04(QWidget * parent)
: QWidget(parent)
{
	// TODO Auto-generated constructor stub
	qDebug()<<"[Window04] start..";

	scene = new DrawingScene;
	drawingArea = new DrawingExample_04;
	drawingArea->setScene(scene);
	shapeLabel = new QLabel(tr("&Shape:"));

	btnLine = new QPushButton(tr("Undraw"));
	btnLine->setMaximumSize(50,20);
	btnLine->setObjectName("btnLine");
	btnEllipse = new QPushButton(tr("Draw"));
	btnEllipse->setObjectName("btnEllipse");
	btnEllipse->setMaximumSize(50,20);

	txtDebug = new QPlainTextEdit;
	scrollArea = new QScrollArea;
	lblPos = new QLabel(tr("&Pos:"));
	txtPos = new QLineEdit;

	connect(btnLine, SIGNAL(clicked()), this, SLOT(shapeChanged()));
	connect(btnEllipse, SIGNAL(clicked()), this, SLOT(shapeChanged()));

	connect(scene, SIGNAL(emitPos(const QString &)),this, SLOT(printPos(const QString &)));

	scrollArea->setWidget(drawingArea);
	scrollArea->setWidgetResizable(true);
	QGridLayout *mainLayout = new QGridLayout;
	//Layout
	mainLayout->setColumnStretch(0, 1);
	//mainLayout->setColumnStretch(3, 1);
	mainLayout->addWidget(scrollArea, 0, 0, 1, 0);
	mainLayout->setRowMinimumHeight(1, 6);
	mainLayout->addWidget(shapeLabel, 2, 1, Qt::AlignRight);
	mainLayout->addWidget(btnLine, 2, 2);
	mainLayout->addWidget(btnEllipse, 2, 3);
	mainLayout->addWidget(lblPos, 3, 1, Qt::AlignRight);
	mainLayout->addWidget(txtPos, 3, 2);

	setLayout(mainLayout);

	scene->setBackgroundBrush(QPixmap(":/background1.png"));

	scene->update();
	drawingArea->update();

	setWindowTitle(tr("Basic Drawing"));
	qDebug()<<"[Window04] constructor closed..";
}

Window04::~Window04() {
	// TODO Auto-generated destructor stub
	qDebug()<<"[Window04] destructor is called..";

	delete drawingArea;
		//QGraphicsView * view;
	delete scene;
	delete txtDebug;
	delete scrollArea;
	delete txtPos;

	qDebug()<<"[Window04] destructor is close..";
}

void Window04::closeEvent(QCloseEvent *event)
{
	qDebug()<<"-----------[Window04] close is called..";
	this->~Window04();

}

void Window04::printPos(const QString & prt)
{
	txtPos->setText(prt);

}

void Window04::shapeChanged()
{
	qDebug()<<"Shape Changed is called..";
	QPushButton * tmp = (QPushButton *)sender();
	if(tmp->objectName() == "btnEllipse"){
		qDebug()<<"Btn Ellipse is called.";
		scene->setMode(0);
	}
	else if(tmp->objectName() == "btnLine"){
		qDebug()<<"Btn Line is called..";
		scene->setMode(1);
	}
	else
		scene->setMode(-1);
}
