#include "ConfigReader.h"

double ConfigReader::getMountingHeight(){return mountingHeight;}
double ConfigReader::getXOffset(){return xOffset;}
double ConfigReader::getYOffset(){return yOffset;}
double ConfigReader::getZOffset(){return zOffset;}
double ConfigReader::getRoll(){return roll;}
double ConfigReader::getPitch(){return pitch;}
double ConfigReader::getYaw(){return yaw;}

double ConfigReader::getMountingAngle(){return mountingAngle;}
double ConfigReader::getMountingXYDist(){return mountingXYDist;}

ConfigReader::ConfigReader(){}

void ConfigReader::read(std::string configFileName)
{
    configFile.open(configFileName);
    if(!configFile.is_open())
        {throw"Could not find configuation File: ";}
    
    std::string skip;
    getline(configFile, skip);

    mountingHeight= getValue();
    xOffset = getValue();
    yOffset = getValue();
    zOffset = getValue();
    roll = getValue();
    pitch = getValue();
    yaw = getValue();

    configFile.close();

    calcVals();
}

std::string ConfigReader::removeSpace(std::string str)
{
    std::remove_if(str.begin(), str.end(), isspace);
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    return str;
}

double ConfigReader::getValue()
{
    std::string line;
    
    getline(configFile, line);
    
    line = removeSpace(line);
    line = line.substr(line.find(":")+1);

    return stod(line);
}

void ConfigReader::calcVals()
{
    mountingXYDist = pow(xOffset,2);
    mountingXYDist += pow(xOffset,2);
    mountingXYDist = sqrt(mountingXYDist);

    double pi = atan(1)*4;

    mountingAngle = atan2(xOffset, yOffset);
    mountingAngle = 180*mountingAngle / pi;
    mountingAngle = fmod(mountingAngle, 360);

    if (mountingAngle < 0){mountingAngle += 360;}
}

std::string ConfigReader::makeEmptyConfigFile()
{

    std::string sample;
    sample = "Paste The Fallowing into a file. \n\n\n";
    sample+="Config File For Run #( ##) Field(##) (MM/DD/YYY) :~~\n";
    sample+="Mounting Height: x.xx\n";
    sample+="X offset: x.xx\n";
    sample+="Y offset: x.xx\n";
    sample+="Z offset: x.xx\n";
    sample+="Roll: x.xx\n";
    sample+="pitch: x.xx\n";
    sample+="yaw: x.xx\n";
    sample+="Lidar Unit:(sumeUnit1,SomeUnit2,SomeUnit3)\n\n\n";

    return sample;

}