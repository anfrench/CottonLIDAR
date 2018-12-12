#include <string>
#include <fstream>
#include "GPSInterp.h"
#include "LMS511Data.h"
#include "PointCloudBuilder.h"
#include "MergeTime.h"

using namespace std;

int main()
{
	GPSInterp gps("zrawGPS.txt");
	string line;
	ifstream lidarFile;
	lidarFile.open("zrawLidar.txt");
	PointCloudBuilder builder;

	while(getline(lidarFile,line))
	{
		try
		{
		MergeTime transmitionTime;
		transmitionTime.setStamp(line);

		ModGPS location;
		location = gps.getLocation(transmitionTime.getTime());

		LMS511Scan lidar;
		lidar.setScan(line);
		lidar.decode();

		builder.addPoints(lidar.getDistValues(),lidar.getStartAngle(), lidar.getAngularStep());
		builder.rotateRow(location.getHeading());
		builder.placeRow(location.getNorthing(), location.getEasting());
		}
		catch(const char * e)
		{

		}
		catch(...){}
	}
	
	builder.writeFile("Test.pcd");

	return 0;
}
