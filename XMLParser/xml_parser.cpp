#include "xml_parser.h"

#define QNH 	1
#define Output 2
#define BA 		3
#define STCA	4
#define MSAW	5
#define Auto	6

XML_Parser::XML_Parser(QWidget *parent)
    : QWidget(parent)
{
	setupUi(this);

	cmbArea->addItem("QNH");
	cmbArea->addItem("Output");
	cmbArea->addItem("BA");
	cmbArea->addItem("STCA");
	cmbArea->addItem("MSAW");
	//cmbArea->addItem("Auto");


	connect(btnOpen,SIGNAL(clicked()),this,SLOT(openXmlFile()));
	connect(btnCancel,SIGNAL(clicked()),qApp,SLOT(quit()));
	connect(btnOK,SIGNAL(clicked()),this,SLOT(saveXml()));
	connect(btnGen,SIGNAL(clicked()),this,SLOT(generationXmlFile()));
	//connect(this,SIGNAL(setDirectory()),this,SLOT(saveXmlFile()));

	//connect(btnDelete,SIGNAL(clicked()),this,SLOT(deleteCreatedRecords()));

	bool chk = dbOpen();
	if(!chk){
		QMessageBox::warning(this, tr("Failed! DB cannot open"), tr("Failed! DB cannot open"));
		return;
	}
}

XML_Parser::~XML_Parser()
{

}
/*
 *  Open XML file located in current folder.
 */
void XML_Parser::openXmlFile()
{
	txtFilePath->setText(" ");
	qDebug()<<"open XML start..."<<cfilename;
	cfilename.clear();
	qDebug()<<"after clear..."<<cfilename;
	QString dir = QDir::currentPath();

	//test
	QFileDialog fdialog(this);
	//fdialog.setFileMode(QFileDialog::ExistingFiles);

	qDebug()<<"File Mode" <<fdialog.fileMode();
	cfilename = fdialog.getOpenFileNames(this, tr("Open the xml file"),
			dir, tr("xml file(*.xml)"));
	count = cfilename.count();

	if (!cfilename.isEmpty()) {
		qDebug() << "fileName : " << cfilename<<"count : " <<count;
		txtFilePath->setText(cfilename.at(0));
	}
}

void XML_Parser::generationXmlFile()
{
	qDebug()<<"Generation XML File Start";
	bool chk;
	txtFilePath->setText(" ");

	if(cmbArea->currentText() == "QNH")
		chk = readQnhTable();
	else if(cmbArea->currentText() == "Output")
		chk = readOutputTable();
	else if(cmbArea->currentText() == "BA")
		chk = readBaTable();
	else if(cmbArea->currentText() == "STCA")
		chk = readStcaTable();
	else if(cmbArea->currentText() == "MSAW")
		chk = readMsawTable();

	if(!chk){
		QMessageBox::warning(this, tr("Error Message"), tr("It is failed to make xml file.."));
		return;
	}
	QMessageBox::information(this, tr("Success Message"), tr("It is completed."));
}

QString XML_Parser::setDirectory(){
	QString dir = QDir::currentPath();
	QString dirname;
	QString fullname;
	QFileDialog fdialog(this);
	//fdialog.setFileMode(QFileDialog::ExistingFiless);

	dirname = fdialog.getExistingDirectory(this,tr("Open Director"),dir,QFileDialog::ShowDirsOnly);
	fullname = dirname+"/";

	qDebug()<<"fullname" <<fullname;

	return fullname;
}

/*
 *   Save xml datas to tables related to QNH.
 */
bool XML_Parser::saveQnhXmlFile(const Qnh & qnhClass, const QString dirname)
{

	QString filename = qnhClass.name + ".xml";
	QString fullname = dirname + filename;
	QFile file(fullname);
	if(!file.open(QIODevice::WriteOnly)){
		qDebug()<<"Cannot Open file for writing";
		return false;
	}
	QTextStream out(&file);
	out<<"<?xml version=\"1.0\" encoding=\"EUC-KR\"?>\n";
	out<<"<QNHArea>\n";
	out<<"\t<ICAOCode>"<<qnhClass.icaocode<<"</ICAOCode>\n";
	out<<"\t<TrLevel>"<<qnhClass.trlevel<<"FL"<<"</TrLevel>\n";
	if(qnhClass.activeflag == "T")
		out<<"\t<ActiveFlag>"<<qnhClass.activeflag<<"RUE"<<"</ActiveFlag>\n";
	else if(qnhClass.activeflag == "F")
		out<<"\t<ActiveFlag>"<<qnhClass.activeflag<<"ALSE"<<"</ActiveFlag>\n";
	out<<"\t<Meteo>\n";
	out<<"\t\t<MeteoSystemCoordWGS>"<<qnhClass.meteoLAT<<" "<<qnhClass.meteoLON<<" 0ft"<<"</MeteoSystemCoordWGS>\n";
	out<<"\t\t<MeteoUpdatePeriod>"<<qnhClass.meteoupdatepriod<<"</MeteoUpdatePeriod>\n";
	out<<"\t\t<Temperature>"<<qnhClass.temperature<<"</Temperature>\n";
	out<<"\t\t<QNH>"<<qnhClass.qnh<<"</QNH>\n";
	out<<"\t\t<MeteoTimeStamp>"<<qnhClass.meteotimestamp<<"</MeteoTimeStamp>\n";
	out<<"\t</Meteo>\n";
	out<<"\t<Name>"<<qnhClass.name<<"</Name>\n";
	if(qnhClass.type == "P"){
		out<<"\t<Polygon>\n";
		out<<"\t\t<Hmin>"<<qnhClass.hmin<<"ft"<<"</Hmin>\n";
		out<<"\t\t<Hmax>"<<qnhClass.hmax<<"ft"<<"</Hmax>\n";
		for(int i = 0 ; i < qnhClass.polygon.pointList.count(); i++){
			out<<"\t\t<Point>"<<qnhClass.polygon.pointList.at(i).lat<<" ";
			out<<qnhClass.polygon.pointList.at(i).lon<<" ";
			if(qnhClass.polygon.pointList.at(i).alt.isEmpty())
				out<<"0ft"<<"</Point>\n";
			else
				out<<qnhClass.polygon.pointList.at(i).alt<<"</Point>\n";
		}
		out<<"\t</Polygon>\n";
	}else if(qnhClass.type == "S"){
		out<<"\t<Sector>\n";
		out<<"\t\t<Hmin>"<<qnhClass.hmin<<"ft"<<"</Hmin>\n";
		out<<"\t\t<Hmax>"<<qnhClass.hmax<<"ft"<<"</Hmax>\n";
		for(int i = 0 ; i < qnhClass.sector.center.count() ; i++){
			out<<"\t\t<Center>"<<qnhClass.sector.center.at(i).lat<<" ";
			out<<qnhClass.sector.center.at(i).lon<<" ";
			if(qnhClass.sector.center.at(i).alt.isEmpty())
				out<<"0ft"<<"</Center>\n";
			else
				out<<qnhClass.sector.center.at(i).alt<<"</Center>\n";
		}
		out<<"\t\t<Az0>"<<qnhClass.sector.az0<<"</Az0>\n";
		out<<"\t\t<Az1>"<<qnhClass.sector.az1<<"</Az1>\n";
		out<<"\t\t<Rmin>"<<qnhClass.sector.rmin<<"NM"<<"</Rmin>\n";
		out<<"\t\t<Rmax>"<<qnhClass.sector.rmax<<"NM"<<"</Rmax>\n";
		out<<"\t</Sector>\n";
	}
	out<<"</QNHArea>\n";

	return true;
}

bool XML_Parser::saveOutputXmlFile(const Aoi & aoiClass, const QString dirname)
{

	QString filename = aoiClass.name + ".xml";
	QString fullname = dirname + filename;
	QFile file(fullname);
	if(!file.open(QIODevice::WriteOnly)){
		qDebug()<<"Cannot Open file for writing";
		return false;
	}
	QTextStream out(&file);
	out<<"<?xml version=\"1.0\" encoding=\"EUC-KR\"?>\n";
	if(aoiClass.cat == "A")	out<<"<AoiArea>\n";
	else if(aoiClass.cat == "B") out<<"<BlankArea>\n";
	out<<"\t<Mask>"<<aoiClass.mask<<"</Mask>\n";
	if(aoiClass.activeflag == "T")
		out<<"\t<ActiveFlag>"<<aoiClass.activeflag<<"RUE"<<"</ActiveFlag>\n";
	else if(aoiClass.activeflag == "F")
		out<<"\t<ActiveFlag>"<<aoiClass.activeflag<<"ALSE"<<"</ActiveFlag>\n";
	out<<"\t<Name>"<<aoiClass.name<<"</Name>\n";
	if(aoiClass.type == "P"){
		out<<"\t<Polygon>\n";
		out<<"\t\t<Hmin>"<<aoiClass.hmin<<"ft"<<"</Hmin>\n";
		out<<"\t\t<Hmax>"<<aoiClass.hmax<<"ft"<<"</Hmax>\n";
		for(int i = 0 ; i < aoiClass.polygon.point.count(); i++){
			out<<"\t\t<Point>"<<aoiClass.polygon.point.at(i).lat<<" ";
			out<<aoiClass.polygon.point.at(i).lon<<" ";
			if(aoiClass.polygon.point.at(i).alt.isEmpty())
				out<<"0ft"<<"</Point>\n";
			else
				out<<aoiClass.polygon.point.at(i).alt<<"ft"<<"</Point>\n";
		}
		out<<"\t</Polygon>\n";
	}else if(aoiClass.type == "S"){
		out<<"\t<Sector>\n";
		out<<"\t\t<Hmin>"<<aoiClass.hmin<<"ft"<<"</Hmin>\n";
		out<<"\t\t<Hmax>"<<aoiClass.hmax<<"ft"<<"</Hmax>\n";
		for(int i = 0 ; i < aoiClass.sector.center.count() ; i++){
			out<<"\t\t<Center>"<<aoiClass.sector.center.at(i).lat<<" ";
			out<<aoiClass.sector.center.at(i).lon<<" ";
			if(aoiClass.sector.center.at(i).alt.isEmpty())
				out<<"0ft"<<"</Center>\n";
			else
				out<<aoiClass.sector.center.at(i).alt<<"ft"<<"</Center>\n";
		}
		out<<"\t\t<Az0>"<<aoiClass.sector.az0<<"</Az0>\n";
		out<<"\t\t<Az1>"<<aoiClass.sector.az1<<"</Az1>\n";
		out<<"\t\t<Rmin>"<<aoiClass.sector.rmin<<"NM"<<"</Rmin>\n";
		out<<"\t\t<Rmax>"<<aoiClass.sector.rmax<<"NM"<<"</Rmax>\n";
		out<<"\t</Sector>\n";
	}

	if(aoiClass.cat == "A")	out<<"</AoiArea>\n";
	else if(aoiClass.cat == "B") out<<"</BlankArea>";

	return true;
}

bool XML_Parser::saveBaXmlFile(const Ba & baClass, const QString dirname)
{

	QString filename = baClass.name + ".xml";
	QString fullname = dirname + filename;
	QFile file(fullname);
	if(!file.open(QIODevice::WriteOnly)){
		qDebug()<<"Cannot Open file for writing";
		return false;
	}
	QTextStream out(&file);
	out<<"<?xml version=\"1.0\" encoding=\"EUC-KR\"?>\n";
	out<<"<BlankArea>\n";
	out<<"\t<Mask>"<<baClass.mask<<"</Mask>\n";
	if(baClass.activeflag == "T")
		out<<"\t<ActiveFlag>"<<baClass.activeflag<<"RUE"<<"</ActiveFlag>\n";
	else if(baClass.activeflag == "F")
		out<<"\t<ActiveFlag>"<<baClass.activeflag<<"ALSE"<<"</ActiveFlag>\n";
	out<<"\t<Name>"<<baClass.name<<"</Name>\n";
	if(baClass.type == "P"){
		out<<"\t<Polygon>\n";
		out<<"\t\t<Hmin>"<<baClass.hmin<<"ft"<<"</Hmin>\n";
		out<<"\t\t<Hmax>"<<baClass.hmax<<"ft"<<"</Hmax>\n";
		for(int i = 0 ; i < baClass.polygon.point.count(); i++){
			out<<"\t\t<Point>"<<baClass.polygon.point.at(i).lat<<" ";
			out<<baClass.polygon.point.at(i).lon<<" ";
			if(baClass.polygon.point.at(i).alt.isEmpty())
				out<<"0ft"<<"</Point>\n";
			else
				out<<baClass.polygon.point.at(i).alt<<"ft"<<"</Point>\n";
		}
		out<<"\t</Polygon>\n";
	}else if(baClass.type == "S"){
		out<<"\t<Sector>\n";
		out<<"\t\t<Hmin>"<<baClass.hmin<<"ft"<<"</Hmin>\n";
		out<<"\t\t<Hmax>"<<baClass.hmax<<"ft"<<"</Hmax>\n";
		for(int i = 0 ; i < baClass.sector.center.count() ; i++){
			out<<"\t\t<Center>"<<baClass.sector.center.at(i).lat<<" ";
			out<<baClass.sector.center.at(i).lon<<" ";
			if(baClass.sector.center.at(i).alt.isEmpty())
				out<<"0ft"<<"</Center>\n";
			else
				out<<baClass.sector.center.at(i).alt<<"ft"<<"</Center>\n";
		}
		out<<"\t\t<Az0>"<<baClass.sector.az0<<"</Az0>\n";
		out<<"\t\t<Az1>"<<baClass.sector.az1<<"</Az1>\n";
		out<<"\t\t<Rmin>"<<baClass.sector.rmin<<"NM"<<"</Rmin>\n";
		out<<"\t\t<Rmax>"<<baClass.sector.rmax<<"NM"<<"</Rmax>\n";
		out<<"\t</Sector>\n";
	}
	out<<"</BlankArea>";

	return true;
}


bool XML_Parser::saveStcaXmlFile(const Stca & stcaClass, const QString dirname)
{
	qDebug()<<"Save stca xml file called...";
	QString filename = stcaClass.name + ".xml";
	QString fullname = dirname + filename;
	QFile file(fullname);
	if(!file.open(QIODevice::WriteOnly)){
		qDebug()<<"Cannot Open file for writing";
		return false;
	}
	QTextStream out(&file);
	out<<"<?xml version=\"1.0\" encoding=\"EUC-KR\"?>\n";
	out<<"<STCAArea>\n";
	out<<"\t<STCAType>"<<stcaClass.stcatype<<"</STCAType>\n";
	if(stcaClass.activeflag == "T")
		out<<"\t<ActiveFlag>"<<stcaClass.activeflag<<"RUE"<<"</ActiveFlag>\n";
	else if(stcaClass.activeflag == "F")
		out<<"\t<ActiveFlag>"<<stcaClass.activeflag<<"ALSE"<<"</ActiveFlag>\n";
	out<<"\t<Name>"<<stcaClass.name<<"</Name>\n";
	if(stcaClass.type == "P"){
		out<<"\t<Polygon>\n";
		out<<"\t\t<Hmin>"<<stcaClass.hmin<<"ft"<<"</Hmin>\n";
		out<<"\t\t<Hmax>"<<stcaClass.hmax<<"ft"<<"</Hmax>\n";
		for(int i = 0 ; i < stcaClass.polygon.point.count(); i++){
			out<<"\t\t<Point>"<<stcaClass.polygon.point.at(i).lat<<" ";
			out<<stcaClass.polygon.point.at(i).lon<<" ";
			if(stcaClass.polygon.point.at(i).alt.isEmpty())
				out<<"0ft"<<"</Point>\n";
			else
				out<<stcaClass.polygon.point.at(i).alt<<"ft"<<"</Point>\n";
		}
		out<<"\t</Polygon>\n";
	}else if(stcaClass.type == "S"){
		out<<"\t<Sector>\n";
		out<<"\t\t<Hmin>"<<stcaClass.hmin<<"ft"<<"</Hmin>\n";
		out<<"\t\t<Hmax>"<<stcaClass.hmax<<"ft"<<"</Hmax>\n";
		for(int i = 0 ; i < stcaClass.sector.center.count() ; i++){
			out<<"\t\t<Center>"<<stcaClass.sector.center.at(i).lat<<" ";
			out<<stcaClass.sector.center.at(i).lon<<" ";
			if(stcaClass.sector.center.at(i).alt.isEmpty())
				out<<"0ft"<<"</Center>\n";
			else
				out<<stcaClass.sector.center.at(i).alt<<"ft"<<"</Center>\n";
		}
		out<<"\t\t<Az0>"<<stcaClass.sector.az0<<"</Az0>\n";
		out<<"\t\t<Az1>"<<stcaClass.sector.az1<<"</Az1>\n";
		out<<"\t\t<Rmin>"<<stcaClass.sector.rmin<<"NM"<<"</Rmin>\n";
		out<<"\t\t<Rmax>"<<stcaClass.sector.rmax<<"NM"<<"</Rmax>\n";
		out<<"\t</Sector>";
	}
	out<<"</STCAArea>";
	qDebug()<<"Save stca xml file finished...";
	return true;
}


bool XML_Parser::saveMsawXmlFile(const Msaw & msawClass, const QString dirname)
{

	QString filename = msawClass.name + ".xml";
	QString fullname = dirname + filename;
	QFile file(fullname);
	if(!file.open(QIODevice::WriteOnly)){
		qDebug()<<"Cannot Open file for writing";
		return false;
	}
	QTextStream out(&file);
	out<<"<?xml version=\"1.0\" encoding=\"EUC-KR\"?>\n";
	out<<"<MSAWArea>\n";
	out<<"\t<MSAWType>"<<msawClass.msawtype<<"</MSAWType>\n";
	if(msawClass.activeflag == "T")
		out<<"\t<ActiveFlag>"<<msawClass.activeflag<<"RUE"<<"</ActiveFlag>\n";
	else if(msawClass.activeflag == "F")
		out<<"\t<ActiveFlag>"<<msawClass.activeflag<<"ALSE"<<"</ActiveFlag>\n";
	out<<"\t<Name>"<<msawClass.name<<"</Name>\n";
	if(msawClass.type == "P"){
		out<<"\t<Polygon>\n";
		out<<"\t\t<Hmin>"<<msawClass.hmin<<"ft"<<"</Hmin>\n";
		out<<"\t\t<Hmax>"<<msawClass.hmax<<"ft"<<"</Hmax>\n";
		for(int i = 0 ; i < msawClass.polygon.point.count(); i++){
			out<<"\t\t<Point>"<<msawClass.polygon.point.at(i).lat<<" ";
			out<<msawClass.polygon.point.at(i).lon<<" ";
			if(msawClass.polygon.point.at(i).alt.isEmpty())
				out<<"0ft"<<"</Point>\n";
			else
				out<<msawClass.polygon.point.at(i).alt<<"ft"<<"</Point>\n";
		}
		out<<"\t</Polygon>\n";
	}else if(msawClass.type == "S"){
		out<<"\t<Sector>\n";
		out<<"\t\t<Hmin>"<<msawClass.hmin<<"ft"<<"</Hmin>\n";
		out<<"\t\t<Hmax>"<<msawClass.hmax<<"ft"<<"</Hmax>\n";
		for(int i = 0 ; i < msawClass.sector.center.count() ; i++){
			out<<"\t\t<Center>"<<msawClass.sector.center.at(i).lat<<" ";
			out<<msawClass.sector.center.at(i).lon<<" ";
			if(msawClass.sector.center.at(i).alt.isEmpty())
				out<<"0ft"<<"</Center>\n";
			else
				out<<msawClass.sector.center.at(i).alt<<"ft"<<"</Center>\n";
		}
		out<<"\t\t<Az0>"<<msawClass.sector.az0<<"</Az0>\n";
		out<<"\t\t<Az1>"<<msawClass.sector.az1<<"</Az1>\n";
		out<<"\t\t<Rmin>"<<msawClass.sector.rmin<<"NM"<<"</Rmin>\n";
		out<<"\t\t<Rmax>"<<msawClass.sector.rmax<<"NM"<<"</Rmax>\n";
		out<<"\t</Sector>\n";
	}
	out<<"</MSAWArea>";

	return true;
}



void XML_Parser::saveXml()
{
	bool chk;
	int i = 0;
	bool isError = false;	//false : NoError, true : Error

	// 1.  파일 미 선택 체크
	if (txtFilePath->text() == ""){
		QMessageBox::warning(this, tr("Please select a file."), tr("Please select a file."));
		return;
	}
	QString combox_selected_data = cmbArea->currentText();

	db.transaction();
	query = new QSqlQuery(db);
	QVector<QString> filename;
	for(int j = 0 ; j < count ; j++){
		QString tmp = cfilename.at(j);
		filename.append(tmp);
		qDebug()<<"filename : "<<filename.at(j);
	}
	while(i < count){
		QFile file(filename.at(i));
		chk = openFile(file);
		if(!chk) {
			isError = true;
			qDebug()<<"File Open Error....";
			break;
		}
		if (combox_selected_data == "QNH") {  // 3.1 QNH인 경우
			qDebug()<<"QNH process for a file";
			if(!(qnhXmlProcess(file))){
				isError = true;
				qDebug()<<"Qnh XML Process Error....";
				break;
			}
		}else if (combox_selected_data == "Output") {  // 3.2 Output인 경우
			qDebug()<<"Output process for a file";
			if(!(outputXmlProcess(file))){
				isError = true;
				qDebug()<<"output XML Process Error....";
				break;
			}
		}else if (combox_selected_data == "BA") {
			qDebug()<<"BA process for a file";
			if(!(baXmlProcess(file))){
				isError = true;
				qDebug()<<"BA XML Process Error....";
				break;
			}
		}else if (combox_selected_data == "STCA"){
			qDebug()<<"STCA process for a file";
			if(!(stcaXmlProcess(file))){
				isError = true;
				qDebug()<<"stc XML Process Error....";
				break;
			}
		}else if (combox_selected_data == "MSAW"){
			qDebug()<<"MSAW process for a file";
			if(!(msawXmlProcess(file))){
				isError = true;
				qDebug()<<"msaw XML, QSqlQuery *query Process Error....";
				break;
			}
		}


		isError = false;
		i++;
	}
	if(isError == true) db.rollback();
	else if(isError == false){
		QMessageBox::information(this, tr("Success Message"), tr("It is completed."));
		db.commit();
	}
	qDebug()<<"save XML finished, filename : "<<cfilename;
	//qDebug()<<"after clear, filename : "<<cfilename;
}



/*
 * Output xml data
 */

bool XML_Parser::outputXmlProcess(QFile & file)
{
	qDebug() <<"output xml process called..";
	bool chk;
	QDomDocument doc;
	Aoi aoiClass;

	chk = checkDomDocument(file, doc);
	if(!chk){
		qDebug()<<"Checking dom document failed";
		return false;
	}

	QDomElement root = doc.documentElement();
	qDebug()<<"root tagname :" <<root.tagName();

	if (root.tagName() == "AoiArea" || root.tagName() == "BlankArea"){
		aoiClass.area = Output;
		if(root.tagName() == "AoiArea") aoiClass.cat = "A";
		else if(root.tagName() == "BlankArea") aoiClass.cat = "B";
		else return false;

		QDomNode child = root.firstChild();
		while(!child.isNull()){
			qDebug() << "xml read .........";
			if(child.toElement().tagName() == "Mask"){
				aoiClass.mask = child.toElement().text();
			}else if(child.toElement().tagName() == "ActiveFlag"){
				if (child.toElement().text() == "TRUE") aoiClass.activeflag = "T";
				else aoiClass.activeflag = "F";
				//aoiClass.activeflag = child.toElement().text();
			}else if(child.toElement().tagName() == "Name"){
				aoiClass.name = child.toElement().text();
			}else if(child.toElement().tagName() == "Polygon") {
				aoiClass.type = "P";
				QDomNode node = child.firstChild();
				while(!node.isNull()) {
					if (node.toElement().tagName() == "Hmin") {
						aoiClass.hmin = node.toElement().text();
						int temp = aoiClass.hmin.count();
						aoiClass.hmin.remove(temp-2,2);
					}else if(node.toElement().tagName() == "Hmax") {
						aoiClass.hmax = node.toElement().text();
						int temp = aoiClass.hmax.count();
						aoiClass.hmax.remove(temp-2,2);
					}else if(node.toElement().tagName() == "Point") {
						QString tt = node.toElement().text();
						LatLon ll;
						QStringList list = tt.split(" ");   // 0 : Lat,   1: Lon,  2 : 0ft
						ll.lat = list.at(0);
						ll.lon = list.at(1);
						ll.alt = list.at(2);
						int temp = ll.alt.count();
						ll.alt.remove(temp-2,2);
						aoiClass.polygon.point.append(ll);
					}
					node = node.nextSibling();
				}
			}else if(child.toElement().tagName() == "Sector"){
				aoiClass.type = "S";
				QDomNode node = child.firstChild();
				while(!node.isNull()){
					if(node.toElement().tagName() == "Hmin"){
						aoiClass.hmin = node.toElement().text();
						int temp = aoiClass.hmin.count();
						aoiClass.hmin.remove(temp-2,2);
					}else if(node.toElement().tagName() == "Hmax"){
						aoiClass.hmax = node.toElement().text();
						int temp = aoiClass.hmax.count();
						aoiClass.hmax.remove(temp-2,2);
					}else if(node.toElement().tagName() == "Center") {
						QString tt = node.toElement().text();
						LatLon ll;
						QStringList list = tt.split(" ");   // 0 : Lat,   1: Lon,  2 : 0ft
						ll.lat = list.at(0);
						ll.lon = list.at(1);
						ll.alt = list.at(2);
						int temp = ll.alt.count();
						ll.alt.remove(temp-2,2);
						aoiClass.sector.center.append(ll);
					}else if(node.toElement().tagName() == "Az0") {
						aoiClass.sector.az0 = node.toElement().text();
					}else if(node.toElement().tagName() == "Az1") {
						aoiClass.sector.az1 = node.toElement().text();
					}else if(node.toElement().tagName() == "Rmin") {
						aoiClass.sector.rmin = node.toElement().text();
						int temp = aoiClass.sector.rmin.count();
						aoiClass.sector.rmin.remove(temp-2,2);
					}else if(node.toElement().tagName() == "Rmax") {
						aoiClass.sector.rmax = node.toElement().text();
						int temp = aoiClass.sector.rmax.count();
						aoiClass.sector.rmax.remove(temp-2,2);
					}
					node = node.nextSibling();
				}
			}
			child = child.nextSibling();
		}
	}else{
		qDebug() << "error AoiArea and BlankArea not...";
		QMessageBox::warning(this, tr("Failed! AoiArea or BlankArea is not found"), tr("Failed! AoiArea or BlankArea  is not found %1").arg(file.fileName()));
		return false;
	}

/*
	if(!checkType(aoiClass.type)){
		QMessageBox::warning(this, tr("Failed! Type is not matched"), tr("Failed! Type is not matched %1").arg(txtFilePath->text()));
		return false;
	}
*/

	chk = checkDuplicationPK(aoiClass.name,aoiClass.area);
	if(!chk){
		QMessageBox::warning(this, tr("Failed! already exist"), tr("Failed! %1 already exists").arg(cmbArea->currentText()));
		return false;
	}

	/*
	 *Insert record into TB_QVH
	 */

	chk = insertMainRecord(aoiClass);
	if(!chk)
	{
		qDebug()<<"insert Record Error";
		//db.rollback();
		return false;
	}

	/*
	 * Execute query procedure to insert either polygon or sector data into specific tables.
	 */
	if (aoiClass.type == "P"){
		// About polygon data
		chk = insertPolygonRecord(aoiClass);
		if (!chk){
			//db.rollback();
			return false;
		}else{
			//QMessageBox::information(this, tr("Success Message"), tr("It is completed."));
			//db.commit();
		}
	}else if(aoiClass.type == "S"){
		//About sector data
		chk = insertSectorRecord(aoiClass);
		if (!chk){
			//db.rollback();
			return false;
		}else{
			//QMessageBox::information(this, tr("Success Message"), tr("It is completed."));
			//db.commit();
		}
	}else return false;




	return true;
}
/*
 * Ba xml data
 */

bool XML_Parser::baXmlProcess(QFile & file)
{
	bool chk;
	QDomDocument doc;
	Ba baClass;

	chk = checkDomDocument(file, doc);
	if(!chk){
		qDebug()<<"Checking dom document failed";
		return false;
	}

	QDomElement root = doc.documentElement();
	qDebug()<<"root tagname :" <<root.tagName();

	if (root.tagName()!= "BlankArea"){
		qDebug() << "error BlankArea not...";
		QMessageBox::warning(this, tr("Failed! BlankArea is not found"), tr("Failed! BlankArea  is not found %1").arg(file.fileName()));
		return false;
	}
	baClass.area = BA;
	QDomNode child = root.firstChild();
	while(!child.isNull()){
		qDebug() << "xml read .........";
		if(child.toElement().tagName() == "Mask"){
			baClass.mask = child.toElement().text();
		}else if(child.toElement().tagName() == "ActiveFlag"){
			if (child.toElement().text() == "TRUE") baClass.activeflag = "T";
			else baClass.activeflag = "F";
		}else if(child.toElement().tagName() == "Name"){
			baClass.name = child.toElement().text();
		}else if(child.toElement().tagName() == "Polygon") {
			baClass.type = "P";
			QDomNode node = child.firstChild();
			while(!node.isNull()) {
				if (node.toElement().tagName() == "Hmin") {
					baClass.hmin = node.toElement().text();
					int temp = baClass.hmin.count();
					baClass.hmin.remove(temp-2,2);
				}else if(node.toElement().tagName() == "Hmax") {
					baClass.hmax = node.toElement().text();
					int temp = baClass.hmax.count();
					baClass.hmax.remove(temp-2,2);
				}else if(node.toElement().tagName() == "Point") {
					QString tt = node.toElement().text();
					LatLon ll;
					QStringList list = tt.split(" ");   // 0 : Lat,   1: Lon,  2 : 0ft
					ll.lat = list.at(0);
					ll.lon = list.at(1);
					ll.alt = list.at(2);
					int temp = ll.alt.count();
					ll.alt.remove(temp-2,2);
					baClass.polygon.point.append(ll);
				}
				node = node.nextSibling();
			}
		}else if(child.toElement().tagName() == "Sector"){
			baClass.type = "S";
			QDomNode node = child.firstChild();
			while(!node.isNull()){
				if(node.toElement().tagName() == "Hmin"){
					baClass.hmin = node.toElement().text();
					int temp = baClass.hmin.count();
					baClass.hmin.remove(temp-2,2);
				}else if(node.toElement().tagName() == "Hmax"){
					baClass.hmax = node.toElement().text();
					int temp = baClass.hmax.count();
					baClass.hmax.remove(temp-2,2);
				}else if(node.toElement().tagName() == "Center") {
					QString tt = node.toElement().text();
					LatLon ll;
					QStringList list = tt.split(" ");   // 0 : Lat,   1: Lon,  2 : 0ft
					ll.lat = list.at(0);
					ll.lon = list.at(1);
					ll.alt = list.at(2);
					int temp = ll.alt.count();
					ll.alt.remove(temp-2,2);
					baClass.sector.center.append(ll);
				}else if(node.toElement().tagName() == "Az0") {
					baClass.sector.az0 = node.toElement().text();
				}else if(node.toElement().tagName() == "Az1") {
					baClass.sector.az1 = node.toElement().text();
				}else if(node.toElement().tagName() == "Rmin") {
					baClass.sector.rmin = node.toElement().text();
					int temp = baClass.sector.rmin.count();
					baClass.sector.rmin.remove(temp-2,2);
				}else if(node.toElement().tagName() == "Rmax") {
					baClass.sector.rmax = node.toElement().text();
					int temp = baClass.sector.rmax.count();
					baClass.sector.rmax.remove(temp-2,2);
				}
				node = node.nextSibling();
			}
		}
		child = child.nextSibling();
	}


/*	if(!checkType(baClass.type)){
		QMessageBox::warning(this, tr("Failed! Type is not matched"), tr("Failed! Type is not matched %1").arg(txtFilePath->text()));
		return false;
	}*/

	chk = checkDuplicationPK(baClass.name,baClass.area);
	if(!chk){
		QMessageBox::warning(this, tr("Failed! already exist"), tr("Failed! %1 already exists").arg(cmbArea->currentText()));
		return false;
	}

	qDebug()<<"Insert Process Start";
	chk = insertMainRecord(baClass);
	if(!chk)
	{
		qDebug()<<"insert Record Error";
		return false;
	}

	/*
	 * Execute query procedure to insert either polygon or sector data into specific tables.
	 */
	if (baClass.type == "P"){
		// About polygon data
		chk = insertPolygonRecord(baClass);
		if (!chk)	return false;
	}else if(baClass.type == "S"){
		//About sector data
		chk = insertSectorRecord(baClass);
		if (!chk)	return false;
	}else return false;

	return true;
}

/**
 * QNH xml data
 */

bool XML_Parser::qnhXmlProcess(QFile & file)
{
	bool chk;
	QDomDocument doc;
	Qnh qnhClass;
	//bool isError = false;	//false : NoError, true : Error

	chk = checkDomDocument(file, doc);
	if(!chk){
		qDebug()<<"Checking dom document failed";
		return false;
	}

	QDomElement root = doc.documentElement();
	if (root.tagName() != "QNHArea") {
		// error message
		qDebug() << "error QNHArea not...";
		QMessageBox::warning(this, tr("Failed! QNHArea is not found"), tr("Failed! QNHArea is not found %1").arg(file.fileName()));
		return false;
	}
	// 4. 태그 분석된 내용에서 키값을 얻어와서 DB에 존재하는지 체크
	qnhClass.area = QNH;
	QDomNode child = root.firstChild();
	while(!child.isNull()) {
		qDebug() << "xml read...............";
		if (child.toElement().tagName() == "ICAOCode") {
			qnhClass.icaocode = child.toElement().text();
		}else if(child.toElement().tagName() == "TrLevel") {
			qnhClass.trlevel = child.toElement().text();
			qnhClass.trlevel = qnhClass.trlevel.replace("FL", "");
		}else if(child.toElement().tagName() == "ActiveFlag") {
			if (child.toElement().text() == "TRUE")
				qnhClass.activeflag = "T";
			else
				qnhClass.activeflag = "F";
			//activeflag = child.toElement().text();
		}else if(child.toElement().tagName() == "Meteo") {
			QDomNode node = child.firstChild();
			while(!node.isNull()) {
				if (node.toElement().tagName() == "MeteoSystemCoordWGS") {
					qnhClass.meteosystemcoordwgs = node.toElement().text();
					QStringList list = qnhClass.meteosystemcoordwgs.split(" ");
					qnhClass.meteoLAT = list.at(0);
					qnhClass.meteoLON = list.at(1);
					qDebug() << "MeteoLAT:"<<qnhClass.meteoLAT<<"MeteoLON:"<<qnhClass.meteoLON;
				}else if(node.toElement().tagName() == "MeteoUpdatePeriod") {
					qnhClass.meteoupdatepriod =node.toElement().text();
				}else if(node.toElement().tagName() == "Temperature") {
					qnhClass.temperature =node.toElement().text();
				}else if(node.toElement().tagName() == "QNH") {
					qnhClass.qnh = node.toElement().text();
				}else if(node.toElement().tagName() == "MeteoTimeStamp") {
					qDebug() << "Meteo Time Stamp";
					qnhClass.meteotimestamp =node.toElement().text();
					QStringList list = qnhClass.meteotimestamp.split(" ");
					qnhClass.date = list.at(0);
					qnhClass.time = list.at(1);
					qDebug() << "Date:"<<qnhClass.date<<"time:"<<qnhClass.time;
				}
				qDebug() << "Finish Meteo Procedure";
				node = node.nextSibling();
			}
		}else if(child.toElement().tagName() == "Name") {
			qnhClass.name = child.toElement().text();
		}
		// name
		else if(child.toElement().tagName() == "Polygon") {
			qDebug()<<"Polygon Procedure";
			qnhClass.type = "P";
			QDomNode node = child.firstChild();
			while(!node.isNull()) {
				if (node.toElement().tagName() == "Hmin") {
					qnhClass.hmin = node.toElement().text();
					int temp = qnhClass.hmin.count();
					qnhClass.hmin.remove(temp-2,2);
				}else if(node.toElement().tagName() == "Hmax") {
					qnhClass.hmax = node.toElement().text();
					int temp = qnhClass.hmax.count();
					qnhClass.hmax.remove(temp-2,2);
				}else if(node.toElement().tagName() == "Point") {
					QString tt = node.toElement().text();
					LatLon ll;
					QStringList list = tt.split(" ");   // 0 : Lat,   1: Lon,  2 : 0ft
					ll.lat = list.at(0);
					ll.lon = list.at(1);
					ll.alt = list.at(2);
					int temp = ll.alt.count();
					ll.alt.remove(temp-2,2);
					qnhClass.polygon.pointList.append(ll);
				}
				node = node.nextSibling();
			}
			qnhClass.type = "P";
		}else if(child.toElement().tagName() == "Sector") {
			qnhClass.type = "S";
			QDomNode node = child.firstChild();
			while(!node.isNull()) {
				if (node.toElement().tagName() == "Hmin") {
					qnhClass.hmin = node.toElement().text();
					int temp = qnhClass.hmin.count();
					qnhClass.hmin.remove(temp-2,2);
				}else if(node.toElement().tagName() == "Hmax") {
					qnhClass.hmax = node.toElement().text();
					int temp = qnhClass.hmax.count();
					qnhClass.hmax.remove(temp-2,2);
				}else if(node.toElement().tagName() == "Center") {
					QString tt = node.toElement().text();
					LatLon ll;
					QStringList list = tt.split(" ");   // 0 : Lat,   1: Lon,  2 : 0ft
					ll.lat = list.at(0);
					ll.lon = list.at(1);
					ll.alt = list.at(2);
					int temp = ll.alt.count();
					ll.alt.remove(temp-2,2);
					qnhClass.sector.center.append(ll);
				}else if(node.toElement().tagName() == "Az0") {
					qnhClass.sector.az0 = node.toElement().text();
				}else if(node.toElement().tagName() == "Az1") {
					qnhClass.sector.az1 = node.toElement().text();
				}else if(node.toElement().tagName() == "Rmin") {
					qnhClass.sector.rmin = node.toElement().text();
					int temp = qnhClass.sector.rmin.count();
					qnhClass.sector.rmin.remove(temp-2,2);
				}else if(node.toElement().tagName() == "Rmax") {
					qnhClass.sector.rmax = node.toElement().text();
					int temp = qnhClass.sector.rmax.count();
					qnhClass.sector.rmax.remove(temp-2,2);
				}

				node = node.nextSibling();
			}
			qnhClass.type = "S";
		}

		child = child.nextSibling();
	}

	// type check...

	/*if(!checkType(qnhClass.type)){
		QMessageBox::warning(this, tr("Failed! Type is not matched"), tr("Failed! Type is not matched %1").arg(txtFilePath->text()));
		return false;
	}*/

	// debug
	//qDebug() << "icaocode : " << icaocode <<"  QNH_NAME:" << name <<"  Active:" << activeflag << " MIN_ALT:"<<hmin<<" MAX_ALT:"<<hmax;

	//Primary Key check
	chk = checkDuplicationPK(qnhClass.name, qnhClass.area);
	if(!chk){
		QMessageBox::warning(this, tr("Failed! already exist"), tr("Failed! %1 already exists").arg(cmbArea->currentText()));
		return false;
	}
	qDebug()<<"Check Duplication PK finished";

	chk = insertMainRecord(qnhClass);
	if(!chk)
	{
		qDebug()<<"insert Record Error";
		//db.rollback();
		return false;
	}

	qDebug()<<"insert Record success";

	if (qnhClass.type == "P"){
		// About polygon data
		qDebug()<<"insert Polygon called";
		chk = insertPolygonRecord(qnhClass);
		if (!chk)return false;
	}else if(qnhClass.type == "S"){
		//About sector data
		qDebug()<<"insert Sector called";
		chk = insertSectorRecord(qnhClass);
		if (!chk) return false;
	}

	return true;
}
/*
 *  STCA Xml Data
 */

bool XML_Parser::stcaXmlProcess(QFile & file)
{
	qDebug() <<"stca xml process called..";
	bool chk;
	Stca stcaClass;
	QDomDocument doc;

	chk = checkDomDocument(file, doc);
	if(!chk){
		qDebug()<<"Checking dom document failed";
		return false;
	}
	QDomElement root = doc.documentElement();
	qDebug()<<"root tagname :" <<root.tagName();

	if (root.tagName() != "STCAArea") {
		// error message
		qDebug() << "error STCAArea not...";
		QMessageBox::warning(this, tr("Failed! STCAArea is not found"), tr("Failed! STCAArea is not found %1").arg(file.fileName()));
		return false;
	}
	stcaClass.area = STCA;
	stcaClass.cat = "S";
	QDomNode child = root.firstChild();
	while(!child.isNull()) {
		qDebug() << "xml read...............";
		if (child.toElement().tagName() == "STCAType") {
			stcaClass.stcatype = child.toElement().text();
			qDebug()<<"StcaType : "<<stcaClass.stcatype;
		}else if(child.toElement().tagName() == "ActiveFlag") {
			if (child.toElement().text() == "TRUE")
				stcaClass.activeflag = "T";
			else
				stcaClass.activeflag = "F";
		}else if(child.toElement().tagName() == "Name") {
			stcaClass.name = child.toElement().text();
		}else if(child.toElement().tagName() == "Polygon") {
			QDomNode node = child.firstChild();
			stcaClass.type = "P";
			while(!node.isNull()) {
				if (node.toElement().tagName() == "Hmin") {
					stcaClass.hmin = node.toElement().text();
					int temp = stcaClass.hmin.count();
					stcaClass.hmin.remove(temp-2,2);
				}else if(node.toElement().tagName() == "Hmax") {
					stcaClass.hmax = node.toElement().text();
					int temp = stcaClass.hmax.count();
					stcaClass.hmax.remove(temp-2,2);
				}else if(node.toElement().tagName() == "Point") {
					QString tt = node.toElement().text();
					LatLon ll;
					QStringList list = tt.split(" ");   // 0 : Lat,   1: Lon,  2 : 0ft
					ll.lat = list.at(0);
					ll.lon = list.at(1);
					ll.alt = list.at(2);
					int temp = ll.alt.count();
					ll.alt.remove(temp-2,2);
					stcaClass.polygon.point.append(ll);
				}
				node = node.nextSibling();
			}
		}else if(child.toElement().tagName() == "Sector") {
			stcaClass.type = "S";
			QDomNode node = child.firstChild();
			while(!node.isNull()) {
				if (node.toElement().tagName() == "Hmin") {
					stcaClass.hmin = node.toElement().text();
					int temp = stcaClass.hmin.count();
					stcaClass.hmin.remove(temp-2,2);
				}else if(node.toElement().tagName() == "Hmax") {
					stcaClass.hmax = node.toElement().text();
					int temp = stcaClass.hmax.count();
					stcaClass.hmax.remove(temp-2,2);
				}else if(node.toElement().tagName() == "Center") {
					QString tt = node.toElement().text();
					LatLon ll;
					QStringList list = tt.split(" ");   // 0 : Lat,   1: Lon,  2 : 0ft
					ll.lat = list.at(0);
					ll.lon = list.at(1);
					ll.alt = list.at(2);
					int temp = ll.alt.count();
					ll.alt.remove(temp-2,2);
					stcaClass.sector.center.append(ll);
				}else if(node.toElement().tagName() == "Az0") {
					stcaClass.sector.az0 = node.toElement().text();
				}else if(node.toElement().tagName() == "Az1") {
					stcaClass.sector.az1 = node.toElement().text();
				}else if(node.toElement().tagName() == "Rmin") {
					stcaClass.sector.rmin = node.toElement().text();
					int temp = stcaClass.sector.rmin.count();
					stcaClass.sector.rmin.remove(temp-2,2);
					//int temp = qnhClass.sector.rmin.count();
				}else if(node.toElement().tagName() == "Rmax") {
					stcaClass.sector.rmax = node.toElement().text();
					int temp = stcaClass.sector.rmax.count();
					stcaClass.sector.rmax.remove(temp-2,2);
				}

				node = node.nextSibling();
			}
		}
		child = child.nextSibling();
	}

/*	if(!checkType(stcaClass.type)){
		QMessageBox::warning(this, tr("Failed! Type is not matched"), tr("Failed! Type is not matched %1").arg(txtFilePath->text()));
		return false;
	}
	*/
	//Primary Key check
	chk = checkDuplicationPK(stcaClass.name, stcaClass.area);
	if(!chk){
		QMessageBox::warning(this, tr("Failed! already exist"), tr("Failed! %1 already exists").arg(cmbArea->currentText()));
		return false;
	}

	//Insert Record to TB_SAFETYAREA
	chk = insertMainRecord(stcaClass);
	if(!chk){
		qDebug()<<"insert Record Error";
		//db.rollback();
		return false;
	}

	if (stcaClass.type == "P"){
		// About polygon data
		chk = insertPolygonRecord(stcaClass);
		if (!chk){
			//db.rollback();
			return false;
		}
	}else if(stcaClass.type == "S"){
		//About sector data
		chk = insertSectorRecord(stcaClass);
		if (!chk){
			//db.rollback();
			return false;
		}
	}else return false;

	return chk;
}

bool XML_Parser::msawXmlProcess(QFile & file)
{
	qDebug() <<"msaw xml process called..";
	bool chk;
	Msaw msawClass;
	QDomDocument doc;
	chk = checkDomDocument(file, doc);
	if(!chk){
		qDebug()<<"Checking dom document failed";
		return false;
	}
	QDomElement root = doc.documentElement();
	qDebug()<<"root tagname :" <<root.tagName();

	if (root.tagName() != "MSAWArea") {
		// error message
		qDebug() << "error MSAWArea not...";
		QMessageBox::warning(this, tr("Failed! MSAWArea is not found"), tr("Failed! MSAWArea is not found %1").arg(file.fileName()));
		return false;
	}

	msawClass.area = MSAW;
	msawClass.cat = "M";
	QDomNode child = root.firstChild();
	while(!child.isNull()) {
		qDebug() << "xml read...............";
		if (child.toElement().tagName() == "MSAWType") {
			msawClass.msawtype = child.toElement().text();
			//qDebug()<<"StcaType : "<<stcaClass.stcatype;
		}else if(child.toElement().tagName() == "ActiveFlag") {
			if (child.toElement().text() == "TRUE")
				msawClass.activeflag = "T";
			else
				msawClass.activeflag = "F";
		}else if(child.toElement().tagName() == "Name") {
			msawClass.name = child.toElement().text();
		}else if(child.toElement().tagName() == "Polygon") {
			QDomNode node = child.firstChild();
			msawClass.type = "P";
			while(!node.isNull()) {
				if (node.toElement().tagName() == "Hmin") {
					msawClass.hmin = node.toElement().text();
					int temp = msawClass.hmin.count();
					msawClass.hmin.remove(temp-2,2);
				}else if(node.toElement().tagName() == "Hmax") {
					msawClass.hmax = node.toElement().text();
					int temp = msawClass.hmax.count();
					msawClass.hmax.remove(temp-2,2);
				}else if(node.toElement().tagName() == "Point") {
					QString tt = node.toElement().text();
					LatLon ll;
					QStringList list = tt.split(" ");   // 0 : Lat,   1: Lon,  2 : 0ft
					ll.lat = list.at(0);
					ll.lon = list.at(1);
					ll.alt = list.at(2);
					int temp = ll.alt.count();
					ll.alt.remove(temp-2,2);
					msawClass.polygon.point.append(ll);
				}
				node = node.nextSibling();
			}
		}else if(child.toElement().tagName() == "Sector") {
			msawClass.type = "S";
			QDomNode node = child.firstChild();
			while(!node.isNull()) {
				if (node.toElement().tagName() == "Hmin") {
					msawClass.hmin = node.toElement().text();
					int temp = msawClass.hmin.count();
					msawClass.hmin.remove(temp-2,2);
				}else if(node.toElement().tagName() == "Hmax") {
					msawClass.hmax = node.toElement().text();
					int temp = msawClass.hmax.count();
					msawClass.hmax.remove(temp-2,2);
				}else if(node.toElement().tagName() == "Center") {
					QString tt = node.toElement().text();
					LatLon ll;
					QStringList list = tt.split(" ");   // 0 : Lat,   1: Lon,  2 : 0ft
					ll.lat = list.at(0);
					ll.lon = list.at(1);
					ll.alt = list.at(2);
					int temp = ll.alt.count();
					ll.alt.remove(temp-2,2);
					msawClass.sector.center.append(ll);
				}else if(node.toElement().tagName() == "Az0") {
					msawClass.sector.az0 = node.toElement().text();
				}else if(node.toElement().tagName() == "Az1") {
					msawClass.sector.az1 = node.toElement().text();
				}else if(node.toElement().tagName() == "Rmin") {
					msawClass.sector.rmin = node.toElement().text();
					int temp = msawClass.sector.rmin.count();
					msawClass.sector.rmin.remove(temp-2,2);
				}else if(node.toElement().tagName() == "Rmax") {
					msawClass.sector.rmax = node.toElement().text();
					int temp = msawClass.sector.rmax.count();
					msawClass.sector.rmax.remove(temp-2,2);
				}

				node = node.nextSibling();
			}
		}
		child = child.nextSibling();
	}

/*	if(!checkType(msawClass.type)){
		QMessageBox::warning(this, tr("Failed! Type is not matched"), tr("Failed! Type is not matched %1").arg(txtFilePath->text()));
		return false;
	}*/

	//Primary Key check
	chk = checkDuplicationPK(msawClass.name, msawClass.area);
	if(!chk){
		QMessageBox::warning(this, tr("Failed! already exist"), tr("Failed! %1 already exists").arg(cmbArea->currentText()));
		return false;
	}

	//Insert Record to TB_SAFETYAREA
	chk = insertMainRecord(msawClass);
	if(!chk){
		qDebug()<<"insert Record Error";
		//db.rollback();
		return false;
	}

	if (msawClass.type == "P"){
		// About polygon data
		chk = insertPolygonRecord(msawClass);
		if (!chk){
			//db.rollback();
			return false;
		}
	}else if(msawClass.type == "S"){
		//About sector data
		chk = insertSectorRecord(msawClass);
		if (!chk){
			//db.rollback();
			return false;
		}
	}else return false;

	return chk;
}

/*
 *  Open the file
 */

bool XML_Parser::openFile(QFile & file)
{

	qDebug()<<"Open file process called";

	if (!file.open(QFile::ReadOnly | QFile::Text)) {
			// error message print
			qDebug() << "error : Text file not...";
			QMessageBox::warning(this, tr("Failed! Text file is not found"), tr("Failed! Text file is not found"));
			return false;
	}

	return true;

}

bool XML_Parser::checkDomDocument(QFile & file, QDomDocument & doc)
{

	QString errorStr;
	int errorLine;
	int errorColumn;

	if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
		{
			// error message print
			qDebug() << "error : " << errorStr;
			QMessageBox::warning(this, tr("Failed! %1").arg(errorStr), tr("Failed! %1").arg(errorStr));
			return false;
		}
	return true;
}






/*
 * 	Check types
 */
bool XML_Parser::checkType(QString & type)
{
	if(type == "S"){
		if(rdoPolygon->isChecked())
			return false;
	}else if(type == "P"){
		if(rdoSector->isChecked()) return false;
	}
	return true;
}

/*
 * 	Open the database
 */
bool XML_Parser::dbOpen()
{
	QString driver = "QOCI";
	QString hostName = "sms02";
	QString databaseName = "smsd";
	QSqlDatabase::removeDatabase(databaseName);
	{
		db = QSqlDatabase::addDatabase(driver, databaseName);
		db.setHostName(hostName);
		db.setDatabaseName(databaseName);
		db.setUserName(tr("adapt"));
		db.setPassword(tr("adapt"));
	}
	if (!db.open()) {
		QMessageBox::critical(this, tr("DataBase Open Error"), tr("DataBase Open Error!"));
		return false;
	}
	return true;
}


bool XML_Parser::checkDuplicationPK(const QString &name, const int area)
{
	//QSqlQuery * query = new QSqlQuery(db);
	QString sql;
	if (area == Output){
		sql = "select count(*) from TB_OUTAREA where OUT_NM='"+name+"'";
	}else if(area == QNH){
		sql = "select count(*) from TB_QNH where QNH_NM='"+name+"'";
	}else if(area == STCA){
		sql = "select count(*) from TB_SAFETYAREA where SAFETY_NM='"+name+"'";
	}else if(area == MSAW){
		sql = "select count(*) from TB_SAFETYAREA where SAFETY_NM='"+name+"'";
	}else if(area == BA){
		sql = "select count(*) from TB_BA where BA_NM='"+name+"'";
	}else{
		qDebug()<<"There is no area name";
		return false;
	}
		qDebug() << "name : " << name;
		qDebug() << "sql : " << sql;
		query->prepare(sql);
		query->exec();
		int cnt = 0;
		if (query->next()) {
			qDebug()<<"QeuryReslt : " <<query->value(0);
			cnt = query->value(0).toInt();
		}
		query->clear();
		if (cnt > 0) {
			qDebug()<<"Primary key exists "<<name;
			//db.rollback();
			return false;
		}else{
			qDebug() << "Primary key can be used"<<name;
			return true;
		}
}

bool XML_Parser::insertMainRecord(const Qnh & qnhClass)
{
	qDebug()<<"Insert Main Record called  AREA : "<<qnhClass.area;
	QString sql;

	if(qnhClass.area == QNH){
		sql = "INSERT INTO TB_QNH (QNH_NM, ACTIVE, TP, MIN_ALT, MAX_ALT, "
				"ICAO_NM, TR_LV, METEO_LAT, METEO_LON, METEO_UPD, QNH, "
				"METEO_TIMESTAMP, TEMPERATURE) ";
		sql += " VALUES ('"+qnhClass.name+"','"+qnhClass.activeflag+"','"+qnhClass.type+"', ";
		sql += " '"+qnhClass.hmin+"','"+qnhClass.hmax+"','"+qnhClass.icaocode+"','"+qnhClass.trlevel+"',";
		sql += " '"+qnhClass.meteoLAT+"','"+qnhClass.meteoLON+"','"+qnhClass.meteoupdatepriod+"', ";
		sql += " '"+qnhClass.qnh+"',to_date('"+qnhClass.meteotimestamp+"','yyyy-MM-dd HH24:MI:ss'),'"+qnhClass.temperature+"')";

		qDebug() << sql;
	}else return false;

	qDebug()<<"Creating SQL finished";
	query->prepare(sql);
	qDebug()<<"Preparing SQL finished";
	/*
	 * Execute query procedure
	 */
	query->exec();
	qDebug()<<"Insert SQL " << sql;
	if (query->lastError().type() != QSqlError::NoError) {
		QMessageBox::critical(this, tr("SQL Error"), query->lastError().text());
		//db.rollback();
		return false;
	}
	query->clear();
	return true;
}

bool XML_Parser::insertMainRecord(const Aoi & aoiClass)
{
	QString sql;
	if(aoiClass.area == Output){
		sql = "INSERT INTO TB_OUTAREA (OUT_NM, ACTIVE, TP, MIN_ALT, MAX_ALT, MASK, CAT)";
		sql += " VALUES ('"+aoiClass.name+"','"+aoiClass.activeflag+"','"+aoiClass.type+"','"+aoiClass.hmin+"','"+aoiClass.hmax+"','"+aoiClass.mask+"','"+aoiClass.cat+"')";
	}else return false;

	query->prepare(sql);
	/*
	 * Execute query procedure
	 */
	query->exec();
	qDebug()<<"Insert SQL " << sql;
	if (query->lastError().type() != QSqlError::NoError) {
		QMessageBox::critical(this, tr("SQL Error"), query->lastError().text());
		//db.rollback();
		return false;
	}

	return true;
}

bool XML_Parser::insertMainRecord(const Ba & baClass)
{
	QString sql;
	QString tmpsensor = "001_001,001_002,001_003,001_016,010_001,010_002,010_003";
	QStringList sensor = tmpsensor.split(",");
	static int count = 0;
/*
	if(baClass.area == BA){
		sql = "INSERT INTO TB_BA (BA_NM, TP, MASK, MAX_ALT, MIN_ALT, ACTIVE)";
		sql += " VALUES ('"+baClass.name+"','"+baClass.type+"','"+baClass.mask+"','"+baClass.hmax+"','"+baClass.hmin+"','"+baClass.activeflag+"')";
	}else return false;
*/

	if(baClass.area == BA){
		sql = "INSERT INTO TB_BA (BA_NM, TP, MASK, MAX_ALT, MIN_ALT, ACTIVE, SENSOR_ID)";
		sql += " VALUES (?, ?, ?, ?, ?, ?, ?)";
	}else return false;
	query->prepare(sql);
	query->addBindValue(baClass.name);
	query->addBindValue(baClass.type);
	query->addBindValue(baClass.mask);
	query->addBindValue(baClass.hmax);
	query->addBindValue(baClass.hmin);
	query->addBindValue(baClass.activeflag);
	if(count >= sensor.length()) count = 0;
	query->addBindValue(sensor.at(count++));
	/*
	 * Execute query procedure
	 */
	query->exec();
	query->clear();
	qDebug()<<"Insert SQL " << sql;
	if (query->lastError().type() != QSqlError::NoError) {
		QMessageBox::critical(this, tr("SQL Error"), query->lastError().text());
		//db.rollback();
		return false;
	}
	//count++;
	return true;
}

bool XML_Parser::insertMainRecord(const Stca & stcaClass)
{
	QString sql;
	if(stcaClass.area == STCA){
		sql = "INSERT INTO TB_SAFETYAREA (SAFETY_NM, ACTIVE, TP, MIN_ALT, MAX_ALT, CAT_TP,"
				"CAT, ACTIVE_TP, STRT_TM, END_TM, DAY)";
		sql += "VALUES ('"+stcaClass.name+"','"+stcaClass.activeflag+"','"+stcaClass.type+"'"
				",'"+stcaClass.hmin+"','"+stcaClass.hmax+"','"+stcaClass.stcatype+"','"+stcaClass.cat+"'"
				",'"+stcaClass.active_tp+"','"+stcaClass.strt_tm+"','"+stcaClass.end_tm+"','"+stcaClass.day+"')";
	}else return false;

	query->prepare(sql);
	/*
	 * Execute query procedure
	 */
	query->exec();
	query->clear();
	qDebug()<<"Insert SQL " << sql;
	if (query->lastError().type() != QSqlError::NoError) {
		QMessageBox::critical(this, tr("SQL Error"), query->lastError().text());
		//db.rollback();
		return false;
	}

	return true;
}

bool XML_Parser::insertMainRecord(const Msaw & msawClass)
{
	QString sql;
	if(msawClass.area == MSAW){
		sql = "INSERT INTO TB_SAFETYAREA (SAFETY_NM, ACTIVE, TP, MIN_ALT, MAX_ALT, CAT_TP,"
				"CAT, ACTIVE_TP, STRT_TM, END_TM, DAY)";
		sql += "VALUES ('"+msawClass.name+"','"+msawClass.activeflag+"','"+msawClass.type+"'"
				",'"+msawClass.hmin+"','"+msawClass.hmax+"','"+msawClass.msawtype+"','"+msawClass.cat+"'"
				",'"+msawClass.active_tp+"','"+msawClass.strt_tm+"','"+msawClass.end_tm+"','"+msawClass.day+"')";
	}else return false;

	query->prepare(sql);
	/*
	 * Execute query procedure
	 */
	query->exec();
	qDebug()<<"Insert SQL " << sql;
	if (query->lastError().type() != QSqlError::NoError) {
		QMessageBox::critical(this, tr("SQL Error"), query->lastError().text());
		//db.rollback();
		return false;
	}

	return true;
}



/*
 * Function for Polygon Records
 */
bool XML_Parser::insertPolygonRecord(const Aoi &aoiClass)
{
	int idx = 1;
	QString sql;
	bool isError = false;
	AoiPolygon polygon = aoiClass.polygon;

	for (int i=0; i<polygon.point.count(); i++) {
		LatLon latlon = polygon.point.at(i);
		sql = "INSERT INTO TB_OUTPOLYGON (OUT_NM, IDX, LAT, LON, ALT, CAT) ";
		sql += " VALUES ( '"+aoiClass.name+"', '"+QString("%1").arg(idx)+"', ";
		sql += " '"+latlon.lat+"', '"+latlon.lon+"','"+latlon.alt+"','"+aoiClass.cat+"')";
		query->prepare(sql);

		query->exec();
		qDebug()<<"Insert Polygon SQL " << sql;
		if (query->lastError().type() != QSqlError::NoError) {
			qDebug()<<"SQL Error : "<<query->lastError().text();
			isError = true;
			break;
		}
		idx++;
	}
	if (isError == true){
		QMessageBox::critical(this, tr("SQL Error"), query->lastError().text());
		return false;
	}
	return true;
}

bool XML_Parser::insertPolygonRecord(const Ba &baClass)
{
	int idx = 1;
	QString sql;
	bool isError = false;
	BaPolygon polygon = baClass.polygon;

	for (int i=0; i<polygon.point.count(); i++) {
		LatLon latlon = polygon.point.at(i);
		sql = "INSERT INTO TB_BAPOLYGON (BA_NM, IDX, LAT, LON) ";
		sql += " VALUES ( '"+baClass.name+"', '"+QString("%1").arg(idx)+"', ";
		sql += " '"+latlon.lat+"', '"+latlon.lon+"')";
		query->prepare(sql);
		QSqlDatabase::database().transaction();
		query->exec();
		qDebug()<<"Insert Polygon SQL " << sql;
		if (query->lastError().type() != QSqlError::NoError) {
			qDebug()<<"SQL Error : "<<query->lastError().text();
			isError = true;
			break;
		}
		idx++;
	}
	if (isError == true){
		QMessageBox::critical(this, tr("SQL Error"), query->lastError().text());
		return false;
	}
	return true;
}

bool XML_Parser::insertPolygonRecord(const Qnh &qnhClass)
{
	int idx = 1;
	QString sql;
	bool isError = false;
	QnhPolygon polygon = qnhClass.polygon;

	for (int i=0; i<polygon.pointList.count(); i++) {
		LatLon latlon = polygon.pointList.at(i);
		sql = "INSERT INTO TB_QNHPOLYGON (QNH_NM, IDX, LAT, LON ) ";
		sql += " VALUES ( '"+qnhClass.name+"', '"+QString("%1").arg(idx)+"', ";
		sql += " '"+latlon.lat+"', '"+latlon.lon+"' )";
		query->prepare(sql);
		QSqlDatabase::database().transaction();
		query->exec();
		qDebug()<<"Insert Polygon SQL " << sql;
		if (query->lastError().type() != QSqlError::NoError) {
			qDebug()<<"SQL Error : "<<query->lastError().text();
			isError = true;
			break;
		}
		idx++;
	}
	if (isError == true){
		QMessageBox::critical(this, tr("SQL Error"), query->lastError().text());
		return false;
	}
	return true;
}

bool XML_Parser::insertPolygonRecord(const Stca &stcaClass)
{
	int idx = 1;
	QString sql;
	bool isError = false;
	StcaPolygon polygon = stcaClass.polygon;

	for (int i=0; i<polygon.point.count(); i++) {
		LatLon latlon = polygon.point.at(i);
		sql = "INSERT INTO TB_SAFETYPOLYGON (SAFETY_NM, IDX, LAT, LON, ALT, CAT) ";
		sql += " VALUES ( '"+stcaClass.name+"', '"+QString("%1").arg(idx)+"', ";
		sql += " '"+latlon.lat+"', '"+latlon.lon+"','"+latlon.alt+"','"+stcaClass.cat+"')";
		query->prepare(sql);
		QSqlDatabase::database().transaction();
		query->exec();
		qDebug()<<"Insert Polygon SQL " << sql;
		if (query->lastError().type() != QSqlError::NoError) {
			qDebug()<<"SQL Error : "<<query->lastError().text();
			isError = true;
			break;
		}
		idx++;
	}
	if (isError == true){
		QMessageBox::critical(this, tr("SQL Error"), query->lastError().text());
		return false;
	}
	return true;
}


bool XML_Parser::insertPolygonRecord(const Msaw &msawClass)
{
	int idx = 1;
	QString sql;
	bool isError = false;
	MsawPolygon polygon = msawClass.polygon;

	for (int i=0; i<polygon.point.count(); i++) {
		LatLon latlon = polygon.point.at(i);
		sql = "INSERT INTO TB_SAFETYPOLYGON (SAFETY_NM, IDX, LAT, LON, ALT, CAT) ";
		sql += " VALUES ( '"+msawClass.name+"', '"+QString("%1").arg(idx)+"', ";
		sql += " '"+latlon.lat+"', '"+latlon.lon+"','"+latlon.alt+"','"+msawClass.cat+"')";
		query->prepare(sql);
		QSqlDatabase::database().transaction();
		query->exec();
		qDebug()<<"Insert Polygon SQL " << sql;
		if (query->lastError().type() != QSqlError::NoError) {
			qDebug()<<"SQL Error : "<<query->lastError().text();
			isError = true;
			break;
		}
		idx++;
	}
	if (isError == true){
		QMessageBox::critical(this, tr("SQL Error"), query->lastError().text());
		return false;
	}
	return true;
}


/*
 * Function for Sector Records
 */

bool XML_Parser::insertSectorRecord(const Qnh &qnhClass)
{
	QString sql;
	bool isError = false;
	QnhSector qnhSector = qnhClass.sector;

	for (int i=0; i<qnhSector.center.count(); i++) {
		LatLon latlon = qnhSector.center.at(i);
		sql = "INSERT INTO TB_QNHSECTOR (QNH_NM, CENTER_LAT, CENTER_LON, AZ0, AZ1, RMIN, RMAX) ";
		sql += " VALUES ('"+qnhClass.name+"', '"+latlon.lat+"', '"+latlon.lon+"', '"+qnhSector.az0+"','"+qnhSector.az1+"','"+qnhSector.rmin+"','"+qnhSector.rmax+"')";
		query->prepare(sql);
		query->exec();
		qDebug()<<"Insert Sector SQL " << sql;
		if (query->lastError().type() != QSqlError::NoError) {
			qDebug()<<"SQL Error : "<<query->lastError().text();
			isError = true;
			break;
		}
	}
	if (isError == true){
		QMessageBox::critical(this, tr("SQL Error"), query->lastError().text());
		return false;
	}
	return true;
}
bool XML_Parser::insertSectorRecord(const Aoi &aoiClass)
{
	QString sql;
	bool isError = false;
	AoiSector aoiSector = aoiClass.sector;

	for (int i=0; i<aoiSector.center.count(); i++) {
		LatLon latlon = aoiSector.center.at(i);
		sql = "INSERT INTO TB_OUTSECTOR (OUT_NM, CENTER_LAT, CENTER_LON, CENTER_ALT, AZ0, AZ1, RMIN, RMAX, CAT) ";
		sql += " VALUES ('"+aoiClass.name+"', '"+latlon.lat+"', '"+latlon.lon+"', '"+latlon.alt+"' , '"+aoiSector.az0+"','"+aoiSector.az1+"','"+aoiSector.rmin+"','"+aoiSector.rmax+"','"+aoiClass.cat+"')";
		query->prepare(sql);
		query->exec();
		qDebug()<<"Insert Sector SQL " << sql;
		if (query->lastError().type() != QSqlError::NoError) {
			qDebug()<<"SQL Error : "<<query->lastError().text();
			isError = true;
			break;
		}
	}
	if (isError == true){
		QMessageBox::critical(this, tr("SQL Error"), query->lastError().text());
		return false;
	}
	return true;
}

bool XML_Parser::insertSectorRecord(const Ba &baClass)
{
	QString sql;
	bool isError = false;
	BaSector baSector = baClass.sector;

	for (int i=0; i<baSector.center.count(); i++) {
		LatLon latlon = baSector.center.at(i);
		sql = "INSERT INTO TB_BASECTOR (CENTER_LAT, CENTER_LON, AZ0, AZ1, RMAX, RMIN, BA_NM) ";
		sql += " VALUES ('"+latlon.lat+"', '"+latlon.lon+"', '"+baSector.az0+"','"+baSector.az1+"','"+baSector.rmax+"','"+baSector.rmin+"','"+baClass.name+"')";
		query->prepare(sql);
		query->exec();
		qDebug()<<"Insert Sector SQL " << sql;
		if (query->lastError().type() != QSqlError::NoError) {
			qDebug()<<"SQL Error : "<<query->lastError().text();
			isError = true;
			break;
		}
	}
	if (isError == true){
		QMessageBox::critical(this, tr("SQL Error"), query->lastError().text());
		return false;
	}
	return true;
}

bool  XML_Parser::insertSectorRecord(const Stca &stcaClass)
{
	QString sql;
	bool isError = false;
	StcaSector stcaSector = stcaClass.sector;

	for (int i=0; i<stcaSector.center.count(); i++) {
		LatLon latlon = stcaSector.center.at(i);
		sql = "INSERT INTO TB_SAFETYSECTOR (SAFETY_NM, CENTER_LAT, CENTER_LON, CENTER_ALT, AZ0, AZ1, RMIN, RMAX, CAT) ";
		sql += " VALUES ('"+stcaClass.name+"', '"+latlon.lat+"', '"+latlon.lon+"', '"+latlon.alt+"',' "+stcaSector.az0+"','"+stcaSector.az1+"','"+stcaSector.rmin+"','"+stcaSector.rmax+"','"+stcaClass.cat+"')";
		query->prepare(sql);
		query->exec();
		qDebug()<<"Insert Sector SQL " << sql;
		if (query->lastError().type() != QSqlError::NoError) {
			qDebug()<<"SQL Error : "<<query->lastError().text();
			isError = true;
			break;
		}
	}
	if (isError == true){
		QMessageBox::critical(this, tr("SQL Error"), query->lastError().text());
		return false;
	}
	return true;

}

bool  XML_Parser::insertSectorRecord(const Msaw &msawClass)
{
	QString sql;
	bool isError = false;
	MsawSector msawSector = msawClass.sector;

	for (int i=0; i<msawSector.center.count(); i++) {
		LatLon latlon = msawSector.center.at(i);
		sql = "INSERT INTO TB_SAFETYSECTOR (SAFETY_NM, CENTER_LAT, CENTER_LON, CENTER_ALT, AZ0, AZ1, RMIN, RMAX, CAT) ";
		sql += " VALUES ('"+msawClass.name+"', '"+latlon.lat+"', '"+latlon.lon+"', '"+latlon.alt+"',' "+msawSector.az0+"','"+msawSector.az1+"','"+msawSector.rmin+"','"+msawSector.rmax+"','"+msawClass.cat+"')";
		query->prepare(sql);
		query->exec();
		qDebug()<<"Insert Sector SQL " << sql;
		if (query->lastError().type() != QSqlError::NoError) {
			qDebug()<<"SQL Error : "<<query->lastError().text();
			isError = true;
			break;
		}
	}
	if (isError == true){
		QMessageBox::critical(this, tr("SQL Error"), query->lastError().text());
		return false;
	}
	return true;

}
bool XML_Parser::deleteCreatedRecords()
{
	qDebug()<<"Delete Records called...";
	db.transaction();
	QSqlQuery * query = new QSqlQuery(db);
	QString sql;

	/*sql = "DELETE FROM TB_QNHPOLYGON WHERE QNH_NM = 'RKSD'";
	query->prepare(sql);*/
	query->exec("DELETE FROM TB_QNHPOLYGON WHERE QNH_NM = 'RKSD'");
	//sql += "DELETE FROM TB_QNHPSECTOR WHERE QNH_NM = 'UUEE'";
	query->exec("DELETE FROM TB_QNHSECTOR WHERE QNH_NM = 'UUEE'");
	query->exec("DELETE FROM TB_QNH WHERE QNH_NM = 'RKSD'");
	query->exec("DELETE FROM TB_QNH WHERE QNH_NM = 'UUEE'");

	//Output records
	query->exec("DELETE FROM TB_OUTPOLYGON WHERE OUT_NM = 'AOI_2'");
	query->exec("DELETE FROM TB_OUTAREA WHERE OUT_NM = 'AOI_2'");

	//Stca records
	query->exec("DELETE FROM TB_SAFETYPOLYGON WHERE SAFETY_NM = 'STCA_Test'");
	query->exec("DELETE FROM TB_SAFETYAREA WHERE SAFETY_NM = 'STCA_Test'");

	//MSAW records
	query->exec("DELETE FROM TB_SAFETYPOLYGON WHERE SAFETY_NM = 'MSAW_Test1'");
	query->exec("DELETE FROM TB_SAFETYAREA WHERE SAFETY_NM = 'MSAW_Test1'");

	if (query->lastError().type() != QSqlError::NoError) {
		qDebug()<<"SQL Error : "<<query->lastError().text();
		db.rollback();
		return false;
	}

	QMessageBox::information(this,tr("Delete all records created"),tr("Delete all records created"));
	db.commit();
	return true;
}


bool XML_Parser::readQnhTable()
{
	qDebug()<<"ReadQNHTABLE Start";
	QString sql;
	bool chk;
	int cnt;				// the number of records.
	sql = "SELECT * FROM TB_QNH";
	QVector<QStringList> Mainrecords;

	//Retrieve records from TB_QNH
	chk = retrieverecords(Mainrecords, sql);
	if(!chk){
		qDebug()<<"Retrieve Record Error....";
		return false;
	}
	cnt = Mainrecords.count();					//set the count of TB_QNH

	for(int i = 0 ; i < cnt ; i++)
	{
		qDebug()<<Mainrecords.at(i);
	}

	//Retrieve polygon records from TB_QNHPOLYGON
	sql.clear();
	sql = "SELECT * FROM TB_QNHPOLYGON";
	QVector<QStringList> Polygonrecords;
	chk = retrieverecords(Polygonrecords, sql);
	if(!chk){
			qDebug()<<"Retrieve Record Error....";
			return false;
	}
	int pcnt = Polygonrecords.count();		//for count of polygon records
	for(int i = 0 ; i < pcnt ; i++)
	{
		qDebug()<<Polygonrecords.at(i);
	}

	//Retrieve sector records from TB_QNHSECTOR
	sql.clear();
	sql = "SELECT * FROM TB_QNHSECTOR";
	QVector<QStringList> Sectorrecords;
	chk = retrieverecords(Sectorrecords, sql);
	if(!chk){
			qDebug()<<"Retrieve Record Error....";
			return false;
	}

	int scnt = Sectorrecords.count();		//for count of sector records
	for(int i = 0 ; i < scnt ; i++)
	{
		qDebug()<<Sectorrecords.at(i);
	}

	QVector<Qnh> qnhClass;
	QString dirname = setDirectory();		//get the directory name to save xml file

	//save records into qnhClass.
	for(int i = 0; i < cnt ; i++){
		qDebug()<<"Call main procedure for qnh";
		Qnh tmpclass;
		tmpclass.name = Mainrecords.at(i).at(0);
		tmpclass.activeflag = Mainrecords.at(i).at(1);
		tmpclass.type = Mainrecords.at(i).at(2);
		tmpclass.hmin = Mainrecords.at(i).at(3);
		tmpclass.hmax = Mainrecords.at(i).at(4);
		tmpclass.icaocode = Mainrecords.at(i).at(5);
		tmpclass.trlevel = Mainrecords.at(i).at(6);
		tmpclass.meteoLAT = Mainrecords.at(i).at(7);
		tmpclass.meteoLON = Mainrecords.at(i).at(8);
		tmpclass.meteoupdatepriod = Mainrecords.at(i).at(9);
		tmpclass.qnh = Mainrecords.at(i).at(10);
		tmpclass.meteotimestamp = Mainrecords.at(i).at(11);
		tmpclass.meteotimestamp.replace("T"," ");
		tmpclass.temperature = Mainrecords.at(i).at(12);
		if(tmpclass.type == "P" && Polygonrecords.count() > 0){
			int p = 0;
			while(p < Polygonrecords.count()){
				qDebug()<<"Call while procedure";
				if(Polygonrecords.at(p).at(0) == tmpclass.name){
					LatLon ll;
					ll.lat = Polygonrecords.at(p).at(2);
					ll.lon = Polygonrecords.at(p).at(3);
					tmpclass.polygon.pointList.append(ll);
					qDebug()<<"Polygon index : "<<p;
				}
				p++;
			}
		}else if(tmpclass.type == "S" && Sectorrecords.count() > 0){
			bool isFinished = false;
			int s = 0;
			while(s < Sectorrecords.count()){
				qDebug()<<"Call Sector while procedure";
				if(Sectorrecords.at(s).at(0) == tmpclass.name){
					if(isFinished == false){
						tmpclass.sector.az0 = Sectorrecords.at(s).at(3);
						tmpclass.sector.az1 = Sectorrecords.at(s).at(4);
						tmpclass.sector.rmin = Sectorrecords.at(s).at(5);
						tmpclass.sector.rmax = Sectorrecords.at(s).at(6);
						isFinished = true;
					}
					LatLon ll;
					ll.lat = Sectorrecords.at(s).at(1);
					ll.lon = Sectorrecords.at(s).at(2);
					tmpclass.sector.center.append(ll);
					qDebug()<<"Sector index : "<<s;
				}
				s++;
			}
		}

		qnhClass.append(tmpclass);
		qDebug()<<"-------After inserting records to qnhClass------";
		qDebug()<<qnhClass.at(i).name<<qnhClass.at(i).activeflag<<qnhClass.at(i).type;
		if(!saveQnhXmlFile(qnhClass.at(i),dirname)){
			return false;
		}
	}

	//QMessageBox::information(this,tr("Completed"),tr("It is completed to make xml files.."));
	return true;
}

bool XML_Parser::readOutputTable()
{
	qDebug()<<"ReadOutputTABLE Start";
	QString sql;
	bool chk;
	int cnt;				// the number of records.
	sql = "SELECT * FROM TB_OUTAREA";
	QVector<QStringList> Mainrecords;


	//Retrieve records from TB_QNH
	chk = retrieverecords(Mainrecords, sql);
	if(!chk){
		qDebug()<<"Retrieve Record Error....";
		return false;
	}
	cnt = Mainrecords.count();

	for(int i = 0 ; i < cnt ; i++)
	{
		qDebug()<<Mainrecords.at(i);
	}
	//Retrieve polygon records from TB_QNHPOLYGON
	sql.clear();
	sql = "SELECT * FROM TB_OUTPOLYGON";
	QVector<QStringList> Polygonrecords;
	chk = retrieverecords(Polygonrecords, sql);
	if(!chk){
			qDebug()<<"Retrieve Record Error....";
			return false;
	}
	//Retrieve polygon records from TB_QNHPOLYGON
	sql.clear();
	sql = "SELECT * FROM TB_OUTSECTOR";
	QVector<QStringList> Sectorrecords;
	chk = retrieverecords(Sectorrecords, sql);
	if(!chk){
			qDebug()<<"Retrieve Record Error....";
			return false;
	}

	QVector<Aoi> aoiClass;
	//Open directory for save
	QString dirname = setDirectory();


	for(int i = 0; i < cnt ; i++){
		qDebug()<<"Call main procedure for Output";
		Aoi tmpclass;
		tmpclass.name = Mainrecords.at(i).at(0);
		tmpclass.activeflag = Mainrecords.at(i).at(1);
		tmpclass.type = Mainrecords.at(i).at(2);
		tmpclass.hmin = Mainrecords.at(i).at(3);
		tmpclass.hmax = Mainrecords.at(i).at(4);
		tmpclass.mask = Mainrecords.at(i).at(5);
		tmpclass.cat = Mainrecords.at(i).at(6);
		qDebug()<<"index : "<<i<<" Type : "<<tmpclass.type;
		qDebug()<<"Count : "<<Polygonrecords.count();
		if(tmpclass.type == "P"&& Sectorrecords.count() > 0){
			int p = 0;
			while(p < Polygonrecords.count()){
				qDebug()<<"Call Polygon procedure";
				if(Polygonrecords.at(p).at(0) == tmpclass.name){
					LatLon ll;
					ll.lat = Polygonrecords.at(p).at(2);
					ll.lon = Polygonrecords.at(p).at(3);
					ll.alt = Polygonrecords.at(p).at(4);
					tmpclass.polygon.point.append(ll);
					qDebug()<<"index : "<<p;
				}
			p++;
			}
		}else if(tmpclass.type == "S" && Sectorrecords.count() > 0){
			bool isFinished = false;
			int s = 0;
			while(s < Sectorrecords.count()){
				qDebug()<<"Call Sector while procedure";
				if(Sectorrecords.at(s).at(0) == tmpclass.name){
					if(isFinished == false){
						tmpclass.sector.az0 = Sectorrecords.at(s).at(4);
						tmpclass.sector.az1 = Sectorrecords.at(s).at(5);
						tmpclass.sector.rmin = Sectorrecords.at(s).at(6);
						tmpclass.sector.rmax = Sectorrecords.at(s).at(7);
						isFinished = true;
					}
					LatLon ll;
					ll.lat = Sectorrecords.at(s).at(1);
					ll.lon = Sectorrecords.at(s).at(2);
					ll.alt = Sectorrecords.at(s).at(3);
					tmpclass.sector.center.append(ll);
					qDebug()<<"index : "<<s;
				}
				s++;
			}
		}

		aoiClass.append(tmpclass);
		qDebug()<<"-------After inserting records to outputClass------";
		//qDebug()<<qnhClass.at(i).name<<qnhClass.at(i).activeflag<<qnhClass.at(i).type;
		if(!saveOutputXmlFile(aoiClass.at(i),dirname))
			return false;
	}


	return true;
}

bool XML_Parser::readBaTable()
{
	qDebug()<<"ReadOutputTABLE Start";
	QString sql;
	bool chk;
	int cnt;				// the number of records.
	sql = "SELECT * FROM TB_BA";
	QVector<QStringList> Mainrecords;


	//Retrieve records from TB_QNH
	chk = retrieverecords(Mainrecords, sql);
	if(!chk){
		qDebug()<<"Retrieve Record Error....";
		return false;
	}
	cnt = Mainrecords.count();

	for(int i = 0 ; i < cnt ; i++)
	{
		qDebug()<<Mainrecords.at(i);
	}
	//Retrieve polygon records from TB_QNHPOLYGON
	sql.clear();
	sql = "SELECT * FROM TB_BAPOLYGON";
	QVector<QStringList> Polygonrecords;
	chk = retrieverecords(Polygonrecords, sql);
	if(!chk){
			qDebug()<<"Retrieve Record Error....";
			return false;
	}
	//Retrieve polygon records from TB_QNHPOLYGON
	sql.clear();
	sql = "SELECT * FROM TB_BASECTOR";
	QVector<QStringList> Sectorrecords;
	chk = retrieverecords(Sectorrecords, sql);
	if(!chk){
			qDebug()<<"Retrieve Record Error....";
			return false;
	}
	QVector<Ba> baClass;

	//Open directory for save
	QString dirname = setDirectory();
	for(int i = 0; i < cnt ; i++){
		qDebug()<<"Call main procedure for Output";
		Ba tmpclass;
		tmpclass.name = Mainrecords.at(i).at(0);
		tmpclass.type = Mainrecords.at(i).at(1);
		tmpclass.mask = Mainrecords.at(i).at(2);
		tmpclass.hmin = Mainrecords.at(i).at(4);
		tmpclass.hmax = Mainrecords.at(i).at(3);
		tmpclass.activeflag = Mainrecords.at(i).at(5);
		qDebug()<<"index : "<<i<<" Type : "<<tmpclass.type;
		qDebug()<<"Count : "<<Polygonrecords.count();
		if(tmpclass.type == "P" && Polygonrecords.count() > 0){
			int p = 0;
			while(p < Polygonrecords.count()){
				qDebug()<<"Call Polygon procedure";
				if(Polygonrecords.at(p).at(0) == tmpclass.name){
					LatLon ll;
					ll.lat = Polygonrecords.at(p).at(2);
					ll.lon = Polygonrecords.at(p).at(3);
					//ll.alt = Polygonrecords.at(p).at(4);
					tmpclass.polygon.point.append(ll);
					qDebug()<<"index : "<<p;
				}
			p++;
			}
		}else if(tmpclass.type == "S" && Sectorrecords.count() > 0){
			int s = 0;
			bool isFinished = false;
			while(s < Sectorrecords.count()){
				qDebug()<<"Call Sector while procedure";
				if(Sectorrecords.at(s).at(6) == tmpclass.name){
					if(isFinished == false){
						tmpclass.sector.az0 = Sectorrecords.at(s).at(2);
						tmpclass.sector.az1 = Sectorrecords.at(s).at(3);
						tmpclass.sector.rmin = Sectorrecords.at(s).at(5);
						tmpclass.sector.rmax = Sectorrecords.at(s).at(4);
						isFinished = true;
					}
					LatLon ll;
					ll.lat = Sectorrecords.at(s).at(0);
					ll.lon = Sectorrecords.at(s).at(1);
					//ll.alt = Sectorrecords.at(s).at(3);
					tmpclass.sector.center.append(ll);
					qDebug()<<"index : "<<s;
				}
				s++;
			}
		}

		baClass.append(tmpclass);
		qDebug()<<"-------After inserting records to outputClass------";
		//qDebug()<<qnhClass.at(i).name<<qnhClass.at(i).activeflag<<qnhClass.at(i).type;
		saveBaXmlFile(baClass.at(i),dirname);
	}


	return true;
}


bool XML_Parser::readStcaTable()
{
	qDebug()<<"ReadOutputTABLE Start";
	QString sql;
	bool chk;
	int cnt;				// the number of records.
	sql = "SELECT * FROM TB_SAFETYAREA WHERE CAT='S'";
	QVector<QStringList> Mainrecords;


	//Retrieve records from TB_QNH
	chk = retrieverecords(Mainrecords, sql);
	if(!chk){
		qDebug()<<"Retrieve Record Error....";
		return false;
	}
	cnt = Mainrecords.count();

	for(int i = 0 ; i < cnt ; i++)
	{
		qDebug()<<Mainrecords.at(i);
	}
	//Retrieve polygon records from TB_QNHPOLYGON
	sql.clear();
	sql = "SELECT * FROM TB_SAFETYPOLYGON WHERE CAT='S'";
	QVector<QStringList> Polygonrecords;
	chk = retrieverecords(Polygonrecords, sql);
	if(!chk){
			qDebug()<<"Retrieve Record Error....";
			return false;
	}

	int pcnt = Polygonrecords.count();
	for(int i = 0 ; i < pcnt ; i++)
	{
		qDebug()<<Polygonrecords.at(i);
	}

	//Retrieve polygon records from TB_QNHPOLYGON
	sql.clear();
	sql = "SELECT * FROM TB_SAFETYSECTOR WHERE CAT='S'";
	QVector<QStringList> Sectorrecords;
	chk = retrieverecords(Sectorrecords, sql);
	if(!chk){
			qDebug()<<"Retrieve Record Error....";
			return false;
	}

	QVector<Stca> stcaClass;
	//Open directory for save
	QString dirname = setDirectory();


	for(int i = 0; i < cnt ; i++){
		qDebug()<<"Call main procedure for STCA";
		Stca tmpclass;
		tmpclass.name = Mainrecords.at(i).at(0);
		tmpclass.activeflag = Mainrecords.at(i).at(1);
		tmpclass.type = Mainrecords.at(i).at(2);
		tmpclass.hmin = Mainrecords.at(i).at(3);
		tmpclass.hmax = Mainrecords.at(i).at(4);
		tmpclass.stcatype = Mainrecords.at(i).at(5);
		tmpclass.cat = Mainrecords.at(i).at(6);
		tmpclass.active_tp = Mainrecords.at(i).at(7);
		tmpclass.strt_tm = Mainrecords.at(i).at(8);
		tmpclass.end_tm = Mainrecords.at(i).at(9);
		tmpclass.day = Mainrecords.at(i).at(10);

		qDebug()<<"index : "<<i<<" Type : "<<tmpclass.type;
		qDebug()<<"Count : "<<Polygonrecords.count();
		if(tmpclass.type == "P" && Polygonrecords.count() > 0){
			int p = 0;
			while(p < Polygonrecords.count()){
				qDebug()<<"Call Polygon procedure";
				if(Polygonrecords.at(p).at(0) == tmpclass.name){
					LatLon ll;
					ll.lat = Polygonrecords.at(p).at(2);
					ll.lon = Polygonrecords.at(p).at(3);
					ll.alt = Polygonrecords.at(p).at(4);
					tmpclass.polygon.point.append(ll);
					qDebug()<<"index : "<<p;
					p++;
				}else p++;
			}
		}else if(tmpclass.type == "S" && Sectorrecords.count() > 0){
			bool isFinished = false;
			int s = 0;
			while(s < Sectorrecords.count()){
				qDebug()<<"Call Sector while procedure";
				if(Sectorrecords.at(s).at(0) == tmpclass.name){
					if(isFinished == false){
						tmpclass.sector.az0 = Sectorrecords.at(s).at(4);
						tmpclass.sector.az1 = Sectorrecords.at(s).at(5);
						tmpclass.sector.rmin = Sectorrecords.at(s).at(6);
						tmpclass.sector.rmax = Sectorrecords.at(s).at(7);
						isFinished = true;
					}
					LatLon ll;
					ll.lat = Sectorrecords.at(s).at(1);
					ll.lon = Sectorrecords.at(s).at(2);
					ll.alt = Sectorrecords.at(s).at(3);
					tmpclass.sector.center.append(ll);
					qDebug()<<"index : "<<s;
				}
				s++;
			}
		}
		stcaClass.append(tmpclass);
		qDebug()<<"-------After inserting records to outputClass------";
		//qDebug()<<qnhClass.at(i).name<<qnhClass.at(i).activeflag<<qnhClass.at(i).type;
		if(!saveStcaXmlFile(stcaClass.at(i),dirname))
		{
			qDebug()<<"save Stca Xml File Failed..";
			return false;
		}
	}


	return true;
}

bool XML_Parser::readMsawTable()
{
	qDebug()<<"Read MSAW TABLE Start";
	QString sql;
	bool chk;
	int cnt;				// the number of records.
	sql = "SELECT * FROM TB_SAFETYAREA WHERE CAT='M'";
	QVector<QStringList> Mainrecords;


	//Retrieve records from TB_QNH
	chk = retrieverecords(Mainrecords, sql);
	if(!chk){
		qDebug()<<"Retrieve Record Error....";
		return false;
	}
	cnt = Mainrecords.count();

	for(int i = 0 ; i < cnt ; i++)
	{
		qDebug()<<Mainrecords.at(i);
	}
	//Retrieve polygon records from TB_QNHPOLYGON
	sql.clear();
	sql = "SELECT * FROM TB_SAFETYPOLYGON WHERE CAT='M'";
	QVector<QStringList> Polygonrecords;
	chk = retrieverecords(Polygonrecords, sql);
	if(!chk){
			qDebug()<<"Retrieve Record Error....";
			return false;
	}

	int pcnt = Polygonrecords.count();
	for(int i = 0 ; i < pcnt ; i++)
	{
		//qDebug()<<Polygonrecords.at(i);
	}

	//Retrieve polygon records from TB_QNHPOLYGON
	sql.clear();
	sql = "SELECT * FROM TB_SAFETYSECTOR WHERE CAT='M'";
	QVector<QStringList> Sectorrecords;
	chk = retrieverecords(Sectorrecords, sql);
	if(!chk){
			qDebug()<<"Retrieve Record Error....";
			return false;
	}
	int scnt = Sectorrecords.count();
	for(int i = 0 ; i < scnt ; i++)
	{
		qDebug()<<Sectorrecords.at(i);
	}

	QVector<Msaw> msawClass;
	//Open directory for save
	QString dirname = setDirectory();

	for(int i = 0; i < cnt ; i++){
		qDebug()<<"Call main procedure for MSAW";
		Msaw tmpclass;
		tmpclass.name = Mainrecords.at(i).at(0);
		tmpclass.activeflag = Mainrecords.at(i).at(1);
		tmpclass.type = Mainrecords.at(i).at(2);
		tmpclass.hmin = Mainrecords.at(i).at(3);
		tmpclass.hmax = Mainrecords.at(i).at(4);
		tmpclass.msawtype = Mainrecords.at(i).at(5);
		tmpclass.cat = Mainrecords.at(i).at(6);
		tmpclass.active_tp = Mainrecords.at(i).at(7);
		tmpclass.strt_tm = Mainrecords.at(i).at(8);
		tmpclass.end_tm = Mainrecords.at(i).at(9);
		tmpclass.day = Mainrecords.at(i).at(10);

		/*qDebug()<<"index : "<<i<<" Type : "<<tmpclass.type;
		qDebug()<<"Count : "<<Polygonrecords.count();*/
		if(tmpclass.type == "P" && Polygonrecords.count() > 0){
			int p = 0;
			while(p < Polygonrecords.count()){
				//qDebug()<<"Call Polygon procedure";
				if(Polygonrecords.at(p).at(0) == tmpclass.name){
					LatLon ll;
					ll.lat = Polygonrecords.at(p).at(2);
					ll.lon = Polygonrecords.at(p).at(3);
					ll.alt = Polygonrecords.at(p).at(4);
					tmpclass.polygon.point.append(ll);
					//qDebug()<<"index : "<<p;
					p++;
				}else p++;
			}
		}else if(tmpclass.type == "S" && Sectorrecords.count() > 0){
			//qDebug()<<"Name : "<<tmpclass.name;
			bool isFinished = false;		//false : no finish,  true : finished
			int s = 0;
			while(s < Sectorrecords.count()){
				//qDebug()<<"Call Sector while procedure";
				qDebug()<<"Sector at:"<<s<<"Name : "<<Sectorrecords.at(s).at(0);
				if(Sectorrecords.at(s).at(0) == tmpclass.name){
					//qDebug()<<"called if function..";
					if(isFinished == false){
						//qDebug()<<"called az0 process..";
						tmpclass.sector.az0 = Sectorrecords.at(s).at(4);
						tmpclass.sector.az1 = Sectorrecords.at(s).at(5);
						tmpclass.sector.rmin = Sectorrecords.at(s).at(6);
						tmpclass.sector.rmax = Sectorrecords.at(s).at(7);
						isFinished = true;
						//qDebug()<<"finished az0 process..";
					}
					LatLon ll;
					ll.lat = Sectorrecords.at(s).at(1);
					ll.lon = Sectorrecords.at(s).at(2);
					ll.alt = Sectorrecords.at(s).at(3);
					tmpclass.sector.center.append(ll);
					//qDebug()<<"idx : "<<s<<"Center : "<<tmpclass.sector.center.at(s).lat;
				}
				s++;
			}
		}

		msawClass.append(tmpclass);
		//qDebug()<<"-------After inserting records to MsawClass------";
		//qDebug()<<qnhClass.at(i).name<<qnhClass.at(i).activeflag<<qnhClass.at(i).type;
		if(!saveMsawXmlFile(msawClass.at(i),dirname)) return false;
	}


	return true;
}

bool XML_Parser::retrieverecords(QVector<QStringList> & records, QString & sql)
{
	qDebug()<<"---RetrieveRecords called.. sql :"<<sql;
	QSqlRecord rec;
	query = new QSqlQuery(db);
	query->prepare(sql);
	query->exec();
	rec = query->record();
	int cnt = rec.count();
	int i = 0;
	//QVector<QStringList> records;
	QString tmpstring;
	QStringList tmpList;
	while(query->next()){
		tmpList.clear();
		for(int j = 0; j < cnt; j++){
			tmpstring = query->value(j).toString();
			tmpList.append(tmpstring);
		}
		//qDebug() << query->value(j).toString();
		records.append(tmpList);
		//qDebug() << records.at(i);
		i++;
	}

	return true;
}
