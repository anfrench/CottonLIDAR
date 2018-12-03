#include "GGA.h"

GGA::GGA()
{
	quality =0; 
	SVNum =0;
	hdop = 0; 
	height = 0; 
	geoidSeparation = 0;
	heightUnit ="x"; 
	geoidUnit = "x";
	reccordAge = 0;
}

void GGA::decode()
{
	string tokens[15];
	
	for(int i=0; i<14;i++)
	{
		tokens[i]= GPS_String.substr(0,GPS_String.find(","));
		GPS_String=GPS_String.substr(GPS_String.find(",")+1);
	}
	tokens[14]=GPS_String;
	
	ID				=tokens[0];
	utc 			= (int)string2Float(tokens[1]);
	latitude 		= string2Float(tokens[2]);
	latDirection 	= tokens[3];
	longitude 		= string2Float(tokens[4]);
	longDirection 	= tokens[5];
	quality 		= (int) string2Float(tokens[6]);
	SVNum 			= (int) string2Float(tokens[7]);
	hdop 			= string2Float(tokens[8]);
	height 			= string2Float(tokens[9]);
	heightUnit 		= tokens[10];
	geoidSeparation = string2Float(tokens[11]);
	geoidUnit 		= tokens[12];
	reccordAge 		= string2Float(tokens[13]);
	checkSum 		= tokens[14];
	
}


string GGA::toString()
{
	string string ="";
	string +="ID: "+ID+"\n";
	string +="utc: "+to_string(utc)+"\n";
	string +="Latitude: "+to_string(latitude)+latDirection+"\n";
	string +="Longitude: "+to_string(longitude)+longDirection+"\n";
	string +="GPS Quality: "+to_string(quality)+"\n";
	string +="SVs in use: "+to_string(SVNum)+"\n";
	string +="HDOP: "+to_string(hdop)+"\n";
	string +="Orthometric height: "+to_string(height)+heightUnit+"\n";
	string +="Geoid Separation: "+to_string(geoidSeparation)+geoidUnit+"\n";
	return string;
}



int GGA::getQuality(){return quality;}
int GGA::getSVNum(){return SVNum;}
float GGA::getHdop(){return hdop;} 
float GGA::getHeight(){return height;} 
float GGA::getGeoidSeparation(){return geoidSeparation;}
float GGA::getReccordAge(){return reccordAge;}



















