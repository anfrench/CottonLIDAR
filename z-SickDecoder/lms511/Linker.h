#pragma once
#include <queue>
#include <fstream>
#include <string>
#include "ModGPS.h"
#include "Spline.h"

using namespace std;

class Linker
{
	private:
	queue<ModGPS> *gps;
	ifstream gpsFile;
	protected:
	public:
	Linker();
	~Linker();
};









