#include "rksiparser.h"
#include <QDebug>


RKSIParser::RKSIParser(QWidget *parent)
    : QWidget(parent)
{
	setupUi(this);
	//qDebug()<<"Test";

	connect(btnFilePath, SIGNAL(clicked()), this, SLOT(openMapFile()));
	connect(btnConvert, SIGNAL(clicked()), this, SLOT(convertMapFile()));
	connect(btnTest, SIGNAL(clicked()), this, SLOT(test()));
	//connect(rdoBatype_P,    SIGNAL(clicked()),                  this, SLOT(batypeChanged()));
	connect(rdoLevel_1, SIGNAL(clicked()), this, SLOT(setLayerLevel()));
	connect(rdoLevel_2, SIGNAL(clicked()), this, SLOT(setLayerLevel()));
	connect(rdoLevel_3, SIGNAL(clicked()), this, SLOT(setLayerLevel()));

	reader 		= 0;
	idNum 		= 1;
	openedFile	= 0;
	fileName		= "";
	layerLevel	= LAYER_LEVEL_1;

	rdoLevel_1->setChecked(true);
//	txtSource->setEnabled(false);
//	txtTarget->setEnabled(false);

}
RKSIParser::~RKSIParser()
{
	delete reader;
	delete openedFile;
}

void RKSIParser::test()
{
	qDebug()<<"test";
	//파일을 읽어 온다.
	openFile();
	StyleManager::getInstance().processXmlParsing(*openedFile);
	StyleManager::getInstance().test();
}


void RKSIParser::setLayerLevel()
{
	qDebug()<<"setLayerLevel is called..";
	if(rdoLevel_1->isChecked() == true){
		layerLevel = LAYER_LEVEL_1;
	}
	if(rdoLevel_2->isChecked() == true){
		layerLevel = LAYER_LEVEL_2;
	}
	if(rdoLevel_3->isChecked() == true){
		layerLevel = LAYER_LEVEL_3;
	}
	txtTarget->setText("");
	qDebug()<<"LayerLevel : "<<layerLevel;
}

/**
 * Open 클릭시 파일을 읽어 온다..
 */
void RKSIParser::openMapFile()
{
	if(rdoLevel_1->isChecked() == false && rdoLevel_2->isChecked() == false && rdoLevel_3->isChecked() == false)
	{
		QMessageBox::warning(this, tr("Error"), tr("Please select layer level.."));
		return;
	}

	txtSource->setText("");
	txtTarget->setText("");
	QString dir = QDir::currentPath();

	//test
	QFileDialog fdialog(this);
	//fdialog.setFileMode(QFileDialog::ExistingFiles);

	qDebug()<<"File Mode" <<fdialog.fileMode();
	cfilename = fdialog.getOpenFileName(this, tr("Open the dat file"),
			dir, tr("dat file(*.kml)"));
	//count = cfilename.count();

	if (!cfilename.isEmpty()) {
		qDebug() << "fileName : " << cfilename;
		txtSource->setText(cfilename);
	}
}

void RKSIParser::openFile()
{
	openedFile = new QFile(cfilename);
	if(!openedFile->open(QIODevice::ReadWrite | QIODevice::Text)){
		qDebug()<<"Cannot open the file";
		return;
	}
}
/*
 * 변수 초기화함..
 */
void RKSIParser::clearInfo()
{
	qDebug()<<"[ClearInfo] start...";
	if(reader != 0){
		delete reader;
		reader = 0;
	}
	if(openedFile != 0)
	{
		delete openedFile;
		openedFile = 0;
	}
	idNum 	= 1;
	mlayer 	= "";
	fileName	= "";
	convertDataInfoList.clear();
	txtTarget->setText("");
	cFileDir	= "";
	qDebug()<<"[ClearInfo] stop...";
}


/*
 * 컨버터 버튼 클릭시 kml 형태의 파일을 map 형태의 파일론 변환한다.
 * 좌표 규칙은 도분초 형태로 변환한다..
 */
void RKSIParser::convertMapFile()
{
	clearInfo();
	if(rdoLevel_1->isChecked() == false && rdoLevel_2->isChecked() == false && rdoLevel_3->isChecked() == false)
	{
		QMessageBox::warning(this, tr("Error"), tr("Please select layer level.."));
		return;
	}
	//파일을 읽어 온다.
	openFile();
	//읽어온 파일을 파싱을 수행한다..
	processXmlParsing(*openedFile);
	delete openedFile;
	openFile();
	StyleManager::getInstance().processXmlParsing(*openedFile);
	StyleManager::getInstance().printStyles();

	setTargetName(fileName);
	QString nLayer = "";	//다음  레이어 정보
	QString cLayer;	//현재 레이어 정보


	//파일명을 확인한다..
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, tr("Question"),
			tr("The file name is %1.map \n Do you really want to continue? ").arg(fileName), QMessageBox::Ok | QMessageBox::Cancel);
	if (reply == QMessageBox::Cancel) {
		this->clearInfo();
		//txtTarget->setText("");
		return;
	}
	//파싱 이 후 컨버팅 한다..
	QFile * file = new QFile(txtTarget->text());
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
		qDebug()<<"Full Info : "<<fullInfo;
		for(int j = 0 ; j < convertDataInfo.getLatList().count(); j++){
			Lon lon = (Lon)convertDataInfo.getLonList().value(j);
			Lat lat = (Lat)convertDataInfo.getLatList().value(j);
			QString lonlat = lon.printMapValues();
			lonlat.append(" / ");
			lonlat.append(lat.printMapValues());
			qDebug()<<"-- "<<lonlat;
		}
		cLayer = convertDataInfo.getLayer();
		nLayer = convertDataInfoList.value(i+1).getLayer();
		makeMapFile(cLayer, nLayer, out, convertDataInfo);
	}
	delete file;
}

void RKSIParser::makeMapFile(QString cLayer, QString nLayer, QTextStream & out, ConvertDataInfo &convertDataInfo)
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

}



/*
void RKSIParser::makeMapFile()
{
	qDebug()<<"makeMapFile is called..";
	QFile file(txtTarget->text());
	if(!file.open(QIODevice::WriteOnly)){
		qDebug()<<"Cannot open file for writing !";

		return;
	}

	QTextStream out(&file);
	for(int i = 0 ; i < convertDataInfoList.count() ; i++){
		ConvertDataInfo cdi = (ConvertDataInfo)convertDataInfoList.at(i);
		cdi.makeMapFile(out);
	}


}*/

/*
 * .kml 형태의 파일을 읽어서 (xml 구조 형태) 파싱한다.
 */
void RKSIParser::processXmlParsing(QFile & file)
{
	qDebug()<<"ProcessXmlParsing is called..";
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
			if(reader->name() == "Folder"){
				/*reader->readNextStartElement();
				isFirstFolder = false;
				if(reader->name() == "name"){
					qDebug()<<"Main Folder Name : "<<reader->readElementText();
				}*/
				this->parseFirstFolder();
			}

		}
	}
}
/*
 *첫번째 Folder 엘리먼트 찾고, 해당 엘리먼트의 자식 엘리먼트인 name의 값을 검출한다.
 *첫번째 Folder 엘리먼트를 찾았을 때, 아래 함수 parseFirstFolder() 함수를 호출하게 된다.
 *첫번째 해당 함수는 name엘리먼트를 검출하여, 파일명으로 생성하고, 두번째 Folder 엘리먼트를 검출하면, parseSecondFolder함수를 호출한다..
 */

void RKSIParser::parseFirstFolder()
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
			if(reader->name() == "name"){
				QString folderName_level_1 = reader->readElementText();
				//qDebug()<<"First Folder Name : "<<reader->readElementText();

				switch(layerLevel){
					case LAYER_LEVEL_1:
					{
						//레벨 1일때, 레이어 이름과 파일이름을 같게 한다..
						mlayer = folderName_level_1;
						fileName = folderName_level_1;
						//setTargetName(fileName);
						break;
					}
					case LAYER_LEVEL_2:
					{
						//레벨 2일때, 첫번째 폴더의 name 태그 값을 파일명으로 지정한다..
						fileName = folderName_level_1;
						//setTargetName(folderName_level_1);
						break;
					}
					case LAYER_LEVEL_3:
						break;
				}
			}else if(reader->name() == "Folder")
			{
				//Folder 태그를 만나면 두번째 Folder 부분을 파싱한다..
				if(layerLevel == LAYER_LEVEL_2){
					idNum = 1;
				}
				this->parseSecondFolder();
			}else if(reader->name() == "Placemark")
			{
				//다음 엘리먼트가 Folder가 아니고 Placemark일 경우..
				//this->parsePlaceMark();
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


void RKSIParser::parseSecondFolder()
{
	qDebug()<<"ParseSecondFolder is called..";
	//현재 위치는 두번째 Folder 의 시작 엘리먼트이다.
	bool isFirst = true;								//true 일경우, name을 검출하기 위해서 readNextStartElement() 함수를 호출 한다..
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
			if(reader->name() == "name"){
				QString folderName_level_2 = reader->readElementText();
				//qDebug()<<"Second Folder Name : "<<reader->readElementText();

				switch(layerLevel){
					case LAYER_LEVEL_1:
					{
						//레이어를 1로 선택 했을 때는 동작 하지 않는다.
						break;
					}
					case LAYER_LEVEL_2:
						//현재 폴더의 단계의 이름을 레이어 명으로 지정한다..
					{
						mlayer = folderName_level_2;
						break;
					}
					case LAYER_LEVEL_3:
					{
						//레이어를 3으로 선택 했을때 해당 폴더의 name은 파일이름으로 지정한다..
						fileName = folderName_level_2;
						//setTargetName(folderName_level_2);
						break;
					}
				}

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
				//this->parsePlaceMark();
			}
		}
		reader->readNext();
	}

}

/*
 *parseSecondtFolder() 함수에서 세번째 Folder 엘리먼트 찾았을 때 해당 함수가 호출된다.
 *해당 함수는 name엘리먼트를 검출하여, 아이디 이름으로 사용하고 , Placemark 엘리먼트를 찾았을때 parsePlacemark함수를 호출 한다.
 */

void RKSIParser::parseThirdFolder()
{
	qDebug()<<"ParseThirdFolder is called..";
	bool isFirst = true;
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
			if(reader->name() == "name"){
				QString folderName_level_3 = reader->readElementText();
				qDebug()<<"FOLDER LEVEL 3 NAME : "<<folderName_level3;
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
						qDebug()<<"Third Folder Name : "<<mlayer;
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

	qDebug()<<"After ThirdFolder : "<<reader->name();
}

void RKSIParser::parsePlaceMark()
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

/*//세번째 Folder Element를 검출 한다.. 사용하지 않음..
void RKSIParser::parseFolder(QXmlStreamReader * reader)
{
	//qDebug()<<"ParseFolder is called..";
	if(reader->tokenType() != QXmlStreamReader::StartElement &&
			reader->name() == "Folder"){
		qDebug()<<"StartTage Name is not Folder";
		return;
	}

	//세번째 Folder태그의 name 태그의 값을 검출한다..
	while(!(reader->tokenType() == QXmlStreamReader::EndElement && reader->name() == "Folder")){
		QXmlStreamReader::TokenType token = reader->readNext();
		if(token == QXmlStreamReader::StartElement){
			//StartElement의 이름이 Folder인 경우
			if(reader->name() == "Folder"){
				while(!(reader->tokenType() == QXmlStreamReader::EndElement && reader->name() == "Folder")){
					reader->readNextStartElement();
					if(reader->name() == "name"){
						qDebug()<<"Last Folder Name : "<<reader->readElementText();
					}
					//reader->readNextStartElement();
					if(reader->name() == "Placemark"){
						while(!(reader->tokenType() == QXmlStreamReader::EndElement && reader->name() == "Placemark")){
							//Placemark의 이름과 좌표를 구함..
							 reader->readNext();
							 if(token == QXmlStreamReader::StartElement){
								 if(reader->name() == "name"){
									 qDebug()<<"Placemark name : "<<reader->readElementText();
								 }
							 }
						}
					}

				}
			}
		}
	}

	//isFinishedThirdFolder = true;
}*/

/*
 * <coordinates> 엘리먼트를 찾았을 때, 해당 엘리먼트의 값(좌표 값)을 가져온다.
 * coordinates의 값은 하나의 스트링 형태로 구성되어 있기때문에 한번 값을 가져오면 또 다른 값을 가져올 필요가 업다. (반복문 필요 없음)
 */
QString RKSIParser::ParseLatLon()
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
				/*if(lineType == "Polygon")
					qDebug()<<"Polygon Coordinates : "<<reader->readElementText();
				else if(lineType == "LineString")
					qDebug()<<"LineString Coordinates : "<<reader->readElementText();*/

				//processLatLon(coordinates, lat, lon);
			}
		}
		reader->readNext();
	}

	return coordinates;
}

//target 이름을 설정한다..
void RKSIParser::setTargetName(QString nFileName)
{
	qDebug()<<"[SetTargetName] nFileName : "<<nFileName;
	QStringList tmpFileName = txtSource->text().split("/");
	for(int i = 0 ; i < tmpFileName.count() ; i++){
		if(i == tmpFileName.count()-1){
			//QStringList nameList = tmpFileName[i].split(".");
			cFileDir.append(nFileName);
			cFileDir.append(".map");
			continue;
		}
		cFileDir.append(tmpFileName.value(i));
		cFileDir.append('/');
	}
	txtTarget->setText(cFileDir);
}

//현재 읽은 라인을 바탕으로 Lon과 Lat를 처리한다.
void RKSIParser::processLatLon(ConvertDataInfo & convertDataInfo){

	qDebug()<<"ProcessLatLon is called..";
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



/*
 * 해당 함수는 Lon정보 (경도)를 도,분,초, 밀리초로 변환하여 Lon 객체를 반환하는 함수 이다.
 */
Lon RKSIParser::convertLon(QString lon)
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


/*
 * 해당 함수는 Lat정보 (위도)를 도,분,초, 밀리초로 변환하여 Lat 객체를 반환하는 함수 이다.
 */
Lat RKSIParser::convertLat(QString lat)
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



/*
 * var1은 최초 위도 경도 자료이며, var2는 해당 데이터에서 degree를 제거한 소수점 타입의 스트링 자료를 담고 있다.
 */
QString RKSIParser::getDegree(QString &var1)
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

QString RKSIParser::getMin(QString &var1)
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

QString RKSIParser::getSec(QString &var1)
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

	/*//var1은 0.xxx로 초와 밀리세컨드를 구하기 위해서 *60을 해야한다..
	QString sec;
	int intSec = (int)(var1 * 60);
			//(int)((var1 - (int)var1)*60);
	//int intSec = (int)(((var1 - (int)var1) * 60 - min) * 60);
	var1 = (var*60) - intSsec;
	sec.setNum(intSec, 10);
	return sec;*/
}

QString RKSIParser::getMilliSec(QString &var1)
{
	QStringList millisecList = var1.split(".");
	//qDebug()<<"FullMilliSec : "<<var1;
	return millisecList[1].left(2);
}
