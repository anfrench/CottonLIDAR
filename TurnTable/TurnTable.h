#include <string>
#include <vector>
#include <math.h>
#include <fstream>
#include "LMS400DataClasses.h"
#define PI 3.14159265

using namespace std;


class Point 
{
	public:
	double x,y,z;
	Point();
};


class TurnTable
{
	private:
	string fileName;

	//Paramiters
	double distToLidar, height;
	double scale;
	double skew, pitch;
	int stepsPerRev, scansPerStep;
	
	//Variables
	double turnTableAngle;
	int turnTableStep;
	
	
	//product
	vector<Point> cloud;
	
	//Methods
	int updateStepperAngle(int scanNumb);
	double toDegree(double angle, int steps);
	double toRad(double angle, int steps);
	double calculateXYAngle(double angle);
	double calculateYXDist(double distance,double angle);
	double calculateXPrime(double r, double xyAngle);
	double calculateYPrime(double r, double xyAngle );
	double calculateX(double xPrime, double yPrime);
	double calculateY(double xPrime, double yPrime);
	double calculateZ(double distance, double angle);
	Point scalePoint(Point point);
	
	Point calculatePoint(double distance, double angle);
	vector<Point> calculateScan(LMS400Scan *scan);
	protected:
	public:
	TurnTable();
	void turnTable();
	
	//Param setters
	void setSpatialParams(double distToLidarIn, double heightIn, double scaleIn);
	void setAngles(double skewIn, double pitchIn);
	void setStepperParam(int stepsPerRevIn, int ScansPerStepIn);
	
	//getters Setters
	void 	setDistance(double distIN);
	double 	getDistance();
	void	setHeight(double heightIN);
	double	getHeight();
	void	setScale(double scaleIN);
	double 	getScale();
	void 	setSkew(double skewIn);
	double	getSkew();
	void	setPitch(double angleIn);
	double	getPitch();
	void 	setStepsPerRev(int stepsIN);
	int 	getStepsPerRev();
	void	setScansPerStep(int ScansPerIn);
	int		getScansperStep();
	void 	setFileName(string nameIN);
	string 	getFileName();
	
	//geting Point Cloud
	vector<Point> getCloud();
	
};







