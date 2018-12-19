#include "Gps.h"
//Constructor
GPS::GPS()
{
	GPS_String = "x";
	ID="x";
	latitude = 0;
	longitude = 0;
	utc=0;
	latDirection = "x";
	longDirection = "x";
	checkSum = "x";
	northing = 0; 
	easting = 0;
}

//Not defined
void GPS::decode(){}
string GPS::toString(){return "";}


//Defined
void GPS::setString(string GPS_IN)
{
	GPS_String = GPS_IN;
	
	decode();
}

float GPS::string2Float(string stringIN)
{
	float num;
	stringstream ss(stringIN);
	ss >> num;
	return num;
}

string GPS::getID(){return ID;}
double GPS::getLat(){return latitude;}
double GPS::getLong(){return longitude;}
double GPS::getUTC(){return utc;}
float GPS::getNorthing(){return northing;}
float GPS::getEasting(){return easting;}








