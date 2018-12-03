#include "LMS511Data.h"

LMS511Scan::LMS511Scan(){}

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
	scanFreq = hex2Dec(scan.substr(99,3));
	measurementFreq = hex2Dec(scan.substr(103,3));
	scaleFactor = hex2Dec(scan.substr(121,8));
	scaleOffset = hex2Dec(scan.substr(130,8));
	startAngle = hex2Dec(scan.substr(139,8));
	angularStep = hex2Dec(scan.substr(148,3));
	measurementNumb = hex2Dec(scan.substr(152,3));

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






