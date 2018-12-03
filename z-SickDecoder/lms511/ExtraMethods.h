#include <string>

using namespace std;

double timeToDouble(int Hour, int minute, int seconds, int fractionalSeconds)
{
	double time =0;
	
	time= (double) seconds;
	time+= (double) minutes *60;
	time += (double) Hour *3600;

	return time;
}

double convertTime(string timeString)
{
	if(timeString.find(",") != string::npos)
	{
		return ;
	}
	else{return ;}
}
