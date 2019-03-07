#define CROPZMAX 10
#define CROPZMIN 2.1
#define INPUTFILE "fileNames.txt"

#include <iostream>
#include <string>
#include <vector>
#include "pclLib/pclCluster.h"
#include "pclLib/Functions.h"

using namespace std;

int main()
{
	float cropMax=10,cropMin=-10;
	string outputFile="TotalField.PCD";
	//Vector of point clouds
	vector<pclCluster> cloudSum;
	int counter=0;
	//setting crop values and output
	//cout<<"Enter Maximum Height: ";
	//cin>>cropMax;
	//cout<<"Enter Minimum Height: ";
	//cin>>cropMin;
	//cout<<"Enter Output File Name: \n";
	//getline(cin, outputFile);
	
	cropMax = 10;
	cropMin = 0.27;

	//File where the filenames are kept
	fstream names;
	names.open(INPUTFILE, fstream::in);
	string fileName;

	while(getline(names, fileName))
	{
		//Initalizing and opening new cloud
		pclCluster *cluster= new pclCluster();
		cluster->open(fileName);
		//making sure that the cloud is at z=0
		//cluster->translateZ(0);

		//croping out ground
		//cluster->crop("z",cropMax,cropMin);
		
		//puting cloud in vector
		cloudSum.push_back(*cluster);
		cout<<counter ++<<endl;
	}
	
	//Using method in "pclLib/functions" to combine all clouds
	pclCluster field = combineVectorCloud(cloudSum);

	//croping out any negitive x and y values
	field.crop("x", 50000, 0);
	field.crop("y", 50000, 0);
	
	//saving
	field.save("TotalField.PCD");
}
