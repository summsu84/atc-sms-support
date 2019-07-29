/*
 * LeftLayout.h
 *
 *  Created on: 2012. 11. 1.
 *      Author: jiwon
 */

#ifndef LEFTLAYOUT_H_
#define LEFTLAYOUT_H_

#include <QTreeWidget>

class LeftLayout : public QTreeWidget
{
public:
	LeftLayout(QWidget *parent = 0);
	virtual ~LeftLayout();
	void init();
	void removeItems();

public slots:
	void test(QTreeWidgetItem * item, int column);
	void genTreeMenu(QString fileName, QStringList layers);
private:


};

#endif /* LEFTLAYOUT_H_ */
