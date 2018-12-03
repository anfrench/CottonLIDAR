#include <stdio.h>
#include <math.h>
#include "includes.h"
#define toCenter

using namespace pcl;
using namespace std;

pointXYZ makePoint(float scanDist,float phi,float theta);
float round2Dec(float numb);

int main()
{
	string fileName;
	vector<PointXYZ> preCloud;
	pointCloud<PointXYZ> cloud;
	double X,Y,Z;

	for()
	{
		//have to figure out how to run loop makePoint()
		preCloud.push_back(makePoint(/*Fill in*/));
	}
	
	cloud.width= preCloud.size();
	cloud.height=1;
	cloud.is_dense= true;
	cloud.points.resize(cloud.width);
	//this may also work... have to test
	//cloud.points=preCloud;
	
	for(int i=0; i<preCloud.size(); i++)
	{
		cloud.points[i]=preCloud[i];
	}
	
	io::savePCDFileASCII(fileName, cloud);
	
}

pointXYZ makePoint(float scanDist,float phi,float theta)
{

	float radious, X, Y, Z;
	PointXYZ point (new pointXYZ);
		
	Z=scanDist*sin(phi);//may have to change from degrees
	radious= scanDist*cos(phi);
	radious= toCenter-radious;
	X=radious*cos(theta);
	Y=radious*sin(theta);

	point.x=round2Dec(X);
	point.x=round2Dec(Y);
	point.x=round2Dec(Z);
	
	return point;
}

float round2Dec(float numb)
{
	float value = (int)(numb*100 + .5);
	return (float)value/100;
}
