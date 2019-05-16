#pragma once
#include <string>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <sstream>
#include "MergeTime.h"

class ExtendedGps
{
    private:
    double northing=0, easting=0, heading=0;
    double roll=0, pitch=0, yaw=0;
    double time=0;

    protected:
    double fixDegrees(double degreeIN);
    public:

    void setNorthing(double northingIN);
    void setEasting(double eastingIN);
    void setHeading(double headingIN);
    void setRoll(double rollIN);
    void setPitch(double pitchIN);
    void setYaw(double yawIN);
    void setTime(double timeIN);

    ExtendedGps();
    void decode(std::string exGPSstring);

    double getNorthing();
    double getEasting();
    double getHeading();
    double getRoll();
    double getPitch();
    double getYaw();
    double getTime();

};