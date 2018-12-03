#include "LMS400DataClasses.h"

//class LMS400DistanceOnly 
LMS400DistanceOnly::LMS400DistanceOnly(string scanIn):LMS400Scan(scanIn)
{
	partition();
}

LMS400DistanceOnly::LMS400DistanceOnly(){}

vector<int> LMS400DistanceOnly::getDistValues()
{
	return distValues;
}

void LMS400DistanceOnly::partition()
{
	LMS400Scan::partition(0,39,36);
}

void LMS400DistanceOnly::decodeData()
{
	distValues.clear();
	string temp = dataBlock, token;
	while(temp.size()>=4)
	{
		token = temp.substr(0,4);
		temp=temp.substr(4);
		token= swap2(token);
		distValues.push_back(hex2Dec(token));
	}
}
void LMS400DistanceOnly::decodeInfoBlocks()
{
	LMS400Scan::decodeInfoBlocks();
}



//class LMS400RemissionsOnly 
LMS400RemissionsOnly::LMS400RemissionsOnly(string scanIn):LMS400Scan(scanIn)
{
	partition();
}

LMS400RemissionsOnly::LMS400RemissionsOnly(){}

vector<int> LMS400RemissionsOnly::getRemisValues()
{
	return remisValues;
}

void LMS400RemissionsOnly::partition()
{
	LMS400Scan::partition(0,39,36);
}

void LMS400RemissionsOnly::decodeData()
{
	
	remisValues.clear();
	string temp = dataBlock, token;
	while(temp.size()>=2)
	{
		token = temp.substr(0,2);
		temp=temp.substr(2);
		remisValues.push_back(hex2Dec(token));
	}
}
void LMS400RemissionsOnly::decodeInfoBlocks()
{
	LMS400Scan::decodeInfoBlocks();
}




//class LMS400RemissionsAndDistance 
LMS400RemissionsAndDistance ::LMS400RemissionsAndDistance (string scanIn):LMS400Scan(scanIn)
{
	partition();
}

LMS400RemissionsAndDistance ::LMS400RemissionsAndDistance(){}

void LMS400RemissionsAndDistance ::partition()
{
	LMS400Scan::partition(0,39,36);
}

void LMS400RemissionsAndDistance ::decodeData()
{
	distValues.clear();
	remisValues.clear();
	string temp = dataBlock, token;
	while(temp.size()>=6)
	{
		token = temp.substr(0,4);
		temp=temp.substr(4);
		token= swap2(token);
		distValues.push_back(hex2Dec(token));

		token = temp.substr(0,2);
		temp=temp.substr(2);
		remisValues.push_back(hex2Dec(token));
	}
}
void LMS400RemissionsAndDistance ::decodeInfoBlocks()
{
	LMS400Scan::decodeInfoBlocks();
}


vector<int> LMS400RemissionsAndDistance::getDistValues()
{
	return distValues;
}

vector<int> LMS400RemissionsAndDistance::getRemisValues()
{
	return remisValues;
}



