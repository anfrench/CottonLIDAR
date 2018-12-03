#include "ModGPS.h"


float ModGPS::getSpeed(){return speed;} 
float ModGPS::getHeading(){return heading;} 
float ModGPS::getNorthing(){return northing;}  
float ModGPS::getEasting(){return easting;} 
int ModGPS::getUTC(){return utc;} 
	
void ModGPS::setSpeed(float speedIN){speed = speedIN;}
void ModGPS::setHeading(float headingIN){heading = headingIN;}
void ModGPS::setNorthing(float northingIN){northing = northingIN;}
void ModGPS::setEasting(float eastingIN){easting = eastingIN;}
void ModGPS::setUTC(int utcIN){utc=utcIN;}








