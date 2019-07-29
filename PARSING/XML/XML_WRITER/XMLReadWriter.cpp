/*
 * XMLReadWriter.cpp
 *
 *  Created on: 2013. 12. 6.
 *      Author: jiwon
 */

#include "XMLReadWriter.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>

XMLReadWriter::XMLReadWriter() {
	// TODO Auto-generated constructor stub

	opentype = 1;
	openFile();
}

XMLReadWriter::~XMLReadWriter() {
	// TODO Auto-generated destructor stub
}



//파일을 열고 헤더를 읽어온다..
QStringList XMLReadWriter::openFile()
{
	qDebug()<<"Open File is called..";
	QString dir = QDir::currentPath();
	QFileDialog fdialog;
	QStringList tmp;
	//fdialog.setFileMode(QFileDialog::ExistingFiles);



	fileName = fdialog.getOpenFileName(0, QString("Open the xml file"),
			dir, QString("xml file(*.xml)"));



	QFile file(fileName);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		errorMessage = QString(QObject::tr("Cannot read file %1:\n%2")
									.arg(file.fileName())
									.arg(file.errorString()));
		return tmp;
	}

	qDebug()<<"type : "<< opentype;
	if(opentype == 0){
		qDebug()<<"h1";
		parsingElement(file);
		file.close();
		for(int i = 0 ; i < hld_list.size() ; i++)
		{
			Holding holding = hld_list.at(i);
			holding.printHolding();
		}
		qDebug()<<"====================";

		sortHolding();

		xmlWrite();
	}else if(opentype == 1)
	{
		qDebug()<<"h2";
		parsingRnavAidElement(file);
		file.close();
		for(int i = 0 ; i < rnav_list.size() ; i++)
		{
			RnavAid rnav = rnav_list.at(i);
			rnav.printRnav();
		}
		qDebug()<<"====================";

		sortRnav();

		xmlWrite();
	}
	return tmp;
}


void XMLReadWriter::parsingElement(QFile &file)
{
	reader = new QXmlStreamReader(&file);

	while(!reader->atEnd() && !reader->hasError())
	{
		//Read next Element;
		QXmlStreamReader::TokenType token = reader->readNext();
		//qDebug()<<"Reader's Next = "<<reader->name();
		if(token == QXmlStreamReader::StartDocument){
			continue;
		}
		if(token == QXmlStreamReader::StartElement){
			//첫번째 Folder 태그를 검출한다..
			if(reader->name() == "Holding"){
				this->parseHolding();
			}

		}
	}
}


void XMLReadWriter::parseHolding()
{
	qDebug()<<"parseHolding is called..";

	bool isFirst = true;			//true 일경우, name을 검출하기 위해서 readNextStartElement() 함수를 호출 한다..

	if(reader->tokenType() != QXmlStreamReader::StartElement && reader->name() == "Holding"){
		return;
	}
	Holding hld;
	while(!(reader->tokenType() == QXmlStreamReader::EndElement && reader->name() == "Holding"))
	{
		//첫번째 Folder 레이어의 name 엘리먼트를 검출한다.
//		if(isFirst == true){
//			reader->readNextStartElement();
//			isFirst = false;
//		}
		if(reader->tokenType() == QXmlStreamReader::StartElement){
			if(reader->name() == "HoldingFixName" || reader->name() == "HolidngFixName"){
				hld.setHoldingFixName(reader->readElementText());
			}else if(reader->name() == "DupId" || reader->name() == "DupID")
			{
				hld.setDupId(reader->readElementText());
			}else if(reader->name() == "InboundTrack")
			{
				hld.setInboundTrack(reader->readElementText());
			}else if(reader->name() == "Direction")
			{
				hld.setDirection(reader->readElementText());
			}else if(reader->name() == "MaxIas")
			{
				hld.setMaxIas(reader->readElementText());
			}else if(reader->name() == "MinAlt")
			{
				hld.setMinAlt(reader->readElementText());
			}else if(reader->name() == "MaxAlt")
			{
				hld.setMaxAlt(reader->readElementText());
			}else if(reader->name() == "CpType")
			{
				hld.setCpType(reader->readElementText());
			}else if(reader->name() == "CpValue")
			{
				hld.setCpValue(reader->readElementText());
			}else if(reader->name() == "Type")
			{
				hld.setType(reader->readElementText());
			}else if(reader->name() == "ReferName")
			{
				hld.setReferName(reader->readElementText());
			}

		}
		reader->readNext();
	}
	hld_list.append(hld);

}

void XMLReadWriter::parsingRnavAidElement(QFile &file)
{
	reader = new QXmlStreamReader(&file);

	while(!reader->atEnd() && !reader->hasError())
	{
		//Read next Element;
		QXmlStreamReader::TokenType token = reader->readNext();
		//qDebug()<<"Reader's Next = "<<reader->name();
		if(token == QXmlStreamReader::StartDocument){
			continue;
		}
		if(token == QXmlStreamReader::StartElement){
			//첫번째 Folder 태그를 검출한다..
			if(reader->name() == "RnavAid"){
				this->parseRnavAid();
			}

		}
	}
}


void XMLReadWriter::parseRnavAid()
{
	qDebug()<<"parseRnav is called..";


	if(reader->tokenType() != QXmlStreamReader::StartElement && reader->name() == "RnavAid"){
		return;
	}
	RnavAid rnav;
	while(!(reader->tokenType() == QXmlStreamReader::EndElement && reader->name() == "RnavAid"))
	{
		//첫번째 Folder 레이어의 name 엘리먼트를 검출한다.
//		if(isFirst == true){
//			reader->readNextStartElement();
//			isFirst = false;
//		}
		if(reader->tokenType() == QXmlStreamReader::StartElement){
			if(reader->name() == "Name"){
				rnav.setName(reader->readElementText());
			}else if(reader->name() == "StationName")
			{
				rnav.setStationName(reader->readElementText());
			}else if(reader->name() == "Type")
			{
				rnav.setType(reader->readElementText());
			}else if(reader->name() == "Frequency")
			{
				rnav.setFrequency(reader->readElementText());
			}else if(reader->name() == "Lat")
			{
				rnav.setLat(reader->readElementText());
			}else if(reader->name() == "Lon")
			{
				rnav.setLon(reader->readElementText());
			}
		}
		reader->readNext();
	}
	rnav_list.append(rnav);

}

void XMLReadWriter::sortHolding()
{


	for(int i = 0 ; i < hld_list.size() ;i++){
		for(int j = 0 ; j < hld_list.size() -1 ; j++){
			Holding hld = hld_list.at(j);
			Holding hld2 = hld_list.at(j + 1);
			if(hld.getHoldingFixName().compare(hld2.getHoldingFixName()) > 0)
			{
				Holding tmp = hld;
				hld_list.replace(j, hld2);
				hld_list.replace(j+1, tmp);
			}
		}
	}

	for(int i = 0 ; i < hld_list.size() ; i++)
	{
		Holding holding = hld_list.at(i);
		holding.printHolding();
	}

}


void XMLReadWriter::sortRnav()
{


	for(int i = 0 ; i < rnav_list.size() ;i++){
		for(int j = 0 ; j < rnav_list.size() -1 ; j++){
			RnavAid hld = rnav_list.at(j);
			RnavAid hld2 = rnav_list.at(j + 1);
			if(hld.getName().compare(hld2.getName()) > 0)
			{
				RnavAid tmp = hld;
				rnav_list.replace(j, hld2);
				rnav_list.replace(j+1, tmp);
			}
		}
	}

	for(int i = 0 ; i < rnav_list.size() ; i++)
	{
		RnavAid holding = rnav_list.at(i);
		holding.printRnav();
	}

}

void XMLReadWriter::xmlWrite()
{
	// File create
	QFile file("/home/jiwon/holding_test.xml");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
		qDebug()<<"File open error! ";
		return ;
	}

	if(opentype == 0)
		xmlWrite(file, hld_list);
	else if(opentype == 1)
		xmlWrite(file, rnav_list);

	file.close();
}

void XMLReadWriter::xmlWrite(QFile & file, QVector<Holding> holding_list)
{
	qDebug()<<"XMLWriter::makeHolding function is called..";
	QXmlStreamWriter xml;
	xml.setAutoFormatting(true);
	xml.setDevice(&file);
	xml.writeStartDocument();

	//HoldingTable
	xml.writeStartElement("HoldingTable");

	for(int i = 0 ; i < holding_list.size() ; i++){

		Holding hd = holding_list.at(i);

		//Holding
		xml.writeStartElement("Holding");
		xml.writeTextElement("HoldingFixName", hd.getHoldingFixName());
		xml.writeTextElement("DupID", hd.getDupId());
		xml.writeTextElement("InboundTrack", hd.getInboundTrack());
		xml.writeTextElement("Direction", hd.getDirection());
		xml.writeTextElement("MaxIas", hd.getMaxIas());
		if(hd.getMinAlt().length() != 0){
			xml.writeTextElement("MinAlt", hd.getMinAlt());
		}
		if(hd.getMaxAlt().length() != 0){
			xml.writeTextElement("MaxAlt", hd.getMaxAlt());
		}
		xml.writeTextElement("CpType", hd.getCpType());
		xml.writeTextElement("CpValue", hd.getCpValue());
		xml.writeTextElement("Type", hd.getType());
		if(hd.getReferName().length() != 0)
			xml.writeTextElement("ReferName", hd.getReferName());

		//End of Holding
		xml.writeEndElement();
	}
	//End of HoldingTable
	xml.writeEndElement();
	xml.writeEndDocument();
}



void XMLReadWriter::xmlWrite(QFile & file, QVector<RnavAid> rnav_list)
{

	qDebug()<<"XMLWriter::makeRnavAid function is called..";
	QXmlStreamWriter xml;
	xml.setAutoFormatting(true);
	xml.setDevice(&file);
	xml.writeStartDocument();

	//RnavAidTable
	xml.writeStartElement("RnavAidTable");

	for(int i = 0 ; i < rnav_list.size() ; i++){

		RnavAid ra = rnav_list.at(i);

		//RnavAid
		xml.writeStartElement("RnavAid");
		xml.writeTextElement("Name", ra.getName());
		xml.writeTextElement("StationName", ra.getStationName());
		xml.writeTextElement("Type", ra.getType());				//20131202 NEW
		xml.writeTextElement("Frequency", ra.getFrequency());
		xml.writeTextElement("Lat", ra.getLat());
		xml.writeTextElement("Lon", ra.getLon());

		//End of RnavAid
		xml.writeEndElement();
	}
	//End of RnavAidTable
	xml.writeEndElement();
	xml.writeEndDocument();
}
