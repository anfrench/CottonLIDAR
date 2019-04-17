    #include "ExtendedGps.h"

    ExtendedGps::ExtendedGps(){}

    double ExtendedGps::getNorthing(){return northing;}
    double ExtendedGps::getEasting(){return easting;}
    double ExtendedGps::getHeading(){return heading;}
    double ExtendedGps::getRoll(){return roll;}
    double ExtendedGps::getPitch(){return pitch;}
    double ExtendedGps::getYaw(){return yaw;}
    double ExtendedGps::getTime(){return time;}

    void ExtendedGps::setNorthing(double northingIN)
    {
        northing=northingIN;
    }
    void ExtendedGps::setEasting(double eastingIN)
    {
        easting=eastingIN;
    }
    void ExtendedGps::setHeading(double headingIN)
    {
        heading=headingIN;
    }
    void ExtendedGps::setRoll(double rollIN)
    {
        roll=fixDegrees(rollIN);
    }
    void ExtendedGps::setPitch(double pitchIN)
    {
        pitch=fixDegrees(pitchIN);
    }
    void ExtendedGps::setYaw(double yawIN)
    {
        yaw=fixDegrees(yawIN);
    }
    void ExtendedGps::setTime(double timeIN)
    {
        time=timeIN;
    }

void ExtendedGps::decode(std::string exGPSstring)
{
    try
    {
    
        std::vector<std::string> tokens;
        std::stringstream data(exGPSstring);
        std::string token;
        MergeTime mtime;
        mtime.setStamp(exGPSstring);
        while(std::getline(data,token, ','))
        {
            tokens.push_back(token);
        }
        setNorthing(stod(tokens[23]));
        setEasting(stod(tokens[22]));
        setHeading(stod(tokens[24]));
        setRoll(stod(tokens[12]));
        setPitch(stod(tokens[11]));
        setYaw(stod(tokens[10]));
        setTime(mtime.getTime());
    }
    catch(...)
    {
        setNorthing(0);
        setEasting(0);
        setHeading(0);
        setRoll(0);
        setPitch(0);
        setYaw(0);
        setTime(0);
    }
}

double ExtendedGps::fixDegrees(double degreeIN)
{
    while(degreeIN>360.0){degreeIN-=360.0;}
    while(degreeIN<0.0){degreeIN+=360.0;}
    return degreeIN;
}