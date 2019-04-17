//#include "ModGPS.h" old class
#include "ExtendedGps.h"
#include <cmath>
#include <fstream>
#include <vector>

#define debug 0
#define Qsize 50
#if debug 
#include <iostream>
#endif

class GPSInterp
{
private:
std::vector<ExtendedGps> gps;
ifstream file;

double offsetDist, offsetAngle;

int advanceCounter =0;

protected:
    double PI;
    int findIndex(double time);
    void advance();
    void adjustIndex(double time);
    double findHeading();
    void setUTM(ExtendedGps *location,int index, double time);
    void applyOffsets(ExtendedGps *location);
public: 
    GPSInterp(std::string fileName);
    GPSInterp();
    void openFile(std::string fileName);
    ExtendedGps getLocation(double time);
    ExtendedGps currentLocation();
   void setOffsetDist(double offsetDistIN);
   void setOffsetAngle(double offsetAngleIN);
   
    int getSize();
};