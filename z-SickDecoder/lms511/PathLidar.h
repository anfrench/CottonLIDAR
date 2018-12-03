#pragma once
#include <string>
#include <vector>
#include <cmath>
#include "ModGPS.h"
#include "LMS511Data.h"

using namespace std;

class Point 
{
	public:
	double x,y,z;
};

class PathLidar
{
	private:
	float height, xOffset, yOffset;
	int timeCorrection;
	Path path;
	vector<Point> cloud;
	string lidarFileName;
	
	ModGPS calculateLidarUTM(ModGPS gps);
	Point calculatePoint(float theta, float distance, ModGPS gps);
	vector<Point> calculateScan(ModGPS gps, LMS511Scan scan);
	void buildCloud();
	double toRad(double angle, int steps);
	double toDegree(double angle, int steps);
	int correctTime(LMS511Scan scan);
	protected:
	public:
	PathLidar();
	void setOffsets(float heightIN,float xOffsetIN,float yOffsetIN);
	void setFileNames(string lidarFile, string gpsFile);
	

};
