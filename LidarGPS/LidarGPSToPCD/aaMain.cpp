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
void Error(ConfigReader *reader);
void getLidar(int lidarType, LMS400Scan *lidar);

using namespace std;

int main()
{
	string outFileName;
	int timeoffsetMS;
	string line;
	ifstream lidarFile;
	GPSInterp gps;
	ConfigReader *configuration;
	PointCloudBuilder builder;
	LMS400Scan* lidar; // All curent lidar classes are built from this base class
	int lidarType=0;  //  used to set the lidar pointer

	try
	{
		string ConfigurationFileName;
		cout<<"Enter The Configuration File Name: ";
		getline(cin, ConfigurationFileName);

		configuration = new(ConfigReader);
		configuration->read(ConfigurationFileName);

		lidarFile.open(configuration->getLidarFileName());
		lidarType=configuration->getLidarType();

		gps.openFile(configuration->getGPSFileName());
		outFileName=configuration->getOutputFileName();
		if(!lidarFile.is_open()){throw "Could NOT open Lidar File.";}
		

		gps.setOffsetDist(configuration->getMountingXYDist());
		gps.setOffsetAngle(configuration->getMountingAngle());
		

		builder.setMountingHeight(configuration->getMountingHeight());
		builder.setRoll(configuration->getRoll());
		builder.setPitch(configuration->getPitch());
		builder.setYaw(configuration->getYaw()); 

		builder.setBounds(configuration->getLowerBounds(),configuration->getUpperBounds());
		builder.setShift(configuration->getShift());


		delete(configuration);
	}
	catch(const char *e)
	{
		cout<<e<<endl;
		Error(configuration);
	}
	catch(...)
	{
		cout<<"Not sure what went Wrong\n";
		Error(configuration);
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
			std::cout<<"\tProgress: "<<currentScan/10000
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

		getLidar(lidarType,lidar);
		lidar->setScan(line);
		lidar->decode();
		#if DBUG
				cout <<"Decoded Lidar"<<endl; 
		#endif
		builder.addPoints(lidar->getDistValues(),lidar->getStartAngle(), lidar->getAngularStep(), lidar->getScaler());
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


void Error(ConfigReader *reader)
{
	string answer;
	cout<<"\tWould you like a sample configuration file to print? (y/n)";
	getline(cin,answer);
	if(answer.find("y") != string::npos ||answer.find("Y") != string::npos)
	{
		cout<<endl<<endl<<reader->makeEmptyConfigFile()<<endl<<endl;
	}
	delete(reader);


	exit(EXIT_FAILURE);
}


void getLidar(int lidarType, LMS400Scan *lidar)
{
	if(lidar!=NULL)
	{
		delete(lidar);
	}

	switch(lidarType)
	{
		case 0:
			lidar= new(LMS511Scan);
			break;
		case 1:
		break;
	}
}