#define TOLERANCE .015
#define MAXZ 2.0
#define MINZ 0.03
#define MAXVOLUME 100.0
#define MAXASPECT 20.0

#include "pclLib/pclCluster.h"
#include "pclLib/Functions.h"
#include "pclLib/pclMaker.h"
#include "readline/readline.h"
#include "pclLib/colors.h"


using namespace std;

string getFileName();

int main()
{
	int maxPoints, minPoints;
	float tolerance;
	
	cout<<"Enter Maximum Points: ";	
	cin>> maxpoints;
	cout<<"Enter Minimum Points: ";	
	cin>> minPoints;
	cout<<"Enter Clustering Tolerance: ";	
	cin>> tolerance;

	pointColors *color = new pointColors();
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr colored (new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr temp (new pcl::PointCloud<pcl::PointXYZRGB>);
	std::vector<pclCluster> clusters;
	pclCluster *cloud= new pclCluster();
	//opening new cloud
	cout<<"What cloud would you like to calculate"<<endl;
	cloud->open(getFileName());
	
	//moving cloud to y axsis
	//cloud->translateCenter(0,0,cloud->center.z);
	//cloud->translate('y', 0);
	
	//croping and resizing new cloud
	cloud->crop("z",MAXZ,MINZ);
	//cloud->crop("y",4,0);
	//cloud->resize(2,12,0);
	cloud->removeOutliers( 100,2.0);
	
	//clustring cloud
	clusters = cloud->EuclideanCluster(minPoints,maxPoints,tolerance);
	
	//turning clusters into circles
	clusters = clustersToCircles(clusters);
	
	for(int i=0; i<clusters.size(); i++)
	{
		float clusterColor = color->getChromaticColor();
		temp =addColor(clusters[i].cloud,clusterColor);
		for(int j=0; j< temp->points.size(); j++)
		{
			colored->points.push_back(temp->points[j]);
		}
	}
	
	colored ->width = colored->points.size();
	colored ->height =1;
	colored ->is_dense=true;

	//cloud->cloud = combineVectorCloud(clusters);
	cout<<"What would you like to save as?"<<endl;
	pcl::io::savePCDFileASCII(getFileName(), *(colored));
	
}


string getFileName()
{
	string fileName;
	cout<<"Enter File Name"<<endl;
	char *buffer = readline("> ");
	if (buffer)
	{
		fileName=buffer;
		free(buffer);
	}
	fileName.erase(remove(fileName.begin(),fileName.end(), ' '), fileName.end());
	return fileName;
}







