/*
 * XMLReadWriter.h
 *
 *  Created on: 2013. 12. 6.
 *      Author: jiwon
 */

#ifndef XMLREADWRITER_H_
#define XMLREADWRITER_H_
#include <QXmlStreamReader>
#include <QVector>
#include <QFile>
#include <QStringList>
#include "Holding.h"
#include "RnavAid.h"

class XMLReadWriter {
public:
	XMLReadWriter();
	virtual ~XMLReadWriter();
	QStringList openFile();
	void parsingElement(QFile & file);
	void parseHolding();
	void parsingRnavAidElement(QFile & file);
	void parseRnavAid();
	void sortHolding();
	void sortRnav();
	void xmlWrite();
	void xmlWrite(QFile & file, QVector<Holding> holding_list);
	void xmlWrite(QFile & file, QVector<RnavAid> rnav_list);

private:

	int opentype; //0 - holding, 1 - rnav
	QXmlStreamReader * reader;
	QXmlStreamWriter * writer;
	QString fileName;
	QString errorMessage;
	QVector<Holding> hld_list;
	QVector<Holding> tmp_hld_list;

	QVector<RnavAid> rnav_list;
	//DbManager * dbManager;

};

#endif /* XMLREADWRITER_H_ */
