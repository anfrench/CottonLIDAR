#include "LidarToPCD.h"

LidarToPCD::~LidarToPCD(){dumpCurrent();}

#if PROGRESS
int LidarToPCD::countLidarLines(std::string fileName)
{
	int count =0;
	std::ifstream lidar;
	std::string line;
	for(lidar.open(fileName); getline(lidar,line); count++){}
	lidar.close();
	return count;
}
#endif

void LidarToPCD::setLidarType(int type)
{
    lidarType=type;
}

void LidarToPCD::setLidarFileName(string lidarFileNameIN)
{
    lidarFileName=lidarFileNameIN;
}

void LidarToPCD::setRollOffset(double offset)
{
    roll=offset;
}

void LidarToPCD::setPitchOffset(double offset)
{
    pitch=offset;
}

void LidarToPCD::setYawOffset(double offset)
{
    yawOffset=offset;
}

void LidarToPCD::setGPSFileName(string gpsFileNameIN)
{
    gpsFileName=gpsFileNameIN;
}

void LidarToPCD::setOutFileName(string outFileNameIN)
{
    outFileName=outFileNameIN;
}

void LidarToPCD::setUpperBounds(Point p)
{
    upperBounds=p;
}

void LidarToPCD::setLowerBounds(Point p)
{
    lowerBounds=p;
}

void LidarToPCD::setShift(Point shiftIN)
{
    shift=shiftIN;
}

void LidarToPCD::setMountingXYDist(double distance)
{
    offsetDistance=distance;
}

void LidarToPCD::setMountingXYAngle(double angle)
{
    offsetAngle=angle;
}

void LidarToPCD::setMountingHeight(double height)
{
    mountingHeight=height;
}

void LidarToPCD::setMaxPoints(int maxPointsIN)
{
    maxPoints=maxPointsIN;
}

void LidarToPCD::setLeadingPoints(int leadingPointsIN)
{
    LeadingPoints=leadingPointsIN;
}

LMS400Scan* LidarToPCD::getLidar(int lidarType)
{
	switch(lidarType)
	{
		case 0:
			return new(LMS511Scan);
		case 1:
		break;
	}
	return new(LMS511Scan);
}

void LidarToPCD::writeFile()
{
    builder->writeFile(outFileName);
}

void LidarToPCD::initalize()
{
    try
    {
#if PROGRESS
	totalScans=countLidarLines(lidarFileName);
#endif

    done=false;
    dumpCurrent();

    lidarFile=new(ifstream);
    builder=new(PointCloudBuilder);
    gps=new(GPSInterp);


    lidarFile->open(lidarFileName);
    if(!lidarFile->is_open()){throw "Could NOT open Lidar File.";}

    gps->setOffsetAngle(offsetAngle);
    gps->setOffsetDist(offsetDistance);
    gps->openFile(gpsFileName);
    
    pitchOffset=gps->getPitchOffset()+pitch;
    rollOffset=gps->getRollOffset()+roll;

    builder->setBounds(lowerBounds, upperBounds);
    builder->setShift(shift);
    builder->setMaxPoints(maxPoints);
    builder->setLeadingPoints(LeadingPoints);
    builder->setMountingHeight(mountingHeight);

    }
    catch(const std::exception& e)
	{
		cout << e.what() << '\n';
	}
    catch(...)
    {
        throw "Initalization Error!";
    }
    
}

void LidarToPCD::process()
{
    initalize();
    string line;
    while(getline(*lidarFile,line) && !done)
	{
		processLidarReccord(line);
	}
}

void LidarToPCD::processLidarReccord(string line)
{
    try
		{
		#if PROGRESS
		currentScan ++;
		if(currentScan>currentStep)
		{
			std::cout<<"\tProgress: "<<currentScan/1000
			<<" of "<<totalScans/1000<<endl;
			currentStep += 1000;
		}
		#endif

		if(line.find("RECEIVE")==string::npos){throw "Lidar record corrupted.\n";}

		MergeTime transmitionTime;
		transmitionTime.setStamp(line);
		#if DBUG
				cout <<"Found time stamp"<<endl; 
		#endif
	
		ExtendedGps location;
		location = gps->getLocation(transmitionTime.getTime());
		#if DBUG
				cout <<"Updated/recived GPS"<<endl; 
		#endif
        lidar=getLidar(lidarType);
		lidar->setScan(line);
		lidar->decode();
		#if DBUG
				cout <<"Decoded Lidar"<<endl; 
		#endif
		builder->setRoll(rollOffset+location.getRoll()+lidar->getStartAngle());
		builder->setPitch(location.getPitch());
		builder->setYaw(yawOffset+location.getYaw());


        /* old ...
		builder->addPoints(lidar->getDistValues(),lidar->getStartAngle(), lidar->getAngularStep(), lidar->getScaler());
		builder->rotateRow(location.getYaw());
		builder->placeRow(location.getNorthing(), location.getEasting());
        */

       //new ... Much Cleaner... So nice.
        builder->setDistValues(lidar->getDistValues());
        builder->setAngularStep(lidar->getAngularStep());
        builder->setScale(lidar->getScaler());

        if(findBestRoll)
            {builder->findBestRoll();}
            
        builder->process();
        builder->placeRow(location.getNorthing(), location.getEasting());
		done=builder->isDone();
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
        if(lidar!=NULL){delete(lidar); lidar=NULL;}
}

void LidarToPCD::findBestRollOffset()
{
    findBestRoll=true;
}

void LidarToPCD::findBestPitchOffset()
{
    findBestPitch=true;
}

void LidarToPCD::dumpCurrent()
{
    currentScan=0;
    currentStep=0;
    if(lidarFile!=NULL){lidarFile->close(); delete(lidarFile); lidarFile=NULL;}
    if(builder!=NULL){delete(builder); builder=NULL;}
    if(gps!=NULL){gps->close(); delete(gps); gps=NULL;}
    if(lidar!=NULL){delete(lidar); lidar=NULL;}
}

