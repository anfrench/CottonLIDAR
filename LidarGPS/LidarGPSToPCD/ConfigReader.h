#include <string>
#include <fstream>
#include <cmath>
#include <algorithm>

class ConfigReader
{
    private:
    std::ifstream configFile;
    double mountingHeight;
    double xOffset, yOffset, zOffset;
    double roll, pitch, yaw;
    
    double mountingAngle;
    double mountingXYDist;

    protected:
    std::string removeSpace(std::string str);
    double getValue();
    void calcVals();

    public:
    ConfigReader();

    double getMountingHeight();
    double getXOffset();
    double getYOffset();
    double getZOffset();
    double getRoll();
    double getPitch();
    double getYaw();
    
    double getMountingAngle();
    double getMountingXYDist();

    void makeEmptyConfigFile();
};