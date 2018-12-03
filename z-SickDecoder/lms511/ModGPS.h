#pragma once
#include <string>
#include <iostream>
#include "RMC.h"
#include "GGA.h"
#include "MergeTime.h"

using namespace std;

class ModGPS 
{
	private:
	protected:
	float speed, heading;
	float northing, easting;
	int utc;
	double genericTime;
	public:
	float getSpeed(); 
	float getHeading();
	float getNorthing(); 
	float getEasting();
	int getUTC();
	double getTime();	

	void setSpeed(float sppedIN); 
	void setHeading(float headingIN);
	void setNorthing(float northingIN); 
	void setEasting(float eastingIN);
	void setUTC(int utcIN);
	void setTime(double timeIN);
	
	void readGpsString(string gps);
	
	string toString();
};










