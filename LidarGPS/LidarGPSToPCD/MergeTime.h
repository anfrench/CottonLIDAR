#pragma once
#include <string>
#include <iostream>

using namespace std;
class MergeTime
{
	private:
	int month, day;
	int hour, minute, second;
	float fractionalSecond;
	int utc;
	double time;
	string stamp;
	protected:
	public:
	MergeTime();
	
	void decode();
	void timeToDouble();
	void decodeLidarTime();
	void decodeGPSTime();
	void setStamp(string stampIN);
	
	int getUTC();
	float getFractonalSeconds();
	
	double getTime();
	
	bool equal(MergeTime time);	
	
	string toString();
	string cutBack(string str, string cutPoint, int offsetz);
	string cutFront(string str, string cutPoint, int offsetz);
};

