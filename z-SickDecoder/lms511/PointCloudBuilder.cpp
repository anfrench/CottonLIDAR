#include "PointCloudBuilder.h"

PointCloudBuilder::PointCloudBuilder(){}

void PointCloudBuilder::addPoints(std::vector<int> distance, double angle, double stepAngle)
{
    for(int i=0; i<distance.size(); i++)
    {
        Point p;
        p.y=0;
        p.x= std::cos(angle) * distance[i];
        p.z = std::sin(angle) * distance[i];

        workingRow.push_back(p);

        angle+=stepAngle;
    }
}

void PointCloudBuilder::rotateRow(double heading)
{
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
        cloud.push_back(p);
    }
}