#include "ModGPS.h"
#include <cmath>
#include <fstream>
#include <vector>

#define debug 0
#define Qsize 50


class GPSInterp
{
private:
std::vector<ModGPS> gps;
ifstream file;

double offsetDist, offsetAngle;

int advanceCounter =0;

protected:
    double PI;
    int findIndex(double time);
    void advance();
    void adjustIndex(double time);
    double findHeading();
    void setUTM(ModGPS *location,int index, double time);
    void applyOffsets(ModGPS *location);
public: 
    GPSInterp(std::string fileName);
    GPSInterp();
    void openFile(std::string fileName);
    ModGPS getLocation(double time);
   
   void setOffsetDist(double offsetDistIN);
   void setOffsetAngle(double offsetAngleIN);
   
    int getSize();
};