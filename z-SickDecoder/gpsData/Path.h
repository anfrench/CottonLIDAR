#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include "ModGPS.h"


using namespace std;

class Path 
{
	private:
	vector<ModGPS> path;
	
	//methods
	vector<float> setUpBuffer(int buffNum);
	void endBuffer();
	float average(vector<float> buffer);
	protected:
	public:
	Path();
	void smooth(int buffNum);
	void createPath(string ggaFileName, string rmcFileName);
	ModGPS nextGPS();
	
};









