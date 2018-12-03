#pragma once
#include "LMS400DataBase.h"
#include <sstream>
using namespace std;

class LMS511Scan : public LMS400Scan
{
	private :
	vector<int> distValues;
	protected :
	string Serial;
	double scaleFactor, scaleOffset;
	int timeFromStart, timeOfTransmision;
	int scanFreq, measurementFreq;

	int string2Int(string toConvert);
	virtual void partition();
	virtual void decodeData();
	virtual void decodeInfoBlocks();
	public :
	LMS511Scan();
	virtual string toString();
	int getTransmitionTime();
	
	
	
};



/*
	comand	version#	devise#		Serial#		stat1	stat2	TeleG#	Scan #	timefromstart	timeof transmision
sSI 	47 	0 		1 		ABF881 		0 	0 	2C25 	2C2C 	B8FF5777 	B8FFFA64 		


stat?	stat?	output	resurved?	reserved?	scanFrequency	measurementfrequency		content	scaleFactor	scaleoffset	
0 	0 	3F 	0 		0 		9C4 		21C 			1 0 0 1 DIST1 	3F800000 	00000000 	

starting angle	steps	amout of data	
FFFF3CB0 	683 	475 		


// all garbage.....
0 0 1 B not defined 0 1 7B2 1 1 0 33 2B 79568 0<03>
*/
