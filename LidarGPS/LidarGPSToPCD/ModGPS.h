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
	double speed, heading, altitude;
	double northing, easting;
	int utc;
	double genericTime;
	
	public:
	double getSpeed(); 
	double getHeading();
	double getNorthing(); 
	double getEasting();
	int getUTC();
	double getTime();
	double getAltitude();	

	void setSpeed(double sppedIN); 
	void setHeading(double headingIN);
	void setNorthing(double northingIN); 
	void setEasting(double eastingIN);
	void setUTC(int utcIN);
	void setTime(double timeIN);
	void setAltitude(double altitudeIN);
	
	void readGpsString(string gps);
	
	string toString();
};










