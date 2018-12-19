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


string GPS::getID(){return ID;}
double GPS::getLat(){return latitude;}
double GPS::getLong(){return longitude;}
double GPS::getUTC(){return utc;}
double GPS::getNorthing(){return northing;}
double GPS::getEasting(){return easting;}



//Not defined
void GPS::decode(){}
string GPS::toString(){return "";}


//Defined
void GPS::setString(string GPS_IN)
{
	GPS_String = GPS_IN;
	decode();
	latLongToUTM();
}

double GPS::string2double(string stringIN)
{
	double num;
	stringstream ss(stringIN);
	ss >> num;
	return num;
}

void GPS::latLongToUTM()
{
	short int zone=12;
	double kscale=1;
	long numpts=1;
	
	latlon2utm(&latitude, &longitude,zone,&easting,&northing,&kscale,numpts);	
}






