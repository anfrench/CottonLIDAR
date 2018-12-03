#include "pclLib/pclCluster.h"
#include "pclLib/Functions.h"
#include "readline/readline.h"
#include "pclLib/convexHull.h"
#include "pclLib/Canopy.h"

using namespace std;

/*
	all menues print out a list of options
	ask the user for input
	and using a switch makes the a function call
	if the function nees further input
	the user will be prompted within the switch's case statment
*/
void menu();
void dataMenu();
void clusterMenu();
void filterMenu();
void editMenu();
void viewMenu();
void tempMenu();
void downSampleMenu();

//
void viewHistogram();


/*depends on the getLine lib
asks for a filename and returns user input
uses auto completion
*/
string getFileName();

/*
	asks user for input.
	using a bitwise and casts the input to an int.
	returns an int. max four bits long
*/
int getInput();

//Displays max/min/avg and asks if you want to view a histogram.
void displayAvg();


//main vector that is oporated on
vector<pclCluster> cloud;
//a vector to be used as a back up
vector<pclCluster> tempCloud;
//a vector where every even number is some data and every odd is it's frequency
vector<int> histogram;
// as long as true the program wil loop
bool loop;

/*
	used by the data section.
*/
double dataMax, dataMin;
double dataAvg;
#include <math.h>


///////////////////////////////////main
int main()
{	
	loop=true;

	while(loop)
	{
		menu();
	}
}
///////////////////////////////////end: main


/*
	Main Menu. Mostly Calls other menues. However also opens a file/ clears the main cloud and exits the program.
*/
void menu()
{
	PointCanopy canopy;
	pclCluster *cluster= new pclCluster();
	cout<<"-----Main Menu-----"<<endl;
	cout<< " 1) Data"<<endl;
	cout<< " 2) Filter"<<endl;
	cout<< " 3) Edit Cloud"<<endl;
	cout<< " 4) Downsample"<<endl;
	cout<< " 5) Cluster"<<endl;
	cout<< " 6) View"<<endl;
	cout<< " 7) Use Temp"<<endl;
	cout<< " 8) Open Cloud"<<endl;
	cout<< " 9) Clear Cloud"<<endl;
	cout<< "10) save Cloud"<<endl;
	cout<< "11) Quit"<<endl;

	switch(getInput())
	{
	
		case 1:
			dataMenu();
			break;
		
		case 2:
			filterMenu();
			break;
			
		case 3: 
			editMenu();
			break;
		case 4:
			downSampleMenu();
			break;
		case 5:
			clusterMenu();			
			break;
		
		case 6:
			viewMenu();
			break;
		case 7:
			tempMenu();
			break;
		case 8:
			//open cloud
			cluster->open(getFileName());
			cloud.push_back(*cluster);
			
			break;
		case 9:
			// clear cloud
			cloud.clear();
			break;
		case 10:
			//save Cloud
			*cluster=combineVectorCloud(cloud);
			cluster->save(getFileName());
			break;
		case 11:
			cout<<"BYE"<<endl;
			//exit program
			loop = false;
			break;
		default:
			cout<<"Choice not valid."<<endl;
		
	}
	cout<<endl;
}

/*
	finds the min/max/average of an aspect of the clusters.
	also creats a histogram of that aspect.
*/
void dataMenu()
{	
	cout<<"-----Data Menu-----"<<endl;
	cout<< "1) Cloud Ranges cloud[0]"<<endl;
	cout<< "2) Cluster points data"<<endl;
	cout<< "3) Cluster volume data"<<endl;
	cout<< "4) Cluster Height"<<endl;

	switch(getInput())
	{
		case 1:
			cloud[0].findSize();
			cout<<"X: "<< cloud[0].minX << " - " <<cloud[0].maxX << "\tTotal Width : "<<cloud[0].width <<endl;
			cout<<"Y: "<< cloud[0].minY << " - " <<cloud[0].maxY << "\tTotal Length: "<<cloud[0].length <<endl;
			cout<<"Z: "<< cloud[0].minZ << " - " <<cloud[0].maxZ << "\tTotal Height: "<<cloud[0].height <<endl;
			break;
		case 2:
			cout<<"Number of Points-"<<endl;
			getAverage(&dataMax, &dataMin, &dataAvg,cloud,1);
			histogram=createHistogram(1, cloud);
			displayAvg();
			break;
		case 3:
			cout<<"Volume-"<<endl;
			getAverage(&dataMax, &dataMin, &dataAvg,cloud,2);
			histogram=createHistogram(2,cloud);
			displayAvg();
			break;
		case 4:
			cout<<"Cluster Height-"<<endl;
			getAverage(&dataMax, &dataMin, &dataAvg,cloud,3);
			histogram=createHistogram(3,cloud);
			displayAvg();
			break;

	}


}


/*
	clusters the first element in the main cloud.
	gives options of how you'd like to cluster.
*/
void clusterMenu()
{
	vector<pclCluster> temp;
	cout<<"-----Cluster Menu-----"<<endl;
	cout<< "1) Euclidean Cluster"<<endl;
	double min,max,tol;
	switch(getInput())
	{
		case 1:
			cout<<"Enter Min cluster size: ";
			cin>>min;
			cout<<"Enter Max Cluster size: ";
			cin>>max;
			cout<<"Enter cluster Tolerence: ";
			cin>>tol;
			temp = cloud[0].EuclideanCluster(min,max,tol);
			cloud.clear();
			cloud=temp;
			break;
		default:
			cout<<"No changes made."<<endl;
	}
	cout<<endl;
}

/*
	gives several ways to filter the clusters.
*/
void filterMenu()
{
	vector<pclCluster> temp;
	cout<< "-----Filter Menu-----"<<endl;
	cout<< "1) Remove outliers \t(cluster[0] only)"<<endl;
	cout<< "2) Aspect Ratio Filter"<<endl;
	cout<< "3) Volume Filter"<<endl;
	cout<< "4) Crop"<<endl;

	double max, min;
	string dim;
	switch(getInput())
	{
		case 1:
			cout<< "Enter points to consider: ";
			cin>>max;
			cout<<"Enter standard deveation mul: ";
			cin>>min;
			cloud[0].removeOutliers((int)max, min);
			break;
		case 2:
			cout<<"Enter the dimensions you'd like to filter(x,y,z)";
			cin>>dim;
			cout<<"Enter the max Aspect Ratio: ";
			cin>>max;
			temp=aspectRatioFilter(dim, max, cloud);
			cloud.clear();
			cloud=temp;
			break;
			
		case 3:
			cout<< "Enter Max Volume: ";
			cin>>max;
			cout<<"Enter Min Volume: ";
			cin>>min;
			temp = volumeFilter(min,max,cloud);
			cloud.clear();
			cloud=temp;
			cout<<cloud.size();
			break;
		
		case 4:
			cout<<"Enter Dimention (x/y/z): ";
			cin>>dim;
			cout<<"Enter Max "<<dim<<": ";
			cin>>max;
			cout<<"Enter Min "<<dim<<": ";
			cin>>min;
			temp=cropAll(dim, max, min, cloud);
			cloud.clear();
			cloud=temp;
			break;
			
		
	}
	cout<<endl;

}

/*
	Gives the difrent options of viewing the clusters.
*/

void viewMenu()
{
	cout<<"-----View Menu-----"<<endl;
	cout<< "1) View By Size"<<endl;
	cout<< "2) View By Cluster"<<endl;
	
	switch(getInput())
	{
		case 1:
			int min, max;
			cout<<"Min Cluster Size: ";
			cin>>min;
			cout<< "Max Cluster Size: ";
			cin>>max;
			viewBySize(min, max, cloud);
			break;
		case 2:
			viewByCluster(cloud);
			break;
		default:
			cout<<"No Changes Made."<<endl;
		
	}
	cout<<endl;
}

/*
	used for making a temp backup of the current cloud
*/
void tempMenu()
{
	cout<<"------Temp Menu------"<<endl;
	cout<<"1) Save current cloud to temp"<<endl;
	cout<<"2) Load temp to current cloud"<<endl;
	cout<<"3) Clear Temp"<<endl;
	cout<<""<<endl;
	
	switch(getInput())
	{
		case 1:
			tempCloud.clear();
			tempCloud=cloud;
			break;
		case 2:
			cloud.clear();
			cloud=tempCloud;
			break;
		case 3:
			tempCloud.clear();
			break;
		default:
			cout<<"No Changes Made."<<endl;
	}
	
	cout<<endl;
}

/*
	propts user for input
	casts input to a four bit int
	returns an int
*/
int getInput()
{

	int control;
	cin>>control;
	cin.clear();
	cin.ignore();	
	
	return control;	
}


/*
	prints the histogram vector
	even numbers on the left and odds on the right.
*/
void viewHistogram()
{
	cout<<"Test test";
	
	for(int i=1; i<histogram.size(); i+=2 )
	{
		cout<<histogram[i-1]<<" , "<<histogram[i]<<endl;
	}
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

void displayAvg()
{
	cout<<"Max: "<<dataMax<<endl;
	cout<<"Min: "<<dataMin<<endl;
	cout<<"Average: "<<dataAvg<<endl;
	cout<<"Number of Clusters: "<< cloud.size() <<endl;
	cout<<"viewHistogram? 1/0"<<endl;
	if(getInput()==1)
	{
		viewHistogram();
	}
	cout<<endl;
}



void editMenu()
{	
	double x, y, z;
	char dim;
	pclCluster temp;
	
	cout<<"-----Edit Menu------"<<endl;
	cout<< "1) Resize \t(cluster[0] only)"<<endl;
	cout<< "2) Translate dim \t(cluster[0] only)"<<endl;
	cout<< "3) Translate Center \t(cluster[0] only)"<<endl;
	cout<< "4) Combine all clusters into one cloud"<<endl;
	cout<< "5) Get Center Point Cloud"<<endl;

	switch(getInput())
	{


		case 1:
			cout<<"Enter X's length (0 for no change):";
			cin >> x;
			cout<<"Enter Y's length (0 for no change):";
			cin >> y;
			cout<<"Enter Z's length (0 for no change):";
			cin >> z;
			cloud[0].resize(x,y,z);
			break;
		case 2:
			cout<<"Enter Dimention (x/y/z): ";
			cin>>dim;
			cout<<"Enter "<<dim<<"'s new location:"<<endl;
			cin>> x;
			cloud[0].translate(dim,x);
			break;
		case 3:
			cout<<"Enter X's new center:";
			cin >> x;
			cout<<"Enter Y's new center:";
			cin >> y;
			cout<<"Enter Z's new center:";
			cin >> z;
			cloud[0].translateCenter(x,y,z);
			break;
		case 4:
			temp=getCenterPointCloud(cloud); 
			cloud.clear();
			cloud.push_back(temp);
			break;
		case 5:
			temp=combineVectorCloud(cloud); 
			cloud.clear();
			cloud.push_back(temp);
			break;
		default:
			cout<<"No Changes Made..."<<endl;
	}
}

void downSampleMenu()
{
	double x,y,z;
	cout<<"-----Downsample Menu------"<<endl;
	cout<< "1) Voxel \t(cluster[0] only)"<<endl;
	cout<< "2) Difrence of Normals \t(cluster[0] only)"<<endl;
	cout<< "3) planar estimation \t(cluster[0] only)"<<endl;
	switch(getInput())
	{
		case 1:
			cout<<"Enter leafSize";
			cin>>x;
			cloud[0].cloud=downSample(x, cloud[0].cloud);
			break;
		case 2:
			cout<<"Enter Short Normal search Radious: ";
			cin >> x;
			cout<<"Enter Large Normal search Radious: ";
			cin >> y;
			cout<<"Enter Normal Magnitude threshold: ";
			cin >> z;
			cloud[0].cloud = donFilter(x,y,z,cloud[0].cloud) ;
			break;
		case 3:
			cout<<"planar estimation not ready";
			cout<<endl;
			cout<<"No changes made"<<endl;
			break;
			
	}
}




