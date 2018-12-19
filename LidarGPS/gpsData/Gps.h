#pragma once
#include <string>
#include <sstream>
#include <stdio.h>

using namespace std;

class GPS
{
	private:
	protected:
	string GPS_String, ID;
	double latitude, longitude; 
	int utc;
	string latDirection, longDirection, checkSum;
	float northing, easting;
	
	//not defined
	virtual void decode();

	//Defined
	float string2Float(string stringIN);
	public:
	GPS();
	void setString(string GPS_IN);
	
	string getID();
	double getLat();
	double getLong();
	double getUTC();
	float getNorthing();
	float getEasting();
	virtual string toString();
	
};




