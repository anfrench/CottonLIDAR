#include "Linker.h"
#include <iostream>

Linker::Linker()
{
	gps = new queue<ModGPS>;
	gpsFile.open("zrawGPS.txt");
	
	while(gps->size()<500)
	{
		try
		{
		string line;
		ModGPS newgps;
		getline(gpsFile, line);
		newgps.readGpsString(line);
		
		gps->push(newgps);
		}
		catch(const char* error)
		{
			cout<<error<<endl;
		}
	}
	
}

Linker::~Linker()
{
	delete gps;
}



