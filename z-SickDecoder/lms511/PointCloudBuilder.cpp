#include "PointCloudBuilder.h"

PointCloudBuilder::PointCloudBuilder()
{
    minPoint.x = 0;
    minPoint.y=0;
    minPoint.z = 0;
    cloud.open("tempFile.txt");
}

void PointCloudBuilder::addPoints(std::vector<int> distance, double angle, double stepAngle, double scale)
{
    angle = toRad(angle, 360);
    stepAngle = toRad(stepAngle, 360);
    for(int i=0; i<distance.size(); i++)
    {
        double dist = ((double)distance[i]) /(1000*scale);
        Point p;
        p.y=0;
        p.x= std::sin(angle) * dist;
        p.z = std::sin(angle) * dist;

        workingRow.push_back(p);

        angle+=stepAngle;
    }
}

void PointCloudBuilder::rotateRow(double heading)
{
    heading = toRad(heading, 360);
    for(int i=0; i<workingRow.size(); i++)
    {
        Point p = workingRow[i];
        p.y= p.x * sin(heading);
        p.x = p.x * cos(heading);
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
        updateMin(p);
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

void PointCloudBuilder::writeFile(std::string fileName)
{
    cloud.close();
    FILE *points;
    std::ofstream PCDFile;
    std::string pointLine;

    points=fopen("tempFile.txt", "r");
    PCDFile.open(fileName);

    PCDFile<<"# .PCD v.7 - Point Cloud Data file format\n";
    PCDFile<<"VERSION .7\n";
    PCDFile<<"FIELDS x y z\n";
    PCDFile<<"SIZE 4 4 4 4\n";
    PCDFile<<"TYPE F F F F\n";
    PCDFile<<"COUNT 1 1 1\n";
    PCDFile<<"WIDTH "<<numberofPoints<<"\n";
    PCDFile<<"HEIGHT 1\n";
    PCDFile<<"VIEWPOINT 0 0 0 1 0 0 0\n";
    PCDFile<<"POINTS "<<numberofPoints<< "\n";
    PCDFile<<"DATA ascii\n";
    for(int i=0; i<numberofPoints; i++)
    {
        Point p;
        fscanf(points,"%f %f %f", &p.x, &p.y, &p.z);
        adjustPoint(&p);
        PCDFile << std::fixed << std::showpoint;
        PCDFile << std::setprecision(6);
        PCDFile<<p.x<<" "<<p.y<<" "<<p.z<<std::endl;
        PCDFile.flush();
    }
    fclose(points);
    PCDFile.flush();
    PCDFile.close();
}

void PointCloudBuilder::adjustPoint(Point *p)
{
    p->x -= minPoint.x;
    p->y -= minPoint.y;
    p->z -= minPoint.z;
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