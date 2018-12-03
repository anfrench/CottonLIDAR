
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
	
	int pointNumb=50;
	float diam=.5, length = 1, ringDen=7;
	
	cout<<"Number of points:  ";
	cin>>pointNumb;
	cout<<"diamiter:  ";
	cin>>diam;
	cout<<"length:  ";
	cin>>length;
	cout<<"ring Dencity:  ";
	cin>>ringDen;
	
	//cloud = makeBranch(diam,length,ringDen ,pointNumb);
	cloud = makeCircle(.3, 100);

	for(int i=0; i<100; i++){cloud->points[i].z += 2;}
	
	cluster.cloud = cloud;
	cluster.findSize();
	cout<<"Points"<<cluster.cloud->points.size() <<endl;
	cout<<"Max x :"<<cluster.maxX <<endl;	
	cout<<"Max y :"<<cluster.maxY <<endl;
	cout<<"Max z :"<<cluster.maxZ <<endl;
	cout<<"Min x :"<<cluster.minX <<endl;
	cout<<"Min y :"<<cluster.minY <<endl;
	cout<<"Min z :"<<cluster.minZ <<endl;
	
	cout<<"Width :"<<cluster.width <<endl;
	cout<<"Height:"<<cluster.height <<endl;
	cout<<"length:"<<cluster.length <<endl;
	pcl::io::savePCDFileASCII("Branch.pcd", *cloud);
	
	return 0;	
}




