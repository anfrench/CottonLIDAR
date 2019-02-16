#include <string>
#include <fstream>
#include "GPSInterp.h"
#include "LMS511Data.h"
#include "PointCloudBuilder.h"
#include "MergeTime.h"
#include "ConfigReader.h"

#define CATCH 0
#define PROGRESS 1
#define DBUG 0

#if PROGRESS
	int countLidarLines(std::string fineName);
#endif


using namespace std;

int main()
{
	string outFileName;
	int timeoffsetMS;
	string line;
	ifstream lidarFile;
	GPSInterp gps;
	ConfigReader *configuration;

	int test = 3;

	try
	{
		string gpsFileName, lidarFileName;

		cout<<"Enter The GPS File Name: ";
		getline(cin, gpsFileName);
		cout<<"Enter The Lidar File Name: ";
		getline(cin, lidarFileName);

		gps.openFile(gpsFileName);
		lidarFile.open(lidarFileName);
		if(!lidarFile.is_open()){throw "Could NOT open Lidar File.";}
	}
	catch(const char * e)
	{
		cout<<e;
		exit(EXIT_FAILURE);
	}

	PointCloudBuilder builder;
	builder.setMin(445389.275375, 3656190.256213,1.7); //should be aded to configuration..probably...

	try
	{
		string ConfigurationFileName;
		cout<<"Enter The Configuration File Name: ";
		getline(cin, ConfigurationFileName);
		cout<<"What Would You Like To Save As? ";
		getline(cin, outFileName);

		configuration = new(ConfigReader);
		configuration->read(ConfigurationFileName);
		
		gps.setOffsetDist(configuration->getMountingXYDist());
		gps.setOffsetAngle(configuration->getMountingAngle());
		
		builder.setMountingHeight(configuration->getMountingHeight());
		builder.setRoll(configuration->getRoll());
		builder.setPitch(configuration->getPitch()); //not acounted for (Does nothing)
		builder.setYaw(configuration->getYaw()); //not acounted for (Does nothing)

		delete(configuration);
	}
	catch(const char *e)
	{
		cout<<e<<endl;
		cout<<"\tMaking Sample Config File.\n\n";
		cout<<configuration->makeEmptyConfigFile();
		delete(configuration);
		exit(EXIT_FAILURE);
	}
	catch(...)
	{
		cout<<"Error Reading Configuration File...\n";
		cout<<"\tMaking Sample Config File.\n";
		cout<<configuration->makeEmptyConfigFile(); 
		delete(configuration);
		exit(EXIT_FAILURE);
	}

	gps.setOffsetDist(1);  // dont remember what this is for... perhaps add to config

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
		currentScan ++;
		if(currentScan>currentStep)
		{
			std::cout<<"\t\t\tProgress: "<<currentScan/10000
			<<" of "<<totalScans/10000<<endl;
			currentStep += 10000;
		}
		#endif

		MergeTime transmitionTime;
		transmitionTime.setStamp(line);
		#if DBUG
				cout <<"Found time stamp"<<endl; 
		#endif

		ModGPS location;
		location = gps.getLocation(transmitionTime.getTime());
		#if DBUG
				cout <<"Updated/recived GPS"<<endl; 
		#endif

		LMS511Scan lidar;
		lidar.setScan(line);
		lidar.decode();
		#if DBUG
				cout <<"Decoded Lidar"<<endl; 
		#endif
		builder.addPoints(lidar.getDistValues(),lidar.getStartAngle(), lidar.getAngularStep(), lidar.getScaler());
		builder.rotateRow(location.getHeading());
		builder.placeRow(location.getNorthing(), location.getEasting());
		#if DBUG
				cout <<"Processed a line"<<endl<<endl; 
		#endif
		
		}
		catch(const char * e)
		{
			#if CATCH
				cout <<e<<endl<<endl;
			#endif
		}
		catch(const std::exception &exc)
		{
			#if CATCH
				cout<<exc.what();
			#endif
		}
		catch(...)
		{
			#if CATCH
				cout <<"Unexpected Error."<<endl<<endl;
			#endif
		}
	}

	
	builder.writeFile(outFileName);

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
