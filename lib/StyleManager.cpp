/*
 * StyleManager.cpp
 *
 *  Created on: 2012. 10. 30.
 *      Author: jiwon
 */

#include "StyleManager.h"
#include <QDebug>>
#include <QXmlStreamAttributes>
#include <QStringList>

StyleManager * StyleManager::pInstance = NULL;

StyleManager::StyleManager() {
	// TODO Auto-generated constructor stub

}

StyleManager::~StyleManager() {
	// TODO Auto-generated destructor stub
}

void StyleManager::processXmlParsing(QFile & file)
{
	//qDebug()<<"[StyleManager] ProcessXmlParsing is called..";
	reader = new QXmlStreamReader(&file);

	while(!reader->atEnd() && !reader->hasError())
	{
		//Read next Element;
		QXmlStreamReader::TokenType token = reader->readNext();
		if(token == QXmlStreamReader::StartDocument){
			continue;
		}
		if(token == QXmlStreamReader::StartElement){
			//qDebug()<<"startElement";
			if(reader->name() == "StyleMap"){
				this->parseStyleMap();
			}else if(reader->name() == "Style"){
				this->parseStyle();
			}
		}
	}
	setType();
}

void StyleManager::parseStyleMap()
{
	//qDebug()<<"[StyleManager] parseTyleMap is called..";

	QString styleMapId;
	QString nUrl;
	QString hUrl;
	if(reader->tokenType() != QXmlStreamReader::StartElement && reader->name() == "StyleMap"){
		return;
	}

	//attribute
	QXmlStreamAttributes attributes = reader->attributes();
	if(attributes.hasAttribute("id")){
		styleMapId = attributes.value("id").toString();
	}

	//StyleMap의 normal과 hightlight의 url을 검출한다..
	while(!(reader->tokenType() == QXmlStreamReader::EndElement && reader->name() == "StyleMap"))
	{
		if(reader->tokenType() == QXmlStreamReader::StartElement){
			if(reader->name() == "key"){
				QString key = reader->readElementText();
				if(key == "normal"){
					//normal url
					reader->readNextStartElement();
					nUrl = reader->readElementText();
					nUrl.remove(0,1);
				}else if(key == "highlight"){
					//highlight url
					reader->readNextStartElement();
					hUrl = reader->readElementText();
					hUrl.remove(0,1);
				}
			}
		}
		reader->readNext();
	}
	//normalr url과 highlight url을 stylemap id와 맵핑 시킨다..
	QStringList styles;
	styles.append(nUrl);
	styles.append(hUrl);
	styleMap.insert(styleMapId, styles);
	//test
	mapIdList.append(styleMapId);

}

void StyleManager::parseStyle()
{
	//qDebug()<<"[StyleManager] parseStyle is called..";
	Style style;
	QString lineColor = "";
	QString lineWidth = "1";
	QString polyColor = "";
	QString styleId   = "";
	if(reader->tokenType() != QXmlStreamReader::StartElement && reader->name() == "Style"){
		return;
	}

	//attribute

	QXmlStreamAttributes attributes = reader->attributes();
	if(attributes.hasAttribute("id")){
		styleId = attributes.value("id").toString();
	}

	//LineStyle과 PolyStyle을 검출한다..
	while(!(reader->tokenType() == QXmlStreamReader::EndElement && reader->name() == "Style"))
	{
		if(reader->tokenType() == QXmlStreamReader::StartElement){
			if(reader->name() == "LineStyle"){
				while(!(reader->tokenType() == QXmlStreamReader::EndElement && reader->name() == "LineStyle")){
					//LineStyle의 color와 width를 구한다..
					if(reader->tokenType() == QXmlStreamReader::StartElement){
						if(reader->name() == "color"){
							//color
							lineColor = reader->readElementText();
							//qDebug()<<"LineColor : "<<lineColor;
						}else if(reader->name() == "width"){
							//width
							lineWidth = reader->readElementText();
						}
					}
					reader->readNext();
				}
			}else if(reader->name() == "PolyStyle"){
				while(!(reader->tokenType() == QXmlStreamReader::EndElement && reader->name() == "PolyStyle")){
					//PolyStyle의 color를 구한다..
					if(reader->tokenType() == QXmlStreamReader::StartElement){
						if(reader->name() == "color"){
							//color
							polyColor = reader->readElementText();
						}
					}
					reader->readNext();
				}
			}
		}
		reader->readNext();
	}

	style.setStyleId(styleId);
	style.setLineColor(lineColor);
	style.setLineWidth(lineWidth);
	style.setPolyColor(polyColor);
	styleList.append(style);
}

void StyleManager::setType()
{
	//qDebug()<<"setType is called..";
	//setNormal..
	//qDebug()<<"list : "<<styleMap.values();
	QList<QStringList> typeList = styleMap.values();

	//setnormal
	for(int i = 0 ; i < typeList.count(); i++){
		QStringList tmpList = (QStringList)typeList.value(i);
		QString normalUrl = tmpList.value(0);
		QString highUrl = tmpList.value(1);

		for(int j = 0 ; j < styleList.count(); j++){
			//qDebug()<<"StyleID : "<<styleList.value(j).getStyleId()<<", normal : "<<normalUrl<<", high : "<<highUrl;
			Style style = (Style)styleList.value(j);

			if(style.getStyleId().trimmed() == normalUrl.trimmed()){
				//set normal type
				//qDebug()<<"style Id : "<<styleList.value(j).getStyleId()<<", normalUrl : "<<normalUrl;
				styleList[j].setType(NORMAL);
				break;
			}
		}
		for(int j = 0 ; j < styleList.count(); j++){
			//qDebug()<<"StyleID : "<<styleList.value(j).getStyleId()<<", normal : "<<normalUrl<<", high : "<<highUrl;
			Style style = (Style)styleList.value(j);
			if(style.getStyleId().trimmed() == highUrl.trimmed()){
				//set highlight type
				styleList[j].setType(HIGHLIGHT);
				break;
			}
		}
	}
	for(int i = 0 ; i < styleList.count(); i++){
		Style style = (Style)styleList.value(i);
		//qDebug()<<"Style Id : "<<style.getStyleId()<<"type : "<<style.getType()<<", line Color : "<<style.getLineColor()<<", line width : "<<style.getLineWidth();
	}
}


void StyleManager::test()
{
	//test..
	//qDebug()<<"[StyleManager] test()";
	for(int i = 0 ; i < mapIdList.count(); i++){
		QString mapId = (QString)mapIdList.value(i);
		//qDebug()<<"ID : "<<mapId<<", normal Url "<<styleMap[mapId].value(0);

	}

	//qDebug()<<"find test stylelist size : "<<styleMap.count();


	/*for(int i = 0 ; i < styleList.count(); i++){
		Style style = (Style)styleList.value(i);
		qDebug()<<"Style Id : "<<style.getStyleId()<<", line Color : "<<style.getLineColor()<<", line width : "<<style.getLineWidth();
	}*/

	for(int i = 0 ; i < mapIdList.count(); i++){
		QString mapId = (QString)mapIdList.value(i);

		Style style = this->findNormalStyle4Style(mapId);

		//qDebug()<<"Normal Style ID : "<<style.getStyleId()<<"line Color : "<<style.getLineColor();

	}

	for(int i = 0 ; i < mapIdList.count(); i++){
		QString mapId = (QString)mapIdList.value(i);

		Style style = this->findHighlightlStyle4Style(mapId);

		//qDebug()<<"High Style ID : "<<style.getStyleId()<<"line Color : "<<style.getLineColor();

	}
}

Style StyleManager::findNormalStyle4Style(QString styleMapId)
{
	//normal type의 style 객체를 리턴 한다..
	//qDebug()<<"[FindNormalStyle4Style] start.. "<<styleMapId;
	QString normalUrl = styleMap[styleMapId].value(0);
	Style style;
	bool isFound = false; 			//true : find, false : noFound
	for(int i = 0 ; i < styleList.count(); i++){
		Style tmpStyle = (Style)styleList.value(i);
		//qDebug()<<"tmpStyleId : "<<tmpStyle.getStyleId()<<"type : "<<tmpStyle.getType()<<", normalId : "<<normalUrl;
		if(tmpStyle.getStyleId() == normalUrl && tmpStyle.getType() == NORMAL){
			//qDebug()<<"Found..";
			style = tmpStyle;
			isFound = true;
			break;
		}
	}

	return style;
}

QString StyleManager::findNormalStyle4String(QString styleMapId)
{
	//normal type의 style 객체를 리턴 한다..

	QString normalUrl = styleMap[styleMapId].value(0);

	return normalUrl;
}

Style StyleManager::findHighlightlStyle4Style(QString styleMapId)
{
	//normal type의 style 객체를 리턴 한다..

	QString highUrl = styleMap[styleMapId].value(1);
	Style style;
	bool isFound = false; 			//true : find, false : noFound
	for(int i = 0 ; i < styleList.count(); i++){
		Style tmpStyle = (Style)styleList.value(i);
		if(tmpStyle.getStyleId() == highUrl && tmpStyle.getType() == HIGHLIGHT){
			style = tmpStyle;
			isFound = true;
			break;
		}
	}

	return style;
}

QString StyleManager::findHighlightStyle4String(QString styleMapId)
{
	//normal type의 style 객체를 리턴 한다..

	QString highUrl = styleMap[styleMapId].value(0);

	return highUrl;
}

void StyleManager::printStyles()
{
	for(int i = 0 ; i < styleList.count(); i++)
	{
		Style style = (Style)styleList.value(i);
		style.printStyle();
	}
}
