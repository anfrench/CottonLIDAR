#pragma once
#include "Gps.h"

class RMC : public GPS
{
	private:
	bool active;
	float speed, heading, date;
	float magneticVariation;
	protected:
	public:
	RMC();
	
	void decode();

	bool isActive();
	float getSpeed();
	float getHeading();
	float getDate();
	float getMagneticVariation();
	string toString();
};

