#pragma once
#include "LMS400DataBase.h"
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class LMS400DistanceOnly : public LMS400Scan
{
	private:
	protected:
	vector<int> distValues;
	virtual void partition();
	virtual void decodeData();
	virtual void decodeInfoBlocks();
	
	public:
	LMS400DistanceOnly(string scanIn);
	LMS400DistanceOnly();
	vector<int> getDistValues();
};



class LMS400RemissionsOnly : public LMS400Scan
{
	private:
	protected:
	vector<int> remisValues;
	virtual void partition();
	virtual void decodeData();
	virtual void decodeInfoBlocks();
	
	public:
	LMS400RemissionsOnly(string scanIn);
	LMS400RemissionsOnly();
	vector<int> getRemisValues();
};



class LMS400RemissionsAndDistance : public LMS400Scan
{
	private:
	protected:
	vector<int> remisValues;
	vector<int> distValues;
	virtual void partition();
	virtual void decodeData();
	virtual void decodeInfoBlocks();
	
	public:
	LMS400RemissionsAndDistance(string scanIn);
	LMS400RemissionsAndDistance();
	vector<int> getRemisValues();
	vector<int> getDistValues();
};


































