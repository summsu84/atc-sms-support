/*
 * Style.h
 *
 *  Created on: 2012. 10. 30.
 *      Author: jiwon
 */

#ifndef STYLE_H_
#define STYLE_H_

#include <QString>
#include "Function.h"

enum{
	NORMAL = 1,
	HIGHLIGHT = 2
};

class Style {
public:
	Style();
	virtual ~Style();

	//inline
	inline void setStyleId(QString styleId){ this->styleId = styleId;}
	inline void setLineColor(QString lineColor){ this->lineColor = lineColor;}
	inline void setLineWidth(QString lineWidth){ this->lineWidth = lineWidth;}
	inline void setPolyColor(QString polyColor){ this->polyColor = polyColor;}
	inline void setType(int type){ this->type = type;}
	inline void setLineType(LineType lineType){ this->lineType = lineType;}
	inline void setFillPattern(FillPattern fillPattern){ this->fillPattern = fillPattern;}
	inline void setFontName(QString fontName){ this->fontName = fontName;}
	inline void setFontSize(QString fontSize){ this->fontSize = fontSize;}
	inline void setFontColor(QString fontColor){ this->fontColor = fontColor;}
	inline void setSymbol(QString symbol){ this->symbol = symbol;}

	inline QString getStyleId(){ return this->styleId;}
	//inline QString getLineColor(){ return this->lineColor;}
	//inline QString getPolyColor(){ return this->polyColor;}
	QString getPolyColor();
	QString getLineColor();
	inline QString getLineWidth(){ return this->lineWidth;}

	inline int getType(){ return this->type;}
	inline LineType getLineType(){ return this->lineType;}
	inline FillPattern getFillPattern(){ return this->fillPattern;}
	inline QString getFontName(){ return this->fontName;}
	inline QString getFontSize(){ return this->fontSize;}
	inline QString getFontColor(){ return this->fontColor;}
	inline QString getSymbol(){ return this->symbol;}

	//void setStyleId(QString styleId){ this->styleId = styleId;}
	void printStyle();


private:
	QString styleId;
	QString lineWidth;
	QString lineColor;
	QString polyColor;
	QString fontName;
	QString fontSize;
	QString fontColor;
	QString symbol;

	LineType lineType;
	FillPattern fillPattern;
	int type;				//normal or highlight
};

#endif /* STYLE_H_ */
