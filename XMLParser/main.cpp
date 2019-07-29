/*#include "xml_parser.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XML_Parser w;

    //login->setModal(true);
    QWidget * desktop = QApplication::desktop();
    QRect * rect = new QRect(desktop->geometry());

    QPoint * form = new QPoint(w.width(),w.height());
    int cx = ((rect->center().x()) - (form->x()/2));
    int cy = ((rect->center().y()) - (form->y()/2));

    qDebug()<<"Main test1 x" << form->x() <<"Main test1 y "<<form->y();
    qDebug()<<"Center x " <<((rect->center().x()) - (form->x()/2));
    qDebug()<<"Desktop Center y " << rect->center().y();
    qDebug()<<"Center y " <<((rect->center().y()) - (form->y()/2));
    w.move(cx, cy);
    w.show();
    return a.exec();
}*/
