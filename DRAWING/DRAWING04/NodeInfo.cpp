#include "NodeInfo.h"

NodeInfo::NodeInfo(QWidget *parent)
    : QDialog(parent)
{
	setupUi(this);
	name = "";
	x = 0;
	y = 0;
}

NodeInfo::~NodeInfo()
{

}


void NodeInfo::setInfo(QString name, qreal x, qreal y)
{
	this->name = name;
	this->x = x;
	this->y = y;
	txtName->setText(this->name);
	txtX->setText(QString().setNum(this->x));
	txtY->setText(QString().setNum(this->y));
}

void NodeInfo::setPos(qreal x, qreal y)
{
	txtX->setText(QString().setNum(x));
	txtY->setText(QString().setNum(y));
}
