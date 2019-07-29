#ifndef NODEINFO_H
#define NODEINFO_H

#include <QtGui/QDialog>
#include "ui_NodeInfo.h"

class NodeInfo : public QDialog,public Ui::NodeInfoClass
{
    Q_OBJECT

public:
    NodeInfo(QWidget *parent = 0);
    ~NodeInfo();
    void setInfo(QString name, qreal x, qreal y);
    void setPos(qreal x, qreal y);
private:
     QString name;
     qreal x;
     qreal y;
};

#endif // NODEINFO_H
