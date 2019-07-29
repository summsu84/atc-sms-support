#ifndef HOLDING_H_
#define HOLDING_H_

#include <QString>

/**
 * Enroute 및 STAR, APCH 홀딩 패턴 정보 생성  (SIM) 20130927 추가
 * LEG_TM, RHO 추가 20131210
 */
class Holding {
public:
	Holding();
	~Holding();


	void setHoldingFixName(QString var) { holding_fix_name = var; }
	void setDupId(QString var) { dup_id = var; }
	void setInboundTrack(QString var) { inbound_track = var; }
	void setDirection(QString var) { direction = var; }
	void setMaxIas(QString var) { max_ias = var; }
	void setMinAlt(QString var) { min_alt = var; }
	void setMaxAlt(QString var) { max_alt = var; }
	void setCpType(QString var) { cp_type = var; }
	void setCpValue(QString var) { cp_value = var; }
	void setType(QString var) { type = var; }
	void setReferName(QString var) { refer_name = var; }
	void setRho(QString var) { rho = var; }
	void setLegTm(QString var) { leg_tm = var; }


	QString getHoldingFixName() { return holding_fix_name ; }
	QString getDupId() { return dup_id ; }
	QString getInboundTrack() { return inbound_track ; }
	QString getDirection() { return direction ; }
	QString getMaxIas() { return max_ias ; }
	QString getMinAlt() { return min_alt ; }
	QString getMaxAlt() { return max_alt ; }
	QString getCpType() { return cp_type ; }
	QString getCpValue() { return cp_value ; }
	QString getType() { return type ; }
	QString getReferName() { return refer_name ; }
	QString getRho() { return rho; }
	QString getLegTm() { return leg_tm; }

	void printHolding();
private:
	QString holding_fix_name;
	QString dup_id;
	QString inbound_track;
	QString direction;
	QString max_ias;
	QString min_alt;
	QString max_alt;
	QString cp_type;
	QString cp_value;
	QString type;
	QString refer_name;
	QString rho;
	QString leg_tm;

};

#endif /* AERODROME_H_ */

