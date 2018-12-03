#include "TurnTable.h"
#include <iostream>
//Point class
Point::Point(){x=y=z=0;}

//TurnTableClass
//public
TurnTable::TurnTable()
{
	distToLidar = 1;
	height = 0;
	scale = 1;
	skew = 0; 
	pitch = 0;
	stepsPerRev = 0; 
	scansPerStep = 1;
	turnTableStep=0;
	turnTableAngle =0; 
}

void TurnTable::setSpatialParams(double distToLidarIn, double heightIn, double scaleIn)
{
	setDistance(distToLidarIn);
	setHeight(heightIn);
	setScale( scaleIn);
}

void TurnTable::setAngles(double skewIn, double pitchIn)
{
	setSkew(skewIn);
	setPitch(pitchIn);	
}

void TurnTable::setStepperParam(int stepsPerRevIn, int scansPerStepIn)
{
	setStepsPerRev(stepsPerRevIn);
	setScansPerStep(scansPerStepIn);
}

void TurnTable::setDistance(double distIN)
{
	distToLidar = distIN;
}

double TurnTable::getDistance()
{
	return distToLidar;
}

void	TurnTable::setHeight(double heightIN)
{
	height = heightIN;
}

double	TurnTable::getHeight()
{
	return height;
}

void	TurnTable::setScale(double scaleIN)
{
	scale = scaleIN;
}

double 	TurnTable::getScale()
{
	return scale;
}

void 	TurnTable::setSkew(double skewIn)
{
	skew = toRad(skewIn, 360);
}

double	TurnTable::getSkew()
{
	return skew;
}
void	TurnTable::setPitch(double angleIn)
{
	pitch = toRad(angleIn,360);
}

double	TurnTable::getPitch()
{
	return pitch;
}

void TurnTable::setStepsPerRev(int stepsIN)
{
	stepsPerRev = stepsIN;
}

int TurnTable::getStepsPerRev()
{
	return stepsPerRev;
}

void	TurnTable::setScansPerStep(int scansPerIn)
{
	scansPerStep = scansPerIn;
}
int	TurnTable::getScansperStep()
{
	return scansPerStep;
}

void TurnTable::setFileName(string nameIN)
{
	fileName=nameIN;
}

string TurnTable::getFileName()
{
	return fileName;
}

vector<Point> TurnTable::getCloud()
{
	return cloud;
}


//Private
int TurnTable::updateStepperAngle(int scanNumb)
{
	if(scanNumb >= scansPerStep)
	{
		scanNumb = 0;
		turnTableStep++;
		turnTableAngle = toRad((double) turnTableStep, stepsPerRev);
	}
	return scanNumb;
}

double TurnTable::toRad(double angle, int steps)
{
	return PI * angle / (steps / 2 );
}

double TurnTable::toDegree(double angle, int steps)
{
	return (steps / 2 ) * angle / PI;
}

double TurnTable::calculateXYAngle(double angle)
{
	return angle*sin(skew);
}

double TurnTable::calculateYXDist(double distance,double angle)
{
	return distance*cos(angle*cos(skew));
}

double TurnTable::calculateXPrime(double r, double xyAngle)
{
	return distToLidar -(r *cos (xyAngle ));
}
double TurnTable::calculateYPrime(double r, double xyAngle )
{
	return r * sin(xyAngle );
}

double TurnTable::calculateX(double xPrime, double yPrime)
{
	return  xPrime * cos(turnTableAngle) - yPrime * sin(turnTableAngle);
}

double TurnTable::calculateY(double xPrime, double yPrime)
{
	return xPrime * sin(turnTableAngle) + yPrime * cos(turnTableAngle);
}

double TurnTable::calculateZ(double distance, double angle)
{
	return distance*sin(angle*cos(skew));
}

Point TurnTable::scalePoint(Point point)
{
	point.x = point.x/scale;
	point.y = point.y/scale;
	point.z = point.z/scale;
	
	return point;
}

Point TurnTable::calculatePoint(double distance, double angle)
{
	Point point;
	double xPrime, yPrime;
	double xyAngle;
	double xyDist;
	
	xyAngle = calculateXYAngle(angle);
	xyDist = calculateYXDist(distance, angle);
	xPrime = calculateXPrime(xyDist, xyAngle);
	yPrime = calculateYPrime(xyDist, xyAngle);
	
	point.x = calculateX(xPrime, yPrime);
	point.y = calculateY(xPrime, yPrime);
	point.z = calculateZ(distance, angle);
	
	return scalePoint(point);
}

vector<Point> TurnTable::calculateScan(LMS400Scan *scan)
{
	double angle, stepVal;
	vector<Point> scanPoints;
	vector<int> distVals;
	
	scan->decode();
	distVals = scan->getDistValues();
	angle = toRad((double)scan->getStartAngle()/10000, 360);
	angle += pitch;
	stepVal = toRad((double)scan->getAngularStep()/10000,360);
	
	for(int i=0; i<distVals.size(); i++)
	{
		scanPoints.push_back(calculatePoint(distVals[i], angle));
		angle -= stepVal;
	}
	
	return scanPoints;
}

void TurnTable::turnTable()
{
	int scanNumb=0;
	LMS400Scan *scan;
	LMS400DistanceOnly dist;
	scan= &dist;
	string data;
	vector<Point> temp;
	ifstream infile(fileName.c_str()); //open the file
	if (infile.is_open() && infile.good()) 
	{
		while (getline(infile, data))
		{
			scanNumb++;
			scan->setScan(data);
			temp = calculateScan(scan);
			cloud.insert(std::end(cloud), std::begin(temp),std::end(temp));
			scanNumb = updateStepperAngle(scanNumb);
			
		}
	}	

}



















