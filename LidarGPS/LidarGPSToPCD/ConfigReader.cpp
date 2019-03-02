#include "ConfigReader.h"

std::string ConfigReader::getLidarFileName(){return removeAndTrim(lidarFileName);}
std::string ConfigReader::getGPSFileName(){return removeAndTrim(GPSFileName);}
std::string ConfigReader::getOutputFileName(){return removeAndTrim(outputFileName);}


double ConfigReader::getMountingHeight(){return mountingHeight;}
double ConfigReader::getXOffset(){return xOffset;}
double ConfigReader::getYOffset(){return yOffset;}
double ConfigReader::getZOffset(){return zOffset;}
double ConfigReader::getRoll(){return roll;}
double ConfigReader::getPitch(){return pitch;}
double ConfigReader::getYaw(){return yaw;}

Point ConfigReader::getUpperBounds(){return boundMax;}
Point ConfigReader::getLowerBounds(){return boundMin;}
Point ConfigReader::getShift(){return shift;}

double ConfigReader::getMountingAngle(){return mountingAngle;}
double ConfigReader::getMountingXYDist(){return mountingXYDist;}

int ConfigReader::getLidarType(){return lidarType;}

ConfigReader::ConfigReader(){}

void ConfigReader::read(std::string configFileName)
{
    configFile.open(configFileName);
    if(!configFile.is_open())
        {throw"Could not find configuation File: ";}
    
    std::string skip;
    getline(configFile, skip);
    getline(configFile, skip);

    getline(configFile, lidarFileName);
    getline(configFile, GPSFileName);
    getline(configFile, outputFileName);

    getline(configFile, skip);

    mountingHeight= getValue();
    xOffset=getValue();
    yOffset=getValue();
    zOffset=getValue();
    roll=getValue();
    pitch=getValue();
    yaw=getValue();

    getline(configFile, skip);

    boundMax.x=getValue();
    boundMax.y=getValue();
    boundMax.z=getValue();

    getline(configFile, skip);

    boundMin.x=getValue();
    boundMin.y=getValue();
    boundMin.z=getValue();

    getline(configFile, skip);

    shift.x=getValue();
    shift.y=getValue();

    getline(configFile, skip);
    
    getline(configFile, lidarName);
    setLidarType();

    configFile.close();

    calcVals();
}

std::string ConfigReader::removeAndTrim(std::string str)
{
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    str = str.substr(str.find(":")+1);
    return str;
}

double ConfigReader::getValue()
{
    std::string line;
    
    getline(configFile, line);
    
    line = removeAndTrim(line);

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

void ConfigReader::setLidarType()
{
    lidarType=0;
    if(lidarName.find("LMS511") != std::string::npos)
    {
        lidarType=0;
    }
    else if(lidarName.find("LMS4000Distance") != std::string::npos)
    {
        lidarType=1;
    }
    else if(lidarName.find("LMS4000DistanceAndRemessions") != std::string::npos)
    {
        lidarType=2;
    }
}

std::string ConfigReader::makeEmptyConfigFile()
{

    std::string sample;
    sample = "Paste The Fallowing into a file. \n\n\n";
    sample+="Config File For Run #( ##) Field(##) (MM/DD/YYY) :~~\n\n";
    sample+="Lidar Data File: xxxxxx.txt\n";
    sample+="GPS Data File: xxxxxx.txt\n";
    sample+="Output(PCD) FileName: xxxxxx.pcd\n\n";
    sample+="Mounting Height: 0.0\n";
    sample+="X offset: 0.0\n";
    sample+="Y offset: 0.0\n";
    sample+="Z offset: 0.0\n";
    sample+="Roll: 0.0\n";
    sample+="pitch: 0.0\n";
    sample+="yaw: 0.0\n\n";
    sample+="Upper Bounds X: 0.0\n";
    sample+="Upper Bounds Y: 0.0\n";
    sample+="Upper Bounds Z: 0.0\n\n";
    sample+="Lower Bounds X: 0.0\n";
    sample+="Lower Bounds Y: 0.0\n";
    sample+="Lower Bounds Z: 0.0\n\n";
    sample+="Shift X: 0.0\n";
    sample+="Shift Y: 0.0\n\n";
    sample+="Lidar Unit:(LMS511,LMS4000Distance,LMS4000DistanceAndRemessions)\n\n\n";

    return sample;

}