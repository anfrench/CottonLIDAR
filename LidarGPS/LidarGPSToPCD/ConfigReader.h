#pragma once

#include <string>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "Point.h"

class ConfigReader
{
    private:
    std::ifstream configFile;

    std::string lidarFileName, lidarName, GPSFileName, outputFileName;

    Point shift, boundMax, boundMin;

    double mountingHeight;
    double xOffset, yOffset, zOffset;
    double roll, pitch, yaw;
    
    double mountingAngle;
    double mountingXYDist;

    int lidarType, excerptNum=0, leadingPoints=0;

    protected:
    std::string removeAndTrim(std::string str);
    double getValue();
    void calcVals();

    void setLidarType();

    public:
    ConfigReader();

    void read(std::string configFileName);

    std::string getLidarFileName();
    std::string getGPSFileName();
    std::string getOutputFileName();

    double getMountingHeight();
    double getXOffset();
    double getYOffset();
    double getZOffset();
    double getRoll();
    double getPitch();
    double getYaw();

    Point getUpperBounds();
    Point getLowerBounds();
    Point getShift();
    
    double getMountingAngle();
    double getMountingXYDist();

    int getLidarType();
    int getExcerptNum();
    int getLeadingPoints();

    void setRoll(double rollIN);
    void setPitch(double pitchIN);
    void setYaw(double yawIN);

    std::string makeEmptyConfigFile();
};