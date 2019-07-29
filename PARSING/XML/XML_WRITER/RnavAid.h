#ifndef RNAVAID_H_
#define RNAVAID_H_

#include <QString>

/**
 * VHF Navaid 정보 생성 (SIM) 20130927 추가
 */
class RnavAid {
public:
	RnavAid();
	~RnavAid();


	void setName(QString var) { name = var; }
	void setStationName(QString var) { station_name = var; }
	void setType(QString var) { type = var; }
	void setFrequency(QString var) { frequency = var; }
	void setLat(QString var) { lat = var; }
	void setLon(QString var) { lon = var; }
	void setClsTp1(QString var) { cls_tp1 = var; }
	void setClsTp2(QString var) { cls_tp2 = var; }
	//20131203 NEW
	void setDmeLat(QString var) { dme_lat = var; }
	void setDmeLon(QString var) { dme_lon = var; }


	QString getName() { return name ; }
	QString getStationName() { return station_name ; }
	QString getType() { return type ; }
	QString getFrequency() { return frequency ; }
	QString getLat() { return lat ; }
	QString getLon() { return lon ; }
	QString getClsTp1() { return cls_tp1; }
	QString getClsTp2() { return cls_tp2; }

	//20131203 NEW
	QString getDmeLat() { return dme_lat ; }
	QString getDmeLon() { return dme_lon ; }

	void printRnav();

private:
	QString name;
	QString station_name;
	QString type;
	QString frequency;
	QString lat;
	QString lon;
	QString cls_tp1;
	QString cls_tp2;
	//20131203 NEW
	QString dme_lat;
	QString dme_lon;
};

#endif /* AERODROME_H_ */

