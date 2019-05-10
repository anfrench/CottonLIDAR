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

double GPSInterp::getRollOffset(){return offsetRoll;}
double GPSInterp::getPitchOffset(){return offsetPitch;}

ExtendedGps GPSInterp::getLocation(double time)
{
    if(gps.size()<3)
        {return gps[0];}

    int index;
    ExtendedGps location;
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
    location.setHeading(findHeading());//this isn't used right now.
    location.setYaw(gps[index].getYaw());
    location.setPitch(gps[index].getPitch());
    location.setRoll(gps[index].getRoll());
    
    #if debug
    std::cout<<"getLocation::setting utm "<<endl;
    #endif
    location.setTime(time);
    setUTM(&location, index, time);
    

    //location.setAltitude(gps[index].getAltitude()); for mod gps...

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
    //findHeadingOffsets(fileName);  put on hold for a while...
    findOffsets(fileName);
    std::string line;
    file.open(fileName);
    if(!file.is_open()){throw "GPS File Could Not Be Opened!";}

    if(!gps.empty()){gps.clear();}

    while(getline(file, line) && gps.size()<Qsize)
    {
        try
        {
            ExtendedGps spot;
            spot.decode(line);
            gps.push_back(spot);
        }
        catch(const char *e)
        {
            #if debug
            cout<<e<<"here"<<std::endl;
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
            ExtendedGps spot;
            spot.decode(line);
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

void GPSInterp::setUTM(ExtendedGps *location,int index, double time)
{
    double startLocation, endLocation;
    double startTime, endTime;
    double duration, slope, increase;
    
    startTime = gps[index].getTime();
    endTime = gps[index+1].getTime();
    duration=endTime-startTime;

    startLocation = gps[index].getNorthing();
    endLocation = gps[index+1].getNorthing();

    slope = (endLocation-startLocation)/duration;
    increase = slope * (time - startTime);
    increase=increase;

    location->setNorthing(startLocation+increase);

    startLocation = gps[index].getEasting();
    endLocation = gps[index+1].getEasting();

    slope = (endLocation-startLocation)/duration;
    increase = slope * (time - startTime);
    increase=increase;

    location->setEasting(startLocation +increase);
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


void GPSInterp::applyOffsets(ExtendedGps *location)
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

ExtendedGps GPSInterp::currentLocation()
{
    return gps[(int)(gps.size()/2)];
}


void GPSInterp::findHeadingOffsets(string fileName)
{
    std::string line;
    file.open(fileName);
    if(!file.is_open()){throw "GPS File Could Not Be Opened!";}

    if(!gps.empty()){gps.clear();}

    while(getline(file, line))
    {
         ExtendedGps spot;
            spot.decode(line);
            double heading =spot.getHeading();
            if(heading<30 && heading >-30)
            {
                headingOffsets.push_back(heading);
            }else if (heading<210 && heading >150)
            {
                headingOffsets.push_back(heading-180);
            }else{headingOffsets.push_back(0);}
    }
    file.close();
    GPSInterp::normalizeHeadingOffsets();
}

 void GPSInterp::normalizeHeadingOffsets()
 {
     bool isNorth=false;
     int northCounter;
     northCounter=0;
     for(int i=30; i<headingOffsets.size(); i++)
    {
        if(headingOffsets[i]!=0){northCounter++;}
        else{northCounter--;}

        if(northCounter>15){isNorth=true;}
        if(northCounter<-15){isNorth=false;}

        if(northCounter<-15){northCounter=-15;}
        if(northCounter>15){northCounter=15;}

        if(!isNorth){headingOffsets[i-30]=0;}
    }
    if(!isNorth)
    {
        for(int i=headingOffsets.size()-30; i<headingOffsets.size(); i++)
        {
            headingOffsets[i]=0;
        }
    }

    filterNormalOffsets();
    for(int i=0; i<headingOffsets.size(); i++)
    {
        cout<<headingOffsets[i]<<endl;
    }
    cout<<"done";
 }

  void GPSInterp::filterNormalOffsets()
  {
    vector<pair<int,int>> index;
    vector<double> averages;
    bool inNorthSection=false;

    bool endNotSet=false;

    for(int i=10; i<headingOffsets.size(); i++)
    {
        if(headingOffsets[i-5]==0 && headingOffsets[i-4]==0 &&
        headingOffsets[i-3]==0 && headingOffsets[i-2]==0 &&
        headingOffsets[i-1]==0 && endNotSet)
        {
            index.back().second=i-5;
            endNotSet=false;
        }
        if(headingOffsets[i-5]==0 && headingOffsets[i-4]==0
            && headingOffsets[i-3]==0 && headingOffsets[i-2]==0
            && headingOffsets[i-1]==0 && headingOffsets[i]!=0)
        {
            index.push_back(make_pair(i,0));
            endNotSet=true;
        }
    }
    if(endNotSet){index.back().second=headingOffsets.size()-1;}

    for(int i=0; i<index.size(); i++)
    {
        double average=findSubsetHeadingOffset(index[i].second,index[i].first);
        setHeadingOffsets(index[i].second,index[i].first,average);
        averages.push_back(average);
    }

    for(int i=0; i<index.size()-1;i++)
    {
        for(int k=index[i].second; k<=index[i+1].first; k++)
        {
            /*
                This area is for adding offsets to the points where it's turning.

            */ 
        }
    }

  }
    

double GPSInterp::findSubsetHeadingOffset(int upper, int lower)
{
    double average=0;
    int count=0;
    for(int k=lower; k<=upper; k++)
    {
        if(headingOffsets[k]!=0)
        {
            average+=headingOffsets[k];
            count++;
        }
    }
    return average/count;
}

void GPSInterp::setHeadingOffsets(int upper, int lower, double value)
{
    for(int i=lower; i<=upper; i++)
    {
        headingOffsets[i]=value;
    } 
}



void GPSInterp::findOffsets(string fileName)
{
    std::string line;
    file.open(fileName);
    if(!file.is_open()){throw "GPS File Could Not Be Opened!";}

    offsetRoll=0;
    offsetPitch=0;
    int count=0;

    while(getline(file, line))
    {
        ExtendedGps spot;
        spot.decode(line);
        
        double pitch=spot.getPitch();
        double roll=spot.getRoll();
        if(pitch>200){pitch-=360;}
        if(roll>200){roll-=360;}

        offsetRoll+=pitch;
        offsetPitch+=roll;

        count++;
    }
    offsetRoll/=count;
    offsetPitch/=count;

    offsetRoll*=-1;
    offsetPitch*=-1;

    file.close();
}