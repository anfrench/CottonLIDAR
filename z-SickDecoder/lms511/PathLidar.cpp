#include "PathLidar.h"

//Private
PathLidar::PathLidar(){}

double PathLidar::toRad(double angle, int steps)
{
	return M_PI * angle / (steps / 2 );
}

double PathLidar::toDegree(double angle, int steps)
{
	return (steps / 2 ) * angle / M_PI;
}


Point PathLidar::calculatePoint(float theta, float distance, ModGPS gps)
{
	Point point;
	float dist;

	point.z	= distance*cos(toRad(theta,360));
	dist 	= distance*sin(toRad(theta,360));
	point.x = dist*cos(toRad(gps.getHeading(),360));
	point.y = dist*sin(toRad(gps.getHeading(),360));

	point.z = height - point.z;
	point.x += gps.getEasting();
	point.y += gps.getNorthing();

	return point;	
}

vector<Point> PathLidar::calculateScan(ModGPS gps, LMS511Scan scan)
{
	vector<Point> pointScan;
	vector<int> distances;
	float angle = scan.getStartAngle();
	float stepSize = scan.getAngularStep();
	
	//this is where i'm at... 
	gps=calculateLidarUTM(gps);

	for(int i=0; i < distances.size(); i++)
	{
		pointScan.push_back(calculatePoint(angle, distances[i], gps));
		
		angle += stepSize;
	}
	
	return pointScan;
}

void PathLidar::buildCloud()
{
	vector<Point> temp;
	fstream  lidarFile;
	string scanLine;
	ModGPS gps;
	LMS511Scan *scan;
	int epoch;

	lidarFile.open(lidarFileName, fstream::in);
	while(getline(lidarFile, scanLine))
	{
		scan->setScan(scanLine);
		gps = path.getNext(correctTime(*scan));
		temp = calculateScan(gps, *scan);
		cloud.insert(std::end(cloud), std::begin(temp),std::end(temp));
		
	}
}


ModGPS PathLidar::calculateLidarUTM(ModGPS gps)
{
	float northing, easting, heading;
	northing = gps.getNorthing();
	easting = gps.getEasting();
	heading = toRad(gps.getHeading(),360);

	northing += xOffset*sin(heading)+yOffset*cos(heading);
	easting  += xOffset*cos(heading)-yOffset*sin(heading);

	gps.setNorthing(northing);
	gps.setEasting(easting);

	return gps;
}

int PathLidar::correctTime(LMS511Scan scan)
{
	int UTC;
	scan.getTransmitionTime();
	UTC+= timeCorrection;
	return UTC;
}



void PathLidar::setOffsets(float heightIN,float xOffsetIN,float yOffsetIN)
{
	height = heightIN;
 	xOffset =xOffsetIN;
	yOffset = yOffsetIN;
}

/*
void PathLidar::setFileNames(string lidarFile, string gpsFile)
{
	lidarFileName = lidarFile;
	path.setFileName(gpsFile);
}


*/





