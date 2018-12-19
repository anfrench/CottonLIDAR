#pragma once
#include "Gps.h"

class RMC : public GPS
{
	private:
	bool active;
	double speed, heading, date;
	double magneticVariation;
	protected:
	public:
	RMC();
	
	void decode();

	bool isActive();
	double getSpeed();
	double getHeading();
	double getDate();
	double getMagneticVariation();
	string toString();
};

