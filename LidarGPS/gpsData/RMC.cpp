#include "RMC.h"
RMC::RMC()
{
	active = false;
	speed = 0;
	heading = 0; 
	date = 0;
	magneticVariation = 0;
}

void RMC::decode()
{
	string tokens[12];
	
	for(int i=0; i<11;i++)
	{
		tokens[i]= GPS_String.substr(0,GPS_String.find(","));
		GPS_String=GPS_String.substr(GPS_String.find(",")+1);
	}
	tokens[12]=GPS_String;
	
	ID				=tokens[0];
	utc 			= (int)string2Float(tokens[1]);
	active 			=(((int) string2Float(tokens[2]))) ==1 ? true : false;
	
	latitude 		= string2Float(tokens[3]);
	latDirection 	= tokens[4];
	longitude 		= string2Float(tokens[5]);
	longDirection 	= tokens[6];
	
	speed			= string2Float(tokens[7]);
	heading			=  string2Float(tokens[8]);
	date 			= string2Float(tokens[9]);
	magneticVariation 		= string2Float(tokens[10]);
	checkSum 		= tokens[11];
	
	
}	


string RMC::toString()
{
	string string ="";
	string +="ID: "+ID+"\n";
	string +="utc: "+to_string(utc)+"\n";
	string +="Latitude: "+to_string(latitude)+latDirection+"\n";
	string +="Longitude: "+to_string(longitude)+longDirection+"\n";
	string +="Knots: " + to_string(speed)+"\n";
	string +="Date: " + to_string(date)+"\n";
	string+="Magnetic Variation: "+to_string(magneticVariation)+"\n";
	
	return string;
}

bool RMC::isActive(){return active;}
float RMC::getSpeed(){return speed;}
float RMC::getHeading(){return heading;}
float RMC::getDate(){return date;}
float RMC::getMagneticVariation(){return magneticVariation;}
