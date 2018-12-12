#include "GPSInterp.h"

GPSInterp::GPSInterp(){PI = atan(1)*4;}
GPSInterp::GPSInterp(std::string fileName)
{
    PI = atan(1)*4;
    openFile(fileName);
}

ModGPS GPSInterp::getLocation(double time)
{
    if(gps.size()<3)
        {return gps[0];}

    int index;
    ModGPS location;
    adjustIndex(time);
    index = findIndex(time);

    if(index <= 0){throw "Time not in bounds!";}

    setUTM(&location, index, time);
    location.setHeading(findHeading());
    location.setTime(time);

    return location;
}

int GPSInterp::findIndex(double time)
{
    int index = 0;
    for(int k=1; k<gps.size(); k++)
    {
        if(time<gps[k].getTime())
        {
            index = k-1;
            break;
        }
    }
    return index;
}

void GPSInterp::openFile(std::string fileName)
{
    std::string line;
    file.open(fileName);
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
            string line;
            getline(file, line);
            ModGPS spot;
            spot.readGpsString(line);
            gps.push_back(spot);
            gps.erase(gps.begin());
            failed = false;
            if(file.eof()){file.close();}
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
     
    if( (index>10 && file.is_open()) && (index >30  || advanceCounter > 25))
    {   
        advance();   
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

    location->setNorthing(startLocation+increase);

    startLocation = gps[index].getEasting();
    endLocation = gps[index+1].getEasting();

    slope = (endLocation-startLocation)/duration;
    increase = slope * (time - startTime);

    location->setEasting(startLocation + increase);
}

int GPSInterp::getSize(){return gps.size();}
