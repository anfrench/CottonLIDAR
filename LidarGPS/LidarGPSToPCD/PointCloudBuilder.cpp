#include "PointCloudBuilder.h"
#include <iostream>

void PointCloudBuilder::setRoll(double rollIN){roll=rollIN;}
void PointCloudBuilder::setPitch(double pitchIN){pitch=pitchIN;}
void PointCloudBuilder::setYaw(double yawIN){yaw=yawIN;}

PointCloudBuilder::PointCloudBuilder()
{
    adjust = false;
    minPoint.x = 0;
    minPoint.y=0;
    minPoint.z = 0;
    cloud.open("tempFile.txt");
}

void PointCloudBuilder::addPoints(std::vector<int> distance, double angle, double stepAngle, double scale)
{
    angle = toRad(angle, 360);
    stepAngle = toRad(stepAngle, 360);
    angle += roll;
    for(int i=0; i<distance.size(); i++)
    {
        double dist = ((double)distance[i]) /(1000*scale);
        
        if(dist>0.4 && dist<80)
        {
            Point p;
            p.y=std::sin(pitch)*dist;
            dist=std::cos(pitch)*dist;
            p.x= std::cos(angle) * dist;
            p.z = std::sin(angle) * dist;
            if(p.z>.7)
            {
                p.z *= -1;
                p.z += mountingHeight;
                workingRow.push_back(p);
            }
        }

        angle+=stepAngle;
    }
}

void PointCloudBuilder::rotateRow(double heading)
{
    heading=toRad(heading, 360);
    heading+=yaw;  // could go over 360... Though shouldn't matter
    for(int i=0; i<workingRow.size(); i++)
    {
        Point p = workingRow[i];
        double xyDist=findXYDist(p);
        p.y=xyDist * sin(heading); 
        p.x =xyDist * cos(heading);
        workingRow[i] = p;
    }
}

void PointCloudBuilder::placeRow(double northing, double easting)
{
    while(!workingRow.empty())
    {
        Point p = workingRow.back();
        workingRow.pop_back();
        p.x+=easting;
        p.y+=northing;

        cloud << std::fixed << std::showpoint;
        cloud << std::setprecision(6);
        cloud<<p.x<<" "<<p.y<<" "<<p.z<<std::endl;

        numberofPoints++;
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

void PointCloudBuilder::writeFile(std::string fileName)
{
    cloud.close();
    std::ifstream points;
    std::ofstream PCDFile;
    std::string pointLine;

    points.open("tempFile.txt");
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
        Point p;
        getline(points,pointLine);
        
        if(adjust)
        {
            p=readPointString(pointLine);
            adjustPoint(&p);
            PCDFile<<p.x<<" "<<p.y<<" "<<p.z<<std::endl;
        }
        else{PCDFile<<pointLine;}

       if(i>flush)
       {
           PCDFile.flush();
           flush+=100000;
           std::cout<<i/100000<<" of "<<numberofPoints/100000<<std::endl;
       }
    }
    points.close();
    PCDFile.flush();
    PCDFile.close();
}

void PointCloudBuilder::adjustPoint(Point *p)
{
    p->x -= minPoint.x;
    p->y -= minPoint.y;
    //p->z*= -1;
   // p->z += minPoint.z;
    
}

void PointCloudBuilder::updateMin(Point p)
{
    if(p.x<minPoint.x){minPoint.x = p.x;}
    if(p.y<minPoint.y){minPoint.y = p.y;}
    if(p.z<minPoint.z){minPoint.z = p.z;}
}

void PointCloudBuilder::setMin(int x, int y, int z)
{
    minPoint.x = x;
    minPoint.y = y;
    minPoint.z = z;
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