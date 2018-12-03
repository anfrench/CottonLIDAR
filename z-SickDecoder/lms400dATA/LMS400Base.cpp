#include "LMS400DataBase.h"

//class LMS400Scan
//not defined in base class
void LMS400Scan::decodeData(){}
vector<int> LMS400Scan::getRemisValues(){vector<int> vec; return vec;}
vector<int> LMS400Scan::getDistValues()	{vector<int> vec; return vec;}
void LMS400Scan::partition(){}

//defined in base class
//Constructors
LMS400Scan::LMS400Scan(string scanIN) : LMS400Scan()
{
	scan=scanIN;
}

LMS400Scan::LMS400Scan()
{
	comandAns 		= 0; 
	distResolution	= 0;
	startAngle 		= 0;
	angularStep 	= 0;
	measurementNumb = 0;
	scanRate 		= 0;
	reflectivity 	= 0;
	startReflectivity = 0;
	stopReflectivity  = 0;
	
	//Status Block Values
	digitalInputs		= 0;
	reserved1			= 0;
	reserved2			= 0;
	teacVals			= 0;
	reserved3			= 0;
	reserved4			= 0;
	scanCounter			= 0;
	telegramCounter		= 0;
	systemCounter 		= 0;
}

void LMS400Scan::decodeInfoBlocks()
{
	//ParameterBlock Values
	comandAns 		= hex2Dec(parameterBlock.substr(0,1)); 
	distResolution	= hex2Dec(swap2(parameterBlock.substr(3,4)));
	startAngle 		= hex2Dec(swap4(parameterBlock.substr(7,8)));
	angularStep 	= hex2Dec(swap2(parameterBlock.substr(15,4)));
	measurementNumb = hex2Dec(swap2(parameterBlock.substr(19,4)));
	scanRate 		= hex2Dec(swap2(parameterBlock.substr(23,4)));
	reflectivity 	= hex2Dec(swap2(parameterBlock.substr(27,4)));
	startReflectivity = hex2Dec(swap2(parameterBlock.substr(31,4)));
	stopReflectivity  = hex2Dec(swap2(parameterBlock.substr(35,4)));
	
	//Status Block Values
	digitalInputs		= hex2Dec(swap2(statusBlock.substr(0,4)));
	reserved1			= hex2Dec(swap2(statusBlock.substr(4,4)));
	reserved2			= hex2Dec(swap2(statusBlock.substr(8,4)));
	teacVals			= hex2Dec(swap2(statusBlock.substr(12,4)));
	reserved3			= hex2Dec(swap2(statusBlock.substr(16,4)));
	reserved4			= hex2Dec(swap2(statusBlock.substr(20,4)));
	scanCounter			= hex2Dec(swap2(statusBlock.substr(24,4)));
	telegramCounter		= hex2Dec(swap2(statusBlock.substr(28,4)));
	systemCounter 		= hex2Dec(swap2(statusBlock.substr(32,4)));
}

/*
	@ param 	none
	@ returns 	void
	Calls decodeInfoBlocks
	Calls decodeData
	Both are virtual functions
	and are not defined in the
	base class.
*/
void LMS400Scan::decode()
{
	decodeInfoBlocks();
	decodeData();
}
	
/*
	Sets the scan to the 
	passed in string
*/


/*
	getters / setters
*/
string LMS400Scan::getScan()
{
	return scan;
}

void LMS400Scan::setScan(string scanIn)
{
	scan = scanIn;
	partition();
}

string LMS400Scan::getParam()
{
	return parameterBlock;
}
string LMS400Scan::getData()
{
	return dataBlock;
}
string LMS400Scan::getStats()
{
	return statusBlock;
}

int LMS400Scan::getComand(){return comandAns;}
int LMS400Scan::getDistanceResolution(){return distResolution;}
int LMS400Scan::getStartAngle(){return startAngle;}
int LMS400Scan::getAngularStep(){return angularStep;}
int LMS400Scan::getMeasurementNumb(){return measurementNumb;}
int LMS400Scan::getScanRate(){return scanRate;}
int LMS400Scan::getReflectivity(){return reflectivity;}
int LMS400Scan::getStartReflectivity(){return startReflectivity;}
int LMS400Scan::getStopReflectivity(){return stopReflectivity;}
int LMS400Scan::getDigitalInputs(){return digitalInputs;}
int LMS400Scan::getReserved1(){return reserved1;}
int LMS400Scan::getReserved2(){return reserved2;}
int LMS400Scan::getTeacVals(){return teacVals;}
int LMS400Scan::getReserved3(){return reserved3;}
int LMS400Scan::getReserved4(){return reserved4;}
int LMS400Scan::getScanCounter(){return scanCounter;}
int LMS400Scan::getTelegramCounter(){return telegramCounter;}
int LMS400Scan::getSystemCounter(){return systemCounter;}

/*
	@param 	int start of param block
			int start of data
			int start of status
	@return void
	
	@use	partitions the scan into the paramiter, data, and status block
*/
void LMS400Scan::partition(int start, int startData, int statLength)
{
	parameterBlock 	= scan.substr(start, startData-start);
	dataBlock 	= scan.substr(startData, scan.length()-(startData+statLength));
	statusBlock	= scan.substr(scan.length()-statLength);
}

/*
	@param 		string 
	@return 	string
	@use 		reverse the order of 2 2char pairs
*/
string LMS400Scan::swap2(string toSwap)
{
	return toSwap.substr(2)+toSwap.substr(0,2);	
}

/*
	@param 	string
	@return string
	@use	reverse the order of 4 2 char pairs 
*/
string LMS400Scan::swap4(string toSwap)
{
	return swap2(toSwap.substr(4)) + swap2(toSwap.substr(0,4));	
}

/*
	@param string
	@return int
	@use converst a hext string to an intager
*/
int LMS400Scan::hex2Dec(string stringHex)
{
	int dec;
	stringstream transfer;
	transfer<< std::hex << stringHex;
	transfer>>dec;
	return dec;
}


string LMS400Scan::toString()
{
	return
	"Parameter Block" + parameterBlock + "\n" +
	"Status Block" + statusBlock + "\n"+
	"Comand : "+ to_string(comandAns) + "\n" + 
	"DistanceResolution : "+ to_string(distResolution) + "\n" + 
	"StartAngle : "+ to_string(startAngle) + "\n" + 
	"AngularStep : "+ to_string(angularStep) + "\n" + 
	"MeasurementNumb: "+ to_string(measurementNumb) + "\n" + 
	"ScanRate : "+ to_string(scanRate) + "\n" + 
	"Reflectivity : "+ to_string(reflectivity) + "\n" + 
	"StartReflectivity : "+ to_string(startReflectivity) + "\n" + 
	"StopReflectivity : "+ to_string(stopReflectivity) + "\n" + 
	"DigitalInputs : "+ to_string(digitalInputs) + "\n" + 
	"Reserved1 : "+ to_string(reserved1) + "\n" + 
	"Reserved2 : "+ to_string(reserved2) + "\n" + 
	"Reserved3 : "+ to_string(reserved3) + "\n" + 
	"Reserved4 : "+ to_string(reserved4) + "\n" + 
	"TeacVals: "+ to_string(teacVals) + "\n" + 
	"ScanCounter : "+ to_string(scanCounter) + "\n" + 
	"TelegramCounter : "+ to_string(telegramCounter) + "\n" + 
	"SystemCounter : "+ to_string(systemCounter)+"\n";
}



