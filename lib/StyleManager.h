/*
 * StyleManager.h
 *
 *  Created on: 2012. 10. 30.
 *      Author: jiwon
 */

#ifndef STYLEMANAGER_H_
#define STYLEMANAGER_H_

#include <QMap>
#include <QFile>
#include <QXmlStreamReader>
#include <QVector>
#include <QStringList>
#include "Style.h"
#include "Function.h"

class Style;

class StyleManager {
public:
	//singltone
	static StyleManager & getInstance(){
		if(pInstance == NULL){
			pInstance = new StyleManager;
		}
		return * pInstance;
	}
	StyleManager();
	virtual ~StyleManager();
	void processXmlParsing(QFile & file);
	void parseStyleMap();
	void parseStyle();
	void setType();

	//styleMapId를 바탕으로 normal, highlight url 또는 style 객체를 가져온다..
	Style findNormalStyle4Style(QString styleMapId);
	QString findNormalStyle4String(QString styleMapId);
	Style findHighlightlStyle4Style(QString styleMapId);
	QString findHighlightStyle4String(QString styleMapId);
	void test();
	void printStyles();
private:
	static StyleManager * pInstance;

	QXmlStreamReader * reader;
	QMap<QString, QStringList>  styleMap;
	QVector<Style>	styleList;
	QStringList mapIdList;

};

#endif /* STYLEMANAGER_H_ */
