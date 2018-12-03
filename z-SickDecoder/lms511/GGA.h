#pragma once
#include "Gps.h"
#include <iostream>



class GGA : public GPS
{
	private:
	int quality, SVNum;
	float hdop, height, geoidSeparation;
	string heightUnit, geoidUnit;
	float reccordAge;
	
	
	protected:
	public:
	GGA();
	void decode();
	int getQuality();
	int getSVNum();
	float getHdop(); 
	float getHeight(); 
	float getGeoidSeparation();
	float getReccordAge();
	
	string toString();
};




















