#include <string>
#include <fstream>
#include "GPSInterp.h"
#include "LMS511Data.h"
#include "PointCloudBuilder.h"
#include "MergeTime.h"


#define CATCH 0
#define DBUG 0
#define PROGRESS 1

using namespace std;


class LidarToPCD
{
private:
    GPSInterp *gps=NULL;
    ifstream *lidarFile=NULL;
    PointCloudBuilder *builder=NULL;
    LMS400Scan* lidar=NULL; // All curent lidar classes are built from this base class
    bool findBestRoll=false, findBestPitch=false;

#if PROGRESS
	int countLidarLines(std::string fineName);
#endif

    
protected:
    string outFileName="", lidarFileName="", gpsFileName="";
    double rollOffset=0, pitchOffset=0, yawOffset=0; 
    double pitch=0, roll=0;
    double mountingHeight=0, offsetAngle=0, offsetDistance=0;
    int currentScan=0, currentStep=0, totalScans=0;
    int maxPoints=0, LeadingPoints=0;
	int lidarType=0;  //  used to set the lidar pointer
	bool done=false;
    Point upperBounds, lowerBounds, shift;


    LMS400Scan* getLidar(int lidarType);
    void processLidarReccord(string reccord);
    
public:
    void setLidarType(int type);
    void setRollOffset(double offset);
    void setPitchOffset(double offset);
    void setYawOffset(double offset);
    void setLidarFileName(string lidarFileNameIN);
    void setGPSFileName(string gpsFileNameIN);
    void setOutFileName(string outFileNameIN);
    void setUpperBounds(Point p);
    void setLowerBounds(Point P);
    void setShift(Point shiftIN);
    void setMountingXYDist(double distance);
    void setMountingXYAngle(double angle);
    void setMountingHeight(double angle);
    void setMaxPoints(int maxPointsIN);
    void setLeadingPoints(int leadingPointsIN);
    void initalize();
    void writeFile();
    void findBestRollOffset();
    void findBestPitchOffset();
    void process();
    void dumpCurrent();
};