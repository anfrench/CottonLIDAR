#pragma once
#include "Gps.h"
#include <iostream>



class GGA : public GPS
{
	private:
	int quality, SVNum;
	double hdop, height, geoidSeparation;
	string heightUnit, geoidUnit;
	double reccordAge;
	
	
	protected:
	public:
	GGA();
	void decode();
	int getQuality();
	int getSVNum();
	double getHdop(); 
	double getHeight(); 
	double getGeoidSeparation();
	double getReccordAge();
	
	string toString();
};




















