#include "Path.h"

Path::Path(){}



void Path::smooth(int buffNum)
{
	if(buffNum%2 == 0 ){buffNum++;}
	
	vector<float> buffer;
	
	buffer=setUpBuffer(buffNum);
	
	int buffRange = (buffNum-1)/2;
	
	for(int i = buffRange; i< path.size()-buffRange; i++)
	{
		path[i].setHeading(average(buffer));
		buffer.push_back(path[buffNum].getHeading());
		//line for poping other end off
		buffer.erase(buffer.begin());
		buffNum++;
	}
}

vector<float> Path::setUpBuffer(int buffNum)
{
	vector<float> buffer;
	for(int i = 0 ; i< buffNum; i++)
	{
		if(i>= path.size()){break;}
		buffer.push_back(path[i].getHeading());
		path[i].setHeading(average(buffer));
	}
	return buffer;
}

float Path::average(vector<float> buffer)
{
	float x, y;
	float heading;
	
	x = y = heading = 0;
	
	for(int i=0; i<buffer.size(); i++)
	{
		x += cos(M_PI*buffer[i]/180);
		y += sin(M_PI*buffer[i]/180);
	}
	
	x = x/ buffer.size();
	y = y/ buffer.size();
	
	heading = atan2(y, x);

	heading = (heading / M_PI) * 180.0;
		
	return heading;
}



void createPath(string ggaFileName, string rmcFileName)
{
	fstream ggaFile, rmcFile;
	
	ModGPS mod;
	RMC rmc;
	GGA gga;
	
	string gps;

	rmcFile.open(rmcFileName, fstream::in);
	while(getline(rmcFile, gps))
	{
		rmc.setString(gps);
		mod.setHeading(rmc.getHeading());
		mod.setSpeed(rmc.getSpeed());
		mod.setNorthing(rmc.getNorthing());
		mod.setEasting(rmc.getEasting());
		mod.setUTC(rmc.getUTC());
	}
}














