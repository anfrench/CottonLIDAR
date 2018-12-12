#include <string>
#include <fstream>
#include "GPSInterp.h"
#include "LMS511Data.h"
#include "PointCloudBuilder.h"
#include "MergeTime.h"

#define CATCH 1
#define PROGRESS 1

#if PROGRESS
	int countLidarLines(std::string fineName);
#endif


using namespace std;

int main()
{
	GPSInterp gps("zrawGPS.txt");
	string line;
	ifstream lidarFile;
	lidarFile.open("zrawLidar.txt");
	PointCloudBuilder builder;

	gps.setOffsetDist(1);

	#if PROGRESS
	int currentScan=0;
	int currentStep=0;
	int totalScans=countLidarLines("zrawLidar.txt");
	#endif


	while(getline(lidarFile,line))
	{
		try
		{
		#if PROGRESS
		currentStep ++;
		if(currentScan>currentStep)
		{
			std::cout<<"\t\t\tProgress: "<<currentScan<<" of "<<totalScans<<endl;
			currentStep ++;
		}
		#endif

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
			#if CATCH
				cout <<e<<endl<<endl;
			#endif
		}
		catch(...)
		{
			#if CATCH
				cout <<"Unexpected Error."<<endl<<endl;
			#endif
		}
	}
	
	builder.writeFile("Test.pcd");

	return 0;
}


#if PROGRESS
int countLidarLines(std::string fileName)
{
	int count =0;
	std::ifstream lidar;
	std::string line;
	for(lidar.open(fileName); getline(lidar,line); count++){}
	lidar.close();
	return count;
}

#endif