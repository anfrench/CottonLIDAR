#define TOLERANCE .013
#define MAXZ 2.0
#define MINZ 0.03
#define MAXVOLUME 100.0
#define MAXASPECT 20.0

#include "pclLib/pclCluster.h"
#include "pclLib/Functions.h"
#include "readline/readline.h"
#include "Color.h"


using namespace std;

string getFileName();

int main()
{
	float clusterColor, cloudColor;
	int rgb;
	//calculating color values
	rgb = (cloudR<<16)|(cloudG<<8)|cloudB;
	cloudColor=(float)rgb;

	rgb = (clusterR<<16)|(clusterG<<8)|clusterB;
	clusterColor=(float)rgb;

	int size;
	pcl::PointCloud<pcl::PointXYZ>::Ptr centerPoints (new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr centerColored (new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloudColored (new pcl::PointCloud<pcl::PointXYZRGB>);
	pclCluster *cloud= new pclCluster();
	vector<pclCluster> clusters;

	//opening new cloud
	cout<<"What cloud would you like to calculate"<<endl;
	cloud->open(getFileName());
	//moving cloud to y axsis
	cloud->translateCenter(0,0,cloud->center.z);
	cloud->translate('y', 0);
	
	//croping and resizing new cloud
	cloud->crop("z",MAXZ,MINZ);
	cloud->resize(2,12,0);
	//cloud->removeOutliers( 100,1.0);

	//capturing the cloud before clustering dounsamples it
	cloudColored=addColor(cloud->cloud,cloudColor);
	//clustring cloud
	clusters = cloud->EuclideanCluster(5,300000,TOLERANCE);
	//filtering cloud
	//clusters=volumeFilter(0,MAXVOLUME,clusters);
	//clusters = aspectRatioFilter(MAXASPECT, clusters);
	
	//making cloud of center points
	//centerPoints=getCenterPointCloud(clusters);
	cloud->cloud=combineVectorCloud(clusters).cloud;
	cloud->translate('x', 2);	
	
	//adding color
	centerColored=addColor(cloud->cloud,clusterColor);
	centerColored->width = centerColored->size();
	centerColored->height =1;
	centerColored->is_dense=true;
	size =	cloudColored->size();
	



	//combining clouds

	cloudColored->resize(size+centerColored->size());
	for(size_t i=0; i<centerColored->size(); i++)
	{
		cloudColored->points[size+i]=centerColored->points[i];
	}

	
	cloudColored->width = cloudColored->size();
	cloudColored->height =1;
	cloudColored->is_dense=true;
	//saving cloud
	cout<<"What would you like to save as?"<<endl;
	pcl::io::savePCDFileASCII(getFileName(), *cloudColored);
	
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







