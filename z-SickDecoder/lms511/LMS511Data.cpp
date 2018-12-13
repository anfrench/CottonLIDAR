#include "LMS511Data.h"
#include<iostream>
LMS511Scan::LMS511Scan(){}

double LMS511Scan::getScaler(){return scaleFactor;}

void LMS511Scan::partition()
{
	LMS400Scan::partition(0,scan.find("DIST")+41,51);
}

void LMS511Scan::decodeInfoBlocks()
{
	
	comandAns = string2Int(scan.substr(42,2));
	
	Serial = scan.substr(49,6);
	
	telegramCounter = hex2Dec(scan.substr(60,4));
	scanCounter =  hex2Dec(scan.substr(65,4));
	timeFromStart = hex2Dec(scan.substr(70,8));
	timeOfTransmision = hex2Dec(scan.substr(79,8));
	scanFreq = hex2Dec(scan.substr(99,4));
	measurementFreq = hex2Dec(scan.substr(104,3));
	scaleFactor = hex2Flo(scan.substr(118,8));
	scaleOffset = hex2Flo(scan.substr(127,8));
	startAngle = hex2Dec(scan.substr(136,8))/10000.0;
	angularStep = hex2Dec(scan.substr(145,4))/10000.0;
	measurementNumb = hex2Dec(scan.substr(150,3));


	/*
	std::cout<<scan.substr(60,100);
	std::cout<<std::endl;
	std::cout<<"telegramCounter"<<scan.substr(60,4);
	std::cout<<std::endl;
	std::cout<<"scanCounter ="<<  scan.substr(65,4);
	std::cout<<std::endl;
	std::cout<<"timeFromStart ="<< scan.substr(70,8);
	std::cout<<std::endl;
	std::cout<<"timeOfTransmision="<< scan.substr(79,8);
	std::cout<<std::endl;
	std::cout<<"scanFreq ="<< scan.substr(99,4);
	std::cout<<std::endl;
	std::cout<<"measurementFreq ="<<scan.substr(104,3);
	std::cout<<std::endl;
	std::cout<<"scaleFactor ="<< scan.substr(118,8);
	std::cout<<std::endl;
	std::cout<<"scaleOffset ="<< scan.substr(127,8);
	std::cout<<std::endl;
	std::cout<<"startAngle ="<< scan.substr(136,8);
	std::cout<<std::endl;
	std::cout<<"angularStep ="<< scan.substr(145,4);
	std::cout<<std::endl;
	std::cout<<"measurementNumb ="<<scan.substr(150,3);
	std::cout<<std::endl;
	std::cout<<std::endl;
	std::cout<<std::endl;
	*/
	
}

string LMS511Scan::toString()
{

	string out = "";
	out += "ComandAnswer: '"+ to_string(comandAns)+ "'\n";
	out += "SerialNumber: '"+ Serial+ "'\n";
	out += "TelegramCounter : '" + to_string(telegramCounter) + "'\n";
	out += "ScanCounter : '" + to_string(scanCounter) + "'\n";
	out += "TimeFromStart : '" + to_string(timeFromStart)+ "'\n";
	out += "TimeOfTransmision : '" + to_string(timeOfTransmision)+ "'\n";
	out += "ScanFrequency : '" + to_string(scanFreq)+ "'\n";
	out += "MeasurmentFrequency : '" + to_string(measurementFreq)+ "'\n";
	out += "ScaleFactor : '" + to_string(scaleFactor)+ "'\n";
	out += "ScaleOffset : '" + to_string(scaleOffset)+ "'\n";
	out += "StartAngle : '" + to_string(startAngle)+ "'\n";
	out += "AngularStep : '" + to_string(angularStep)+ "'\n";
	out += "NumberOfMeasurements : '" + to_string(measurementNumb)+ "'\n";
	return out;
}


int LMS511Scan::string2Int(string toConvert)
{
	int out;
	stringstream ss;
	ss << toConvert;
	ss >> out;
	return out;
}

void LMS511Scan::decodeData()
{
	distValues.clear();
	string temp = dataBlock, token;
	while(temp.size()>=4)
	{
		token = temp.substr(0,3);
		temp=temp.substr(4);
		distValues.push_back(hex2Dec(token));
	}
}


int LMS511Scan::getTransmitionTime()
{
	return timeOfTransmision;
}

vector<int> LMS511Scan::getDistValues()
{
	return distValues;
}





