/*
 * Style.cpp
 *
 *  Created on: 2012. 10. 30.
 *      Author: jiwon
 */

#include "Style.h"
#include <QDebug>

Style::Style() {
	// TODO Auto-generated constructor stub
	styleId		= "";
	lineWidth	= "1";
	lineColor	= "";
	polyColor	= "";
	fontName		= "";
	fontSize		= "";
	fontColor	= "";
	symbol		= "";
	lineType		= SOLID;
	fillPattern	= SOLID_F;
	type			= NORMAL;
}

Style::~Style() {
	// TODO Auto-generated destructor stub
}

void Style::printStyle()
{
	QString print = "Id :";
	print.append(styleId);
	print.append(", Line Color : ");
	print.append(lineColor);
	print.append(", Line Width : ");
	print.append(lineWidth);
	print.append(", Poly Color : ");
	print.append(polyColor);

	//qDebug()<<print;
}

QString Style::getLineColor()
{
	if(this->lineColor == ""){
		return "";
	}
	//A. R. G. B 형태를 두자리씩 끊어 16진수코드를 10진수로 변환한다..
	//해당 컬러타입은 ABGR형태로 알파 다음 블루채널이 들어오고, 그 다음 그린채널, 맨 뒤 2바이트가 레드를 나타낸다..
	QString alphaC	= this->lineColor.mid(0,2);
	QString blueC		= this->lineColor.mid(2,2);
	QString greenC	= this->lineColor.mid(4,2);
	QString redC	= this->lineColor.mid(6,2);
	bool ok;

	//qDebug()<<"[STYLE] getLineColor..start.... color : "<<alphaC<<redC<<greenC<<blueC;

	int intRed = redC.toInt(&ok, 16);
	int intGreen = greenC.toInt(&ok, 16);
	int intBlue = blueC.toInt(&ok, 16);

	QString convertedColor = QString().setNum(intRed, 10);
	convertedColor.append(',');
	convertedColor.append(QString().setNum(intGreen, 10));
	convertedColor.append(',');
	convertedColor.append(QString().setNum(intBlue, 10));

	//qDebug()<<"Result : "<<convertedColor;

	return convertedColor;

}



QString Style::getPolyColor()
{
	//A. R. G. B 형태를 두자리씩 끊어 16진수코드를 10진수로 변환한다..
	//해당 컬러타입은 ABGR형태로 알파 다음 블루채널이 들어오고, 그 다음 그린채널, 맨 뒤 2바이트가 레드를 나타낸다..
	if(this->polyColor == ""){
		return "";
	}

	QString alphaC	= this->polyColor.mid(0,2);
	QString blueC		= this->polyColor.mid(2,2);
	QString greenC	= this->polyColor.mid(4,2);
	QString redC	= this->polyColor.mid(6,2);
	bool ok;

	//qDebug()<<"[STYLE] getPolyColor..start.... color : "<<alphaC<<redC<<greenC<<blueC;

	int intRed = redC.toInt(&ok, 16);
	int intGreen = greenC.toInt(&ok, 16);
	int intBlue = blueC.toInt(&ok, 16);

	QString convertedColor = QString().setNum(intRed, 10);
	convertedColor.append(',');
	convertedColor.append(QString().setNum(intGreen, 10));
	convertedColor.append(',');
	convertedColor.append(QString().setNum(intBlue, 10));

	//qDebug()<<"Result : "<<convertedColor;

	return convertedColor;

}
