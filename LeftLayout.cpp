/*
 * LeftLayout.cpp
 *
 *  Created on: 2012. 11. 1.
 *      Author: jiwon
 */

#include "LeftLayout.h"
#include <QDebug>
#include <QStringList>

LeftLayout::LeftLayout(QWidget *parent) : QTreeWidget(parent)
{
	// TODO Auto-generated constructor stub
	connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(test(QTreeWidgetItem *, int)));
	init();
	//test
	//genTreeMenu(QString("RKSI"), QStringList()<<"RKSI_RUNWAYS"<<"RKSI_BUILDING");
}

LeftLayout::~LeftLayout() {
	// TODO Auto-generated destructor stub
	qDebug()<<"[~LeftLayout] start..";
	QTreeWidgetItem * item = 0;
	item = topLevelItem(0);
	if(item != 0){
		qDebug()<<"LeftLayout - CurrentItem : "<<item->text(0);
		QList<QTreeWidgetItem *> childItemList = item->takeChildren();
		QTreeWidgetItem * childItem;
		foreach(childItem, childItemList){
			qDebug()<<"Child Items :"<<childItem->text(0);
		}
		takeTopLevelItem(0);
	}
}


void LeftLayout::init()
{
	qDebug()<<"test..";

	//Set QtreeWidget Column Header (FileName, Layer_name)
	QTreeWidgetItem * headerItem = new QTreeWidgetItem();
	headerItem->setText(0, QString("LAYER_NAME"));
	this->setHeaderItem(headerItem);
}

void LeftLayout::test(QTreeWidgetItem * item, int column)
{

}
/*
 * Mainwindow에서 파일을 읽었을 때, 파일 이름과 레이어를 genTreeMenu로 보낸다.
 */
void LeftLayout::genTreeMenu(QString fileName, QStringList layers)
{
	qDebug()<<"Layers name :"<<layers;
	QTreeWidgetItem * item = new QTreeWidgetItem();
	item->setText(0, fileName);	//Root

	for(int i = 0 ; i < layers.count(); i++){
		QString layerName = (QString)layers.value(i);
		QTreeWidgetItem * layer = new QTreeWidgetItem();
		layer->setText(0, layerName);

		item->addChild(layer);
	}
	this->addTopLevelItem(item);
}

void LeftLayout::removeItems()
{
	/*while(int nb = this->topLevelItemCount())
	{
		delete this->takeTopLevelItem(nb - 1);
	}*/
	QTreeWidgetItem * item = this->currentItem();
	int i = this->indexOfTopLevelItem(item);
	this->takeTopLevelItem(i);
	delete item;
}
