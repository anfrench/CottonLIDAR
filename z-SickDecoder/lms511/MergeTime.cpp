#include "MergeTime.h"

MergeTime::MergeTime(){}
void MergeTime::setStamp(string stampIN){stamp = stampIN; decode();}
int MergeTime::getUTC(){return utc;}
float MergeTime::getFractonalSeconds(){return fractionalSecond;}
string MergeTime::toString(){return stamp;}
bool MergeTime::equal(MergeTime time){return utc == time.getUTC(); }	

void MergeTime::decode()
{
	if(stamp.find(",") != string::npos)
	{
		decodeGPSTime();
	}
	else
	{
		decodeLidarTime();
	}
	timeToDouble();
}

void MergeTime::decodeLidarTime()
{
	string temp, token[6];
	temp = stamp.substr(stamp.find("-")+1);

	token[0] = temp.substr(0,temp.find("-"));
	temp = temp.substr(temp.find("-")+1);

	token[1] = temp.substr(0,2);
	temp = temp.substr(3);

	token[2] = temp.substr(0,temp.find(":"));
	temp = temp.substr(temp.find(":")+1);

	token[3] = temp.substr(0,temp.find(":"));
	temp = temp.substr(temp.find(":")+1);

	token[4] = temp.substr(0,temp.find("."));
	temp = temp.substr(temp.find(".")+1);

	token[5] = temp.substr(0,temp.find("-")-1);
	if(token[5].length()>3){token[5] = token[5].substr(0,3);}
	
	month = stoi(token[0]);
	day= stoi(token[1]);
	hour= stoi(token[2]);
	minute= stoi(token[3]);
	second= stoi(token[4]);
	fractionalSecond = stof(token[5]);
}
	
void MergeTime::decodeGPSTime()
{

	string temp, token[6];
	temp = stamp.substr(stamp.find(",")+1);
	temp = temp.substr(temp.find(",")+1);
	
	for(int i=0; i<6; i++)
	{
		token[i] = temp.substr(0,temp.find(","));
		temp = temp.substr(temp.find(",")+1);
	}
	if(token[5].length()>3){token[5] = token[5].substr(0,3);}
	month = stoi(token[0]);
	day= stoi(token[1]);
	hour= stoi(token[2]);
	minute= stoi(token[3]);
	second= stoi(token[4]);
	fractionalSecond = stof(token[5]);
}

void MergeTime::timeToDouble()
{
	time =0;
	
	time=(double) fractionalSecond;
	time+=(double) second;
	time+= (double) minute *60;
	time += (double) hour *3600;
}


double MergeTime::getTime(){return time;}

