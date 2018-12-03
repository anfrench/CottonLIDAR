#include <iostream>
#include <string>
#include <fstream>
#include "ModGPS.h"

using namespace std;

int main()
{
	ifstream rawGPS, rawLidar;
	string gpsString, lidarString;
	
	
	rawGPS.open("zrawGPS.txt");
	
	while(getline(rawGPS, gpsString))
	{
		
	}
	
	
	return 1;
}
