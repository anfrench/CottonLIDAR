#include <string>
#include <iostream>
#include <vector>
#include "TurnTable.h"
#include "readline/readline.h"

using namespace std;

void writeToFile(vector<Point> data, string fileName);
bool intensity;
int main()
{
	string fileNameIn, fileNameOut;
	double distToLidarIn, heightIn, scaleIn;
	double skewIn, virticalAngleIn;
	int stepsPerRevIn, scansPerStepIn;
	vector<Point> scene;
	TurnTable table;
	intensity = false;
	cout<<"Sick LMS 400 Distance Only TurnTable To PCD\n\n";
	cout<<"Enter Name of file to be decoded.";
	cin>>fileNameIn;
	cout<<"Enter Name of file to be saved.";
	cin>>fileNameOut;
	cout<<"Distance to the lidar?";
	cin>>distToLidarIn;
	cout<<"Height offset?";
	cin>>heightIn;
	cout<<"Scale?";
	cin>>scaleIn;
	cout<<"Skew?";
	cin>>skewIn;
	cout<<"Virtical Angle?";
	cin>>virticalAngleIn;
	cout<<"Steps Per Rev?";
	cin>>stepsPerRevIn;
	cout<<"Scans Per Step?";
	cin>> scansPerStepIn;
	
	table.setFileName(fileNameIn);
	table.setSpatialParams(distToLidarIn, heightIn, scaleIn);
	table.setAngles(skewIn, virticalAngleIn);
	table.setStepperParam(stepsPerRevIn, scansPerStepIn);
	table.turnTable();
	
	scene = table.getCloud();
	intensity=table.hasRemissions();

	writeToFile(scene , fileNameOut);

cout<<endl<<endl<<endl;
}



void writeToFile(vector<Point> data, string fileName)
{
	ofstream outFile;
	outFile.open (fileName.c_str());
	outFile<<"# .PCD v0.7 - Point Cloud Data file format\n";
	outFile<<"VERSION 0.7\n";
	outFile<<"FIELDS x y z"; 	if(intensity){outFile<<" intensity";} outFile<<"\n";
	outFile<<"SIZE 4 4 4"; 		if(intensity){outFile<<" 4";} outFile<<"\n";
	outFile<<"TYPE F F F"; 		if(intensity){outFile<<" F";} outFile<<"\n";
	outFile<<"COUNT 1 1 1";		if(intensity){outFile<<" 1";} outFile<<"\n";
	outFile<<"WIDTH "<<data.size()<<endl;
	outFile<<"HEIGHT 1\n";
	outFile<<"VIEWPOINT 0 0 0 1 0 0 0\n";
	outFile<<"POINTS "<<data.size()<<endl;
	outFile<<"DATA ascii\n";


	for(int i=0; i<data.size(); i++)
	{
		outFile <<data[i].x<<" ";
		outFile <<data[i].y<<" ";
		outFile <<data[i].z<<" ";
		if(intensity){outFile<<data[i].remission<<" ";}
		outFile<<endl;
	}
			
			
	//closing files
	outFile.close();
	
}
























