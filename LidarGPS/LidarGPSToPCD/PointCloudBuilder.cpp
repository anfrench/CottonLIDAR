#include "PointCloudBuilder.h"
#include <iostream>
#include <limits>
#include <ctime>
#define PROGRESS 1

void PointCloudBuilder::setRoll(double rollIN){roll=rollIN;}
void PointCloudBuilder::setPitch(double pitchIN){pitch=pitchIN;}
void PointCloudBuilder::setYaw(double yawIN){yaw=yawIN;}
void PointCloudBuilder::setMaxPoints(int maxPointsIN){maxPoints=maxPointsIN;}
void PointCloudBuilder::setLeadingPoints(int leadingPointsIN){leadingPoints=leadingPointsIN;}
Point PointCloudBuilder::getMaxValues(){return maxVal;}
Point PointCloudBuilder::getMinValues(){return minVal;}
void PointCloudBuilder::setAngularStep(double stepAngleIN){stepAngle=stepAngleIN;}
void PointCloudBuilder::setScale(double scaleIN){scale=scaleIN;}

void PointCloudBuilder::setDistValues(std::vector<int> vectorIn)
{
    if(!distances.empty())
    {
        distances.clear();
    }

    if(leadingPoints>0)
    {
        leadingPoints-=vectorIn.size(); 
    }
    else
    {
        for(int i=0; i<vectorIn.size(); i++)
        {
            distances.push_back(vectorIn[i]);
        }
    }
}

PointCloudBuilder::~PointCloudBuilder()
{
    if(cloud.is_open()){cloud.close();}
    remove(tempFileName.c_str());
}

PointCloudBuilder::PointCloudBuilder()
{
    noBounds = true;
    shiftValue.x=0;
    shiftValue.y=0; 
    shiftValue.z=0;

    boundMax.x=0;
    boundMax.y=0;
    boundMax.z=0;

    boundMin.x=0;
    boundMin.y=0;
    boundMin.z=0;

    maxVal.x= std::numeric_limits<double>::min();
    maxVal.y=std::numeric_limits<double>::min();
    maxVal.z=std::numeric_limits<double>::min();

    minVal.x=std::numeric_limits<double>::max();
    minVal.y=std::numeric_limits<double>::max();
    minVal.z=std::numeric_limits<double>::max();

    Normals=false;

    openTempFile();
}

void PointCloudBuilder::makePoints()
{

    double angle=roll;
    for(int i=0; i<distances.size(); i++)
    {
        double dist = ((double)distances[i]) /(1000*scale);
        
        if(dist>0.4 && dist<80) // with updates to code I may want to remove this if
        {
            Point p;
            p.y=0;
            p.x=std::cos(toRad(angle, 360))*dist;
            p.z=std::sin(toRad(angle, 360))*dist;

            p.n_x=(-1)*std::cos(toRad(angle, 360)); //normals
            p.n_z=(-1)*std::cos(toRad(angle, 360));

            p.y=std::sin(toRad(pitch, 360))*p.z;//adjusting for pitch
            p.z=std::cos(toRad(pitch, 360))*p.z;

            p.n_y=std::sin(toRad(pitch, 360))*p.n_z;
            p.n_z=std::cos(toRad(pitch, 360))*p.n_z;

            if(p.z>.7)
            {
                workingRow.push_back(p);
            }
        }

        angle+=stepAngle;
    }
}

/*
    Makes the row face the heading + yaw
*/
void PointCloudBuilder::rotateRow()
{ 

    double heading=yaw;
    if(sin(toRad(heading,360))>0){heading=0;}
    else{heading=180;}

    for(int i=0; i<workingRow.size(); i++)
    {
        Point p = workingRow[i];
        double xyDist=findXYDist(p);
        double xyNormDist=findXYNormDist(p);
        
        p.y=xyDist*sin(toRad(heading, 360)); 
        p.x=xyDist*cos(toRad(heading, 360));

        p.n_y=xyNormDist*sin(toRad(heading, 360));
        p.n_x=xyNormDist*cos(toRad(heading, 360));

        workingRow[i] = p;
    }
}

/*
    works on the workingRow.

    Adds gps northing and easting. Decides if point is out of range.
    if not adds shift values, and puts the point in temp file.
*/ void PointCloudBuilder::placeRow(double northing, double easting)
{
    while(!workingRow.empty())
    {
        Point p = workingRow.back();
        workingRow.pop_back();

        p.x+=easting;
        p.y+=northing;

        if(noBounds || inBounds(p))
        {
            p.x+=shiftValue.x;
            p.y+=shiftValue.y;

            cloud << std::fixed << std::showpoint;
            cloud << std::setprecision(10);
            cloud<<p.x<<" "<<p.y<<" "<<p.z;
            if(Normals)
            {
                cloud<<" "<<p.n_x<<" "<<p.n_y<<" "<<p.n_z;
            }
            cloud<<std::endl;
            numberofPoints++;
        }
    }
    cloud.flush();
}

double PointCloudBuilder::toRad(double angle, int steps)
{
	return  PI * angle / (steps / 2 );
}

double PointCloudBuilder::toDegree(double angle, int steps)
{
	return (steps / 2 ) * angle / PI;
}

double PointCloudBuilder::findXYDist(Point p)
{
    double distance=0;

    distance=(p.x*p.x) + (p.y*p.y);
    distance=std::sqrt(distance);

    return distance;
}

double PointCloudBuilder::findXYNormDist(Point p)
{
    double distance=0;

    distance=(p.n_x*p.n_x) + (p.n_y*p.n_y);
    distance=std::sqrt(distance);

    return distance;
}

void PointCloudBuilder::writeFile(std::string fileName)
{
    if(Normals){writeFileWithNormals(fileName);}
    else{writeFileNoNormal(fileName);}
}

void PointCloudBuilder::writeFileWithNormals(std::string fileName)
{
    cloud.close();
    std::ifstream points;
    std::ofstream PCDFile;
    std::string pointLine;

    points.open(tempFileName.c_str());
    PCDFile.open(fileName);

    PCDFile << std::fixed << std::showpoint;
    PCDFile << std::setprecision(6);

    PCDFile<<"# .PCD v.7 - Point Cloud Data file format"<<std::endl;
    PCDFile<<"VERSION .7"<<std::endl;
    PCDFile<<"FIELDS x y z normal_x normal_y normal_z"<<std::endl;
    PCDFile<<"SIZE 4 4 4 4 4 4"<<std::endl;
    PCDFile<<"TYPE F F F F F F"<<std::endl;
    PCDFile<<"COUNT 1 1 1 1 1 1"<<std::endl;
    PCDFile<<"WIDTH "<<numberofPoints<<std::endl;
    PCDFile<<"HEIGHT 1"<<std::endl;
    PCDFile<<"VIEWPOINT 0 0 0 1 0 0 0"<<std::endl;
    PCDFile<<"POINTS "<<numberofPoints<<std::endl;
    PCDFile<<"DATA ascii"<<std::endl;
    
    int flush = 100000;
    for(int i=0; i<numberofPoints; i++)
    {
        getline(points,pointLine);
        PCDFile<<pointLine<<std::endl;

       if(i>flush)
       {
           PCDFile.flush();
           flush+=100000;
        #if PROGRESS
           std::cout<<i/100000<<" of "<<numberofPoints/100000<<std::endl;
        #endif
       }
    }

    points.close();
    PCDFile.flush();
    PCDFile.close();

    remove(tempFileName.c_str());
}

void PointCloudBuilder::writeFileNoNormal(std::string fileName)
{
    cloud.close();
    std::ifstream points;
    std::ofstream PCDFile;
    std::string pointLine;

    points.open(tempFileName.c_str());
    PCDFile.open(fileName);

    PCDFile << std::fixed << std::showpoint;
    PCDFile << std::setprecision(6);

    PCDFile<<"# .PCD v.7 - Point Cloud Data file format"<<std::endl;
    PCDFile<<"VERSION .7"<<std::endl;
    PCDFile<<"FIELDS x y z"<<std::endl;
    PCDFile<<"SIZE 4 4 4"<<std::endl;
    PCDFile<<"TYPE F F F"<<std::endl;
    PCDFile<<"COUNT 1 1 1"<<std::endl;
    PCDFile<<"WIDTH "<<numberofPoints<<std::endl;
    PCDFile<<"HEIGHT 1"<<std::endl;
    PCDFile<<"VIEWPOINT 0 0 0 1 0 0 0"<<std::endl;
    PCDFile<<"POINTS "<<numberofPoints<<std::endl;
    PCDFile<<"DATA ascii"<<std::endl;
    
    int flush = 100000;
    for(int i=0; i<numberofPoints; i++)
    {
        getline(points,pointLine);
        PCDFile<<pointLine<<std::endl;

       if(i>flush)
       {
           PCDFile.flush();
           flush+=100000;
        #if PROGRESS
           std::cout<<i/100000<<" of "<<numberofPoints/100000<<std::endl;
        #endif
       }
    }

    points.close();
    PCDFile.flush();
    PCDFile.close();
}

void PointCloudBuilder::setShift(Point shiftIN)
{
    shiftValue.x=shiftIN.x;
    shiftValue.y=shiftIN.y;
    shiftValue.z=0;
}

void PointCloudBuilder::setBounds(Point lower, Point upper)
{
    boundMax=upper;
    boundMin=lower;

    double sum=0;

    sum+=upper.x+upper.y+upper.z;
    sum+=lower.x+lower.y+lower.z;

    if(sum!=0){noBounds=false;}

}

bool PointCloudBuilder::inBounds(Point p)
{
    if(p.x>maxVal.x){maxVal.x=p.x;}
    if(p.y>maxVal.y){maxVal.y=p.y;}
    if(p.z>maxVal.z){maxVal.z=p.z;}

    if(p.x<minVal.x){minVal.x=p.x;}
    if(p.y<minVal.y){minVal.y=p.y;}
    if(p.z<minVal.z){minVal.z=p.z;}

    return     boundMin.x<p.x && p.x<boundMax.x
            && boundMin.y<p.y && p.y<boundMax.y
            && boundMin.z<p.z && p.z<boundMax.z;
}

Point PointCloudBuilder::readPointString(std::string pointString)
{
    Point p;
    std::stringstream ss;
    ss<<pointString;
    ss>>p.x>>p.y>>p.z;

    return p;
}

void PointCloudBuilder::setMountingHeight(double heightIn)
{
    mountingHeight = heightIn;
}

void PointCloudBuilder::doNormals()
{
    Normals=true;
}

bool PointCloudBuilder::isDone()
{
    if(maxPoints!=0 && numberofPoints>maxPoints)
    {
        return true;
    }
    return false;
}

void PointCloudBuilder::openTempFile()
{
    std::ifstream test;
    std::string fileNameTest=tempFileName;
    srand(time(0));
    int count=rand()%5000;
    bool exsist=true;

    while(exsist)
    {
        count++;
        fileNameTest=std::to_string(count)+tempFileName;
        test.open(fileNameTest.c_str());
        exsist=test.is_open();
        test.close();
    }
    tempFileName=fileNameTest;
    cloud.open(tempFileName);
}

void PointCloudBuilder::setHeight()
{
    for(int i=0; i<workingRow.size(); i++)
    {
        workingRow[i].z=mountingHeight-workingRow[i].z;
    }
}

void PointCloudBuilder::process()
{
    discardCurrent();
    makePoints();
    rotateRow();
    setHeight();
    checkRow();
}

void PointCloudBuilder::findBestRoll()
{
    roll= (double)(int)roll;
    for(double decPlace=1.0; decPlace>=.001; decPlace/=10)
    {
        findBestRollOffset(decPlace);
    }
}

void PointCloudBuilder::findBestRollOffset(double decPlace)
{
    double minimumDisparity=1000, bestFitRoll=roll;
    double minAngle=roll-10*decPlace;
    double maxAngle=roll+10*decPlace;

    for(roll=minAngle; roll<=maxAngle; roll+=decPlace)
    {
        process();
        double disparity= rowMaxVal.z-rowMinVal.z;
        if(disparity<minimumDisparity)
        {
            minimumDisparity=disparity;
            bestFitRoll=roll;
        }
    }
    roll=bestFitRoll;
}

void PointCloudBuilder::discardCurrent()
{
    if(!workingRow.empty()){workingRow.clear();}

    rowMaxVal.x= std::numeric_limits<double>::min();
    rowMaxVal.y=std::numeric_limits<double>::min();
    rowMaxVal.z=std::numeric_limits<double>::min();

    rowMinVal.x=std::numeric_limits<double>::max();
    rowMinVal.y=std::numeric_limits<double>::max();
    rowMinVal.z=std::numeric_limits<double>::max();

}

void PointCloudBuilder::checkRow()
{
    for(int i=0; i<workingRow.size(); i++)
    {
        Point p=workingRow[i];

        if(p.x>rowMaxVal.x){rowMaxVal.x=p.x;}
        if(p.y>rowMaxVal.y){rowMaxVal.y=p.y;}
        if(p.z>rowMaxVal.z){rowMaxVal.z=p.z;}

        if(p.x<rowMinVal.x){rowMinVal.x=p.x;}
        if(p.y<rowMinVal.y){rowMinVal.y=p.y;}
        if(p.z<rowMinVal.z){rowMinVal.z=p.z;}
    }
}