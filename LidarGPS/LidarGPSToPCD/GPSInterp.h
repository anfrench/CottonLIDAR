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
std::vector<double> headingOffsets;
std::vector<ExtendedGps> gps;
ifstream file;

double offsetDist, offsetAngle;
double offsetPitch, offsetRoll;

int advanceCounter =0;

protected:
    double PI;
    int findIndex(double time);
    void advance();
    void adjustIndex(double time);
    double findHeading();
    void setUTM(ExtendedGps *location,int index, double time);
    void applyOffsets(ExtendedGps *location);

    void findOffsets(std::string fileName);

    void findHeadingOffsets(string fileName);
    void normalizeHeadingOffsets();
    void filterNormalOffsets();
    double findSubsetHeadingOffset(int upper, int lower);
    void setHeadingOffsets(int upper, int lower, double value);
public: 
    GPSInterp(std::string fileName);
    GPSInterp();
    void openFile(std::string fileName);
    ExtendedGps getLocation(double time);
    ExtendedGps currentLocation();
   void setOffsetDist(double offsetDistIN);
   void setOffsetAngle(double offsetAngleIN);
   double getRollOffset();
   double getPitchOffset();
   
    int getSize();
};