#include "GPSInterp.h"

GPSInterp::GPSInterp()
{
    PI = atan(1)*4;
    offsetAngle=0;
    offsetDist =0;
}
GPSInterp::GPSInterp(std::string fileName)
{
    GPSInterp();
    openFile(fileName);
}

ModGPS GPSInterp::getLocation(double time)
{
    if(gps.size()<3)
        {return gps[0];}

    int index;
    ModGPS location;
    #if debug
    std::cout<<"getLocation::adjusting time. "<<endl;
    #endif
    adjustIndex(time);

    #if debug
    std::cout<<"getLocation::finding index "<<endl;
    #endif
    index = findIndex(time);

    if(index <= 0 || index >= gps.size()-4){throw "Time not in bounds!";}

    #if debug
    std::cout<<"getLocation::setting heading "<<endl;
    #endif
    location.setHeading(findHeading());

    #if debug
    std::cout<<"getLocation::setting utm "<<endl;
    #endif
    setUTM(&location, index, time);
    location.setTime(time);

    #if debug
    std::cout<<"getLocation::applying offsets "<<endl;
    #endif
    applyOffsets(&location);
    
    return location;
}

int GPSInterp::findIndex(double time)
{
    int index = 0;
    for(int k=1; k<gps.size(); k++)
    {
        if(time<gps[k].getTime())
        {
            return k-1;
        }
    }
    return gps.size()-1;
}

void GPSInterp::openFile(std::string fileName)
{
    std::string line;
    file.open(fileName);
    if(!file.is_open()){throw "GPS File Could Not Be Opened!";}

    if(!gps.empty()){gps.clear();}

    while(getline(file, line) && gps.size()<Qsize)
    {
        try
        {
            ModGPS spot;
            spot.readGpsString(line);
            gps.push_back(spot);
        }
        catch(const char *e)
        {
            #if debug
            cout<<e<<std::endl;
            #endif
        }
    }

}

double GPSInterp::findHeading()
{
    double x=0, y=0;

    for(int i=0; i<gps.size(); i++)
    {
        double heading = gps[i].getHeading();
        heading = PI*heading/180 ;
        x+= cos(heading);
        y+= sin(heading);
    }
    //x/= gps.size();
    //y/= gps.size();

    double heading = (180*std::atan(y/x)/PI);
    heading = ((double)(((int)heading)%360))+ heading - floor(heading);

    return heading;
}

void GPSInterp::advance()
{
    advanceCounter =0;
    bool failed = true;
    while(file.is_open() && failed)
    {
        try
        {
            if(file.eof()){file.close();}
            string line;
            getline(file, line);
            ModGPS spot;
            spot.readGpsString(line);
            gps.push_back(spot);
            gps.erase(gps.begin());
            failed = false;
        }
        catch(const char *e)
        {
             #if debug
            cout<<e<<std::endl;
            #endif
        }
    }
    if(!file.is_open() && failed)
    {
         gps.erase(gps.begin());
    }
}

void GPSInterp::adjustIndex(double time)
{
    advanceCounter++;
    int index = findIndex(time);
     
    while( (index>10 && file.is_open()) && (index >25  || advanceCounter > 25))
    {   
        advance();   
        index = findIndex(time);
    }
    
}

void GPSInterp::setUTM(ModGPS *location,int index, double time)
{
    double startLocation, endLocation;
    double startTime, endTime;
    double duration, slope, increase;
    
    startTime = gps[index].getTime();
    endTime = gps[index+1].getTime();
    startLocation = gps[index].getNorthing();
    endLocation = gps[index+1].getNorthing();

    duration = endTime - startTime;
    slope = (endLocation-startLocation)/duration;
    increase = slope * (time - startTime);
    increase=std::abs(increase);
    if(location->getHeading()>90&&location->getHeading()<180){increase*=-1;}

    location->setNorthing(startLocation+increase);

    startLocation = gps[index].getEasting();
    endLocation = gps[index+1].getEasting();

    slope = (endLocation-startLocation)/duration;
    increase = slope * (time - startTime);
    increase=std::abs(increase);
    if(location->getHeading()>90&&location->getHeading()<180){increase*=-1;}

    location->setEasting(startLocation + increase);
}

void GPSInterp::setOffsetDist(double offsetDistIN)
{
    offsetDist = offsetDistIN;
}

void GPSInterp::setOffsetAngle(double offsetAngleIN)
{
    offsetAngle = PI*offsetAngleIN/180;
}

int GPSInterp::getSize(){return gps.size();}


void GPSInterp::applyOffsets(ModGPS *location)
{
    double angle, northing, easting;
    
    angle = PI*location->getHeading()/180;
    northing = location->getNorthing();
    easting = location->getEasting();

    angle += offsetAngle;
    northing += offsetDist*cos(angle);
    easting += offsetDist*sin(angle);

    location->setNorthing(northing);
    location->setEasting(easting);
}

ModGPS GPSInterp::currentLocation()
{
    return gps[(int)(gps.size()/2)];
}