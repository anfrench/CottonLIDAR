
#define PI 3.14159265
#include "pclLib/pclCluster.h"
#include "pclLib/Functions.h"
#include "pclLib/pclMaker.h"
#include "readline/readline.h"
#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace std;

string getFileName();

int main()
{
	pclCluster cluster;
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
	
	string input="";
	
	vector<double> bounds;
	
	
	while(input.find("#")==string::npos)
	{
		getline(cin,input);
		// need to push back something....double temp=atod
	}
	
	
	for(int i=0; i<=150; i+=10)
	{
		bounds.push_back(i);
	}
	
	cloud=makeStepPlane(bounds,5);

	pcl::io::savePCDFileASCII("field.pcd", *cloud);
	
	return 0;	
}



