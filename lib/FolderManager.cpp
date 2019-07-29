/*
 * FolderManager.cpp
 *
 *  Created on: 2012. 11. 1.
 *      Author: jiwon
 */

#include "FolderManager.h"
#include <QDebug>
#include <QMessageBox>
#include <QObject>

FolderManager * FolderManager::pInstance = NULL;

FolderManager::FolderManager() {
	// TODO Auto-generated constructor stu	b
	mapfilename		= "";
	nfilename		= "";
	cfilename		= "";
	layerLevel		= LAYER_LEVEL_1;
	initPointer();
	clearInfo();
}

FolderManager::~FolderManager() {
	// TODO Auto-generated destructor stub
	delete reader;
	delete openedFile;
}

void FolderManager::initPointer()
{
	reader = 0;
	openedFile = 0;
}

void FolderManager::setLayerLevel(int _layerLevel)
{
	qDebug()<<"[FolderManager]setLayerLevel start.. "<<_layerLevel;
	layerLevel = _layerLevel;

}


//변수 초기화함..
void FolderManager::clearInfo()
{
	qDebug()<<"[ClearInfo] start...";
	if(reader != 0){
		qDebug()<<"delete reader..";
		delete reader;

	}
	if(openedFile != 0)
	{
		qDebug()<<"delete openedFile..";
		delete openedFile;
		qDebug()<<"after delete openedFile..";
	}

	qDebug()<<"after delete pointers..";
	reader 			= 0;
	openedFile 		= 0;
	idNum 			= 1;
	mlayer 			= "";
	mapfilename		= "";
	nfilename		= "";
	convertDataInfoList.clear();
	layerNames.clear();
	qDebug()<<"[ClearInfo] stop...";
}


/*
 * 컨버터 버튼 클릭시 kml 형태의 파일을 map 형태의 파일론 변환한다.
 * 좌표 규칙은 도분초 형태로 변환한다..
 */
//실제로 컨버트 작업을 한다..
void FolderManager::convertMapFile()
{
	//스타일을 구하기 위해 파일을 다시연다..
	openFile();
	StyleManager::getInstance().processXmlParsing(*openedFile);
	StyleManager::getInstance().printStyles();


	QString nLayer = "";	//다음  레이어 정보
	QString cLayer;	//현재 레이어 정보


	//파일명을 확인한다..

	if(process_mode == FILE_MODE){
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(0, QObject::tr("Question"),
				QObject::tr("The file name is %1.map \n Do you really want to continue? ").arg(mapfilename), QMessageBox::Ok | QMessageBox::Cancel);
		if (reply == QMessageBox::Cancel) {
			this->clearInfo();
			//txtTarget->setText("");
			return;
		}

		QFile * file = new QFile(nfilename);
		if(!file->open(QIODevice::WriteOnly)){
			qDebug()<<"Cannot open file for writing !"<<file->errorString();

			return;
		}

		QTextStream out(file);

		//bool isFirst = true;
		//CovertDataInfo 객체 리스트를 한개씩 읽어서 좌표를 새롭게 변환한다..
		for(int i = 0 ; i < convertDataInfoList.count(); i++)
		{
			ConvertDataInfo convertDataInfo = (ConvertDataInfo)convertDataInfoList.value(i);
			//for Debug
			QString fullInfo = convertDataInfo.getLayer();
			fullInfo.append(", Id : ");
			fullInfo.append(convertDataInfo.getId());
			fullInfo.append(", Coordinates: ");
			//fullInfo.append(convertDataInfo.getCoordinate().trimmed());

			//map데이터 형식으로 lat lon을 변환한다..
			processLatLon(convertDataInfo);
			//qDebug()<<"Full Info : "<<fullInfo;
			for(int j = 0 ; j < convertDataInfo.getLatList().count(); j++){
				Lon lon = (Lon)convertDataInfo.getLonList().value(j);
				Lat lat = (Lat)convertDataInfo.getLatList().value(j);
				QString lonlat = lon.printMapValues();
				lonlat.append(" / ");
				lonlat.append(lat.printMapValues());
				//qDebug()<<"-- "<<lonlat;
			}
			cLayer = convertDataInfo.getLayer();
			nLayer = convertDataInfoList.value(i+1).getLayer();
			makeMapFile(cLayer, nLayer, out, convertDataInfo);
			convertDataInfoList.replace(i, convertDataInfo);
		}
		delete file;
	}
	else if(process_mode == DB_MODE)
	{


		QFile * file = new QFile(nfilename);
		if(!file->open(QIODevice::WriteOnly)){
			qDebug()<<"Cannot open file for writing !"<<file->errorString();

			return;
		}

		QTextStream out(file);

		for(int i = 0 ; i < convertDataInfoList.count(); i++)
		{
			ConvertDataInfo convertDataInfo = (ConvertDataInfo)convertDataInfoList.value(i);
			//for Debug
			QString fullInfo = convertDataInfo.getLayer();
			fullInfo.append(", Id : ");
			fullInfo.append(convertDataInfo.getId());
			fullInfo.append(", Coordinates: ");
			//fullInfo.append(convertDataInfo.getCoordinate().trimmed());

			//map데이터 형식으로 lat lon을 변환한다..
			processLatLon(convertDataInfo);
			//qDebug()<<"Full Info : "<<fullInfo;
			for(int j = 0 ; j < convertDataInfo.getLatList().count(); j++){
				Lon lon = (Lon)convertDataInfo.getLonList().value(j);
				Lat lat = (Lat)convertDataInfo.getLatList().value(j);
				QString lonlat = lon.printMapValues();
				lonlat.append(" / ");
				lonlat.append(lat.printMapValues());
				//qDebug()<<"-- "<<lonlat;
			}
			cLayer = convertDataInfo.getLayer();
			nLayer = convertDataInfoList.value(i+1).getLayer();
			makeMapFile(cLayer, nLayer, out, convertDataInfo);
			convertDataInfoList.replace(i, convertDataInfo);
		}
		delete file;
	}
}

//target 이름을 설정한다..
void FolderManager::setTargetName(QString nFileName)
{
	qDebug()<<"[SetTargetName] nFileName start..: "<<nFileName;
	QStringList tmpFileName = cfilename.split("/");
	for(int i = 0 ; i < tmpFileName.count() ; i++){
		if(i == tmpFileName.count()-1){
			nfilename.append(nFileName);
			nfilename.append(".map");
			if(process_mode == DB_MODE){
				nfilename.append(".tmp");
			}
			continue;
		}
		nfilename.append(tmpFileName.value(i));
		nfilename.append('/');
	}
	//qDebug()<<"[SetTargetName] stop....new file name: "<<nfilename;
}

void FolderManager::makeMapFile(QString cLayer, QString nLayer, QTextStream & out, ConvertDataInfo &convertDataInfo)
{
	//Layer 생성하기 -> i번째 객체의 레이어와 i+1번째 객체의 레이어를 비교하여, 다를 경우 새로운 레이어이므로 "LAYER:LAYER명" 형식을 생성한다..
	QString layer = "LAYER:";
	convertDataInfo.findStyle();
	LineType lineType =  convertDataInfo.getStyle().getLineType();
	QString lineColor =  convertDataInfo.getStyle().getLineColor();
	QString lineWidth =  convertDataInfo.getStyle().getLineWidth();
	FillPattern fillPattern = convertDataInfo.getStyle().getFillPattern();
	QString fillColor = convertDataInfo.getStyle().getPolyColor();
	QString fontName  = convertDataInfo.getStyle().getFontName();
	QString fontSize  = convertDataInfo.getStyle().getFontSize();
	QString fontColor = convertDataInfo.getStyle().getFontColor();
	QString symbol    = convertDataInfo.getStyle().getSymbol();

	static bool isFirst = true;

	if(isFirst == true){
		layer.append(cLayer);				// 현재 레이어 이름 등록
		layerNames.append(cLayer);		// 레이어 이름을 관리하기 위해서 저장한다..
		layer.append(':');
		switch(lineType){
			case SOLID:
			{
				layer.append('1');
				break;
			}
			case DASHED:
			{
				layer.append('2');
				break;
			}
			case DOTTED:
			{
				layer.append('3');
			}
		}
		layer.append(':');
		layer.append(lineWidth);
		layer.append(':');
		layer.append(lineColor);
		layer.append(':');
		switch(fillPattern){
			case SOLID_F:
			{
				layer.append('1');
				break;
			}
			case DOTTED_F:
			{
				layer.append('2');
				break;
			}
			case HORIZONTALLINE:
			{
				layer.append('3');
				break;
			}
			case VERTICALLINE:
			{
				layer.append('4');
				break;
			}
			case CROSSLINE:
			{
				layer.append('5');
				break;
			}
			case SLASHED:
			{
				layer.append('6');
				break;
			}
			case REVERSESLASHED:
			{
				layer.append('7');
				break;
			}
			case CROSSSLASHED:
			{
				layer.append('8');
				break;
			}
		}
		layer.append(':');
		layer.append(fillColor);
		layer.append(':');
		layer.append(fontName);
		layer.append(':');
		layer.append(fontSize);;
		layer.append(':');
		layer.append(fontColor);
		layer.append(':');
		layer.append(symbol);;

		out<<layer<<endl;
		isFirst = false;
	}
	convertDataInfo.makeMapFile(out);

	if(cLayer != nLayer)
	{
		out<<"ENDLAYER"<<endl;
		isFirst = true;
	}

	//test//
	//convertDataInfo.makeMapFile();

}


void FolderManager::openFile()
{
	//kml파일을 읽어온다..
	openedFile = new QFile(cfilename);
	if(!openedFile->open(QIODevice::ReadWrite | QIODevice::Text)){
		qDebug()<<"[FolderManager] openFile().. Cannot open the file";
		return;
	}
}


int FolderManager::processLayerParsing(QString kmlfileName)
{
	qDebug()<<"ProcessLayerParsing is called..layer level :"<<this->layerLevel;
	clearInfo();
	cfilename = kmlfileName;
	openFile();
	int layer = LAYER_LEVEL_0;
	reader = new QXmlStreamReader(openedFile);

	bool isFirst_1 = true;
	bool isFirst_2 = true;

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
			if(reader->name() == "Folder"){
				qDebug()<<"Found Frist Folder!..";
				layer = LAYER_LEVEL_1;
				while(!(reader->tokenType() == QXmlStreamReader::EndElement && reader->name() == "Folder"))
				{
					//첫번째 Folder 레이어의 name 엘리먼트를 검출한다.
					if(isFirst_1 == true){
						reader->readNextStartElement();
						isFirst_1 = false;
					}
					if(reader->tokenType() == QXmlStreamReader::StartElement){
						qDebug()<<"insdie start Element of first folder ... "<<reader->name();
						if(reader->name() == "Folder")
						{
							qDebug()<<"Found Second Folder!..";
							layer = LAYER_LEVEL_2;
							while(!(reader->tokenType() == QXmlStreamReader::EndElement && reader->name() == "Folder"))
							{

								//첫번째 Folder 레이어의 name 엘리먼트를 검출한다.
								if(isFirst_2 == true){
									reader->readNextStartElement();
									isFirst_2 = false;
								}
								if(reader->tokenType() == QXmlStreamReader::StartElement){
									qDebug()<<"insdie start Element of second folder ... "<<reader->name();
									if(reader->name() == "Folder")
									{
										qDebug()<<"Found Third Folder!..";
										layer = LAYER_LEVEL_3;
										return layer;
									}
								}
								reader->readNext();
							}
							return layer;
						}
					}
					reader->readNext();
				}
				return layer;
			}
		}
	}

	delete openedFile;
	return layer;
}


 /*
  * .kml 형태의 파일을 읽어서 (xml 구조 형태) 파싱한다.
  * mainwindow로 부터 kml 파일을 읽어와서 cfilename에 저장한다.
  */

QString FolderManager::processXmlParsing(QString kmlfileName)
{
	qDebug()<<"ProcessXmlParsing is called..layer level :"<<this->layerLevel;
	clearInfo();
	cfilename = kmlfileName;
	openFile();
	firstFolderName = true;

	reader = new QXmlStreamReader(openedFile);

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
			if(reader->name() == "Folder"){
				this->parseFirstFolder();
			}

		}
	}

	delete openedFile;
	//실제로 맵파일을 생성한다..
	setTargetName(mapfilename);
	this->convertMapFile();
	//delete reader;
	return nfilename;
}

 /*
  * 첫번째 Folder 엘리먼트 찾고, 해당 엘리먼트의 자식 엘리먼트인 name의 값을 검출한다.
  * 첫번째 Folder 엘리먼트를 찾았을 때, 아래 함수 parseFirstFolder() 함수를 호출하게 된다.
  * 첫번째 해당 함수는 name엘리먼트를 검출하여, 파일명으로 생성하고, 두번째 Folder 엘리먼트를 검출하면, parseSecondFolder함수를 호출한다..
  */


void FolderManager::parseFirstFolder()
{
	qDebug()<<"parseFirstFolder is called..";

	bool isFirst = true;			//true 일경우, name을 검출하기 위해서 readNextStartElement() 함수를 호출 한다..

	if(reader->tokenType() != QXmlStreamReader::StartElement && reader->name() == "Folder"){
		return;
	}
	while(!(reader->tokenType() == QXmlStreamReader::EndElement && reader->name() == "Folder"))
	{
		//첫번째 Folder 레이어의 name 엘리먼트를 검출한다.
		if(isFirst == true){
			reader->readNextStartElement();
			isFirst = false;
		}
		if(reader->tokenType() == QXmlStreamReader::StartElement){
			if(process_mode == FILE_MODE){
				//MODE가 FILE MODE인 경우,, 모든 FOLDER를 파싱한다..
				if(reader->name() == "name"){
					QString folderName_level_1 = reader->readElementText();
					qDebug()<<"First Folder Name : "<<folderName_level_1;
					switch(layerLevel){
						case LAYER_LEVEL_1:
						{
							//레벨 1일때, 레이어 이름과 파일이름을 같게 한다..
							mlayer = folderName_level_1;
							mapfilename = folderName_level_1;
							break;
						}
						case LAYER_LEVEL_2:
							//SET LAYER NAME ACCORDING TO FOLDER NAME
						{
							mapfilename = folderName_level_1;
							//setTargetName(folderName_level_1);
							break;
						}
						case LAYER_LEVEL_3:
							break;
					}
				}else if(reader->name() == "Folder")
				{
					if(layerLevel == LAYER_LEVEL_2){
						idNum = 1;
					}
					this->parseSecondFolder();
				}else if(reader->name() == "Placemark")
				{
					//다음 엘리먼트가 Folder가 아니고 Placemark일 경우..
					qDebug()<<"parsePlaceMark is called in First Folder..";
					this->parsePlaceMark();
				}else{
					qDebug()<<"Not found proper tag name.."<<reader->name();
				}
			}
			else if(process_mode == DB_MODE){
				//MODE가 DB MODE인 경우, 첫번째 폴더의 name만 검출 하고 Placemakr로 넘어간다..
				//단, name을 검출 할때 처음 파싱한 Folder의 name만 검출해야 하므로 이 후의 name에 대해서는 skip 한다..

				if(reader->name() == "name" && firstFolderName == true){
					QString folderName_level_1 = reader->readElementText();
					qDebug()<<"----------------First Folder Name : "<<folderName_level_1;
						//레벨 1일때, 레이어 이름과 파일이름을 같게 한다..
						mlayer = folderName_level_1;
						mapfilename = folderName_level_1;
						firstFolderName = false;

				}else if(reader->name() == "Placemark")
				{
					//다음 엘리먼트가 Folder가 아니고 Placemark일 경우..
					qDebug()<<"parsePlaceMark is called in First Folder..";
					this->parsePlaceMark();
				}else{
					qDebug()<<"Not found proper tag name.."<<reader->name();
				}
			}

		}
		reader->readNext();
	}

}

/*
 *두번째 folder 엘리먼트를 검출한다.
 *parseFirstFolder() 함수에서 두번째 Folder 엘리먼트 찾았을 때 해당 함수가 호출된다.
 *해당 함수는 name엘리먼트를 검출하여, 레이어 이름으로 사용하고 , 세번째 Folder 엘리먼트를 검출한다.
*/


void FolderManager::parseSecondFolder()
{
	qDebug()<<"ParseSecondFolder is called..";
	//현재 위치는 두번째 Folder 의 시작 엘리먼트이다.
	bool isFirst = true;								//true 일경우, name을 검출하기 위해서 readNextStartElement() 함수를 호출 한다..

	bool isSecondFolder = true;						//두번째 Folder 이하에 Document의 name이 존재 하는 경우..
	//QString layer = "";
	if(reader->tokenType() != QXmlStreamReader::StartElement && reader->name() == "Folder"){
		return;
	}

	while(!(reader->tokenType() == QXmlStreamReader::EndElement && reader->name() == "Folder"))
	{
		//두번째 Folder name을 검출하기 위한 명령
		if(isFirst == true){
			reader->readNextStartElement();
			isFirst = false;
		}
		if(reader->tokenType() == QXmlStreamReader::StartElement){
			if(reader->name() == "name" && isSecondFolder == true){
				QString folderName_level_2 = reader->readElementText();
				qDebug()<<"Second Folder Name : "<<folderName_level_2;

				switch(layerLevel){
					case LAYER_LEVEL_1:
					{
						break;
					}
					case LAYER_LEVEL_2:
						//SET LAYER NAME ACCORDING TO FOLDER NAME
					{
						mlayer = folderName_level_2;
						break;
					}
					case LAYER_LEVEL_3:
						//SET FILE NAME ACCORDING TO FOLDER NAME
						mapfilename = folderName_level_2;
						break;
				}
				isSecondFolder = false;

			}else if(reader->name() == "Folder")
			{
				//세번째 폴더를 찾았기 때문에 parseThirdFolder() 함수를 호출 한다..
				if(layerLevel == LAYER_LEVEL_3){
					idNum = 1;
				}
				this->parseThirdFolder();
			}else if(reader->name() == "Placemark")
			{
				//다음 엘리먼트의 이름이 Folder가 아니고 Placemark일경우..
				this->parsePlaceMark();
			}
		}
		reader->readNext();
	}

}

/*
 *parseSecondtFolder() 함수에서 세번째 Folder 엘리먼트 찾았을 때 해당 함수가 호출된다.
 *해당 함수는 name엘리먼트를 검출하여, 아이디 이름으로 사용하고 , Placemark 엘리먼트를 찾았을때 parsePlacemark함수를 호출 한다.
*/

void FolderManager::parseThirdFolder()
{
	//qDebug()<<"ParseThirdFolder is called..";
	bool isFirst = true;
	bool thirdFolderName = false;		//세번째 폴더 이름
	//QString layer;
	//현재 위치는 세번째 Folder 의 시작 엘리먼트이다..
	if(reader->tokenType() != QXmlStreamReader::StartElement && reader->name() == "Folder"){
		return;
	}

	while(!(reader->tokenType() == QXmlStreamReader::EndElement && reader->name() == "Folder"))
	{
		//Folder

		if(isFirst == true){
			reader->readNextStartElement();
			isFirst = false;
		}
		if(reader->tokenType() == QXmlStreamReader::StartElement){
			//ConvertDataInfo convertDataInfo;		//컨버터 객체 선언
			if(reader->name() == "name" && thirdFolderName == false){
				QString folderName_level_3 = reader->readElementText();
				qDebug()<<"Level 3 Folder Name : "<<folderName_level_3;
				switch(layerLevel){
					case LAYER_LEVEL_1:
					case LAYER_LEVEL_2:
					{
						break;
					}
					case LAYER_LEVEL_3:
					{
						//layer = reader->readElementText();
						mlayer = folderName_level_3;
						//qDebug()<<"Third Folder Name : "<<mlayer;
						thirdFolderName = true;
						break;
					}
				}

			}else if(reader->name() == "Placemark")
			{
				this->parsePlaceMark();
			}
		}
		reader->readNext();
	}

	qDebug()<<"----------------After ThirdFolder : ";
}

/**
 * 태그 엘리먼트의 값이 PlaceMark 일 경우 해당 함수를 호출 하여, ID와 좌표를 검출 한다.
 */
void FolderManager::parsePlaceMark()
{
	//qDebug()<<"ParsePlaceMark is called..";
	//Placemark 엘리먼트만 파싱한다.
	//현재 위치는 Placemark 의 시작 엘리먼트이다..
	QString id;
	QString styleMapKey;
	//해당 함수에서는 우리가 이용하고자 하는 정보를 객체 (ConvertDataInfo)에 저장하는 기능을 한다..
	ConvertDataInfo convertDataInfo;
	if(reader->tokenType() != QXmlStreamReader::StartElement && reader->name() == "Placemark"){
		return;
	}
	//reader->readNext();
	while(!(reader->tokenType() == QXmlStreamReader::EndElement && reader->name() == "Placemark"))
	{
		//Placemark
		if(reader->tokenType() == QXmlStreamReader::StartElement){
			if(reader->name() == "name"){
				id = reader->readElementText();
				//test
				if(id == "notitle"){
					id = mlayer;
					id.append("_");
					id.append(QString().setNum(idNum, 10));
					idNum++;
					qDebug()<<"Layer : "<<mlayer<<"/ ID : "<<id;
				}
				//qDebug()<<"Place mark Name : "<<id;
			}else if(reader->name() == "styleUrl"){
				//added 20121030
				//스타일 시트를 적용 시키기 위한 키 값..
				styleMapKey = reader->readElementText();
				styleMapKey.remove(0,1);
				convertDataInfo.setStyleMapKey(styleMapKey);
			}
			else if(reader->name() == "Polygon" || reader->name() == "LineString")
			{
				if(reader->name() == "Polygon"){
					convertDataInfo.setType(POLYGON);
				}else if(reader->name() == "LineString"){
					convertDataInfo.setType(POLYLINE);
				}
				convertDataInfo.setIdLayer(mlayer,id);
				convertDataInfo.setCoordinate(ParseLatLon());
				convertDataInfoList.append(convertDataInfo);

			}
		}
		reader->readNext();
	}

}

/*
 * <coordinates> 엘리먼트를 찾았을 때, 해당 엘리먼트의 값(좌표 값)을 가져온다.
 * coordinates의 값은 하나의 스트링 형태로 구성되어 있기때문에 한번 값을 가져오면 또 다른 값을 가져올 필요가 업다. (반복문 필요 없음)
*/

QString FolderManager::ParseLatLon()
{
	//qDebug()<<"ParseLatLon";
	//qDebug()<<"ParsePlaceMark is called..";


	//Placemark 엘리먼트만 파싱한다.
	//현재 위치는 Placemark 의 시작 엘리먼트이다..

	QString lineType = reader->name().toString();
	QString coordinates;

	if(reader->tokenType() != QXmlStreamReader::StartElement && reader->name() == lineType){
		return "error";
	}

	while(!(reader->tokenType() == QXmlStreamReader::EndElement && reader->name() == lineType))
	{
		//qDebug()<<"name of tag : "<<reader->name();
		//qDebug()<<"line Type : "<<lineType;
		//Polygon or LineString
		if(reader->tokenType() == QXmlStreamReader::StartElement){
			if(reader->name() == "coordinates"){
				//coordinates는 여러개의 좌표 값들이 하나의 스트링 형태로 구성되어 있기때문에 스트링 값만 읽어온다..
				coordinates = reader->readElementText();
				if(lineType == "Polygon")
					qDebug()<<"Polygon Coordinates : "<<reader->readElementText();
				else if(lineType == "LineString")
					qDebug()<<"LineString Coordinates : "<<reader->readElementText();

				//processLatLon(coordinates, lat, lon);
			}
		}
		reader->readNext();
	}

	return coordinates;
}
/*
//target 이름을 설정한다..
void FolderManager::setTargetName(QString nFileName)
{
	qDebug()<<"[SetTargetName] nFileName : "<<nFileName;
	QStringList tmpFileName = txtSource->text().split("/");
	for(int i = 0 ; i < tmpFileName.count() ; i++){
		if(i == tmpFileName.count()-1){
			//QStringList nameList = tmpFileName[i].split(".");
			nfilename.append(nFileName);
			nfilename.append(".map");
			continue;
		}
		nfilename.append(tmpFileName.value(i));
		nfilename.append('/');
	}
	txtTarget->setText(nfilename);
}*/

//현재 읽은 라인을 바탕으로 Lon과 Lat를 처리한다.
void FolderManager::processLatLon(ConvertDataInfo & convertDataInfo){

	//qDebug()<<"ProcessLatLon is called..";
	QVector<Lat> latList;
	QVector<Lon> lonList;
	//QString coordinate = convertDataInfo.getCoordinate();

	//coordinates들을 좌표로 분류 하기..
	QStringList coordinates = convertDataInfo.getCoordinate().trimmed().split(" ");
	QString strLat;
	QString strLon;
	for(int i = 0 ; i < coordinates.count() ; i++){
		//,형태로 split한다..
		QStringList latlonList = coordinates[i].split(",");
		strLon = (QString)latlonList.value(0);
		strLat = (QString)latlonList.value(1);
		Lon lon;
		Lat lat;
		//Lon 컨버터
		lon = convertLon(strLon);
		//qDebug()<<"lon : "<<lon.getDegree()<<lon.getMin()<<lon.getSec()<<lon.getMilliSec();
		//Lat 컨버터
		lat = convertLat(strLat);
		latList.append(lat);
		lonList.append(lon);
	}
	convertDataInfo.setLatList(latList);
	convertDataInfo.setLonList(lonList);
}




// 해당 함수는 Lon정보 (경도)를 도,분,초, 밀리초로 변환하여 Lon 객체를 반환하는 함수 이다.

Lon FolderManager::convertLon(QString lon)
{
	Lon convertedLon;
	QString degree = getDegree(lon);
	QString min = getMin(lon);
	QString sec = getSec(lon);
	QString millsec = getMilliSec(lon);

	convertedLon.setDegree(degree);
	convertedLon.setMin(min);
	convertedLon.setSec(sec);
	convertedLon.setMilliSec(millsec);

	return convertedLon;
}



 // 해당 함수는 Lat정보 (위도)를 도,분,초, 밀리초로 변환하여 Lat 객체를 반환하는 함수 이다.

Lat FolderManager::convertLat(QString lat)
{
	Lat convertedLat;
	QString degree = getDegree(lat);
	QString min = getMin(lat);
	QString sec = getSec(lat);
	QString millsec = getMilliSec(lat);

	convertedLat.setDegree(degree);
	convertedLat.setMin(min);
	convertedLat.setSec(sec);
	convertedLat.setMilliSec(millsec);

	return convertedLat;
}




// var1은 최초 위도 경도 자료이며, var2는 해당 데이터에서 degree를 제거한 소수점 타입의 스트링 자료를 담고 있다.

QString FolderManager::getDegree(QString &var1)
{
	//정수와 소수점을 분리한다. 정수 부분은 degree 값으로, 소수점 부분은 0.xxx로 나타낸다..
	//original
	QStringList degreeList = var1.split(".");

	QString prefix = "0.";

	var1 = prefix.append(degreeList.value(1));

	return degreeList.value(0);

	/*QString degree;
	degree.setNum((int)var1);
	var1 = var1 - (int)var1;	//0.minsecmillisec 구조..
	return degree;*/
}

QString FolderManager::getMin(QString &var1)
{
	//original
	bool ok;
	double doubleMin = var1.toDouble(&ok) * 60;	//정수는 Min, 소수점 이하자리는 초와 밀리초로 구분한다. doubleMin은 "Minute.이하값" 형태의 값이다.
	int intMin = (int)doubleMin;
	QString min;

	if(intMin < 10){
		min = "0";
		min.append(QString().setNum(intMin,10));
	}else{
		min.setNum(intMin, 10);
	}

	//qDebug()<<"BeforMaking Min : "<<var1<<", DoubleMin : "<<QString::number(doubleMin, 'f', 6)<<" Min : "<<min;

	//Min을 제외한 소수점 숫자를 초 단위로 변환하기 위해서 doubleMin을 split한다..
	QString originMin;
	//originMin.setNum(doubleMin);
	originMin.setNum(doubleMin, 'f', 6);
	//qDebug()<<"FullValue of Min : "<<originMin;
	QStringList minList = originMin.split(".");

	QString prefix = "0.";

	var1 = prefix.append(minList.value(1));		//var1은 Min 데이터가 없는 소수점과 밀리초 형태의 스트링 형태의 소수점 타

	return min;

	//var1는 0.xx 형태로 여기에 60을 곱하여 int로 변환하면 min의 값을 얻을 수 있다.
	/*QString min;
	int intMin = (int)(var1 * 60);
			//(int)((var1-(int)var1)*60);
	min.setNum(intMin, 10);

	var1 = (var1 * 60) - intMin;		//0.secmillisec 구조..
	return min;*/
}

QString FolderManager::getSec(QString &var1)
{
	bool ok;
	double doubleSec = var1.toDouble(&ok) * 60;	//정수는 Sec, 소수점 이하자리는 밀리초로 구분한다.
	int intSec = (int)doubleSec;
	QString sec;
	if(intSec < 10){
		sec = "0";
		sec.append(QString().setNum(intSec,10));
	}else{
		sec.setNum(intSec, 10);
	}

	//qDebug()<<"BeforMaking Sec : "<<var1<<", DoubleSec : "<<QString::number(doubleSec, 'f', 6)<<" Sec : "<<sec;
	//Sec을 제외한 소수점 만들기.. doubleSec을 split하
	QString originSec;
	originSec.setNum(doubleSec);
//	qDebug()<<"FullValue of Sec : "<<originSec;
	QStringList secList = originSec.split(".");

	QString prefix = "0.";

	var1 = prefix.append(secList.value(1));		//var1은 Min 데이터가 없는 소수점과 밀리초 형태의 스트링 형태의 소수점 타

	return sec;

	//var1은 0.xxx로 초와 밀리세컨드를 구하기 위해서 *60을 해야한다..
	/*QString sec;
	int intSec = (int)(var1 * 60);
			//(int)((var1 - (int)var1)*60);
	//int intSec = (int)(((var1 - (int)var1) * 60 - min) * 60);
	var1 = (var*60) - intSsec;
	sec.setNum(intSec, 10);
	return sec;*/
}

QString FolderManager::getMilliSec(QString &var1)
{
	QStringList millisecList = var1.split(".");
	//qDebug()<<"FullMilliSec : "<<var1;
	return millisecList[1].left(2);
}
