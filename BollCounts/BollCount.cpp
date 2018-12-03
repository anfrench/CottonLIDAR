#include "Definitions.h"
#include "pclLib/pclCluster.h"
#include "pclLib/Functions.h"
#include <bits/stdc++.h>
#include <sstream>
#include <fstream>

using namespace std;

string fileName;
void printTime(int seconds);
void writeToFile(string ID, double northAdjust, double eastAdjust , vector<pclCluster> cloud);
void writeHeader(FILE *file, string ID ,vector<pclCluster> cloud, double xAdjust, double yAdjust);

vector<pclCluster> cloudUpper;
vector<pclCluster> cloudLower;

int main()
{
	//variables used for timeStamps
	time_t timeTotal, timePer, end;
	timeTotal=time(NULL);
	int seconds;
	
	float maxZ, minZ;
	int minClusterSize, maxClusterSize;
	float tolerance;
	
	cout<<"Enter Maximum Z: ";
	cin>> maxZ;
	cout<<"Enter Minimum Z: ";
	cin>> minZ;
	cout<<"Enter Maximum cluster points: ";
	cin>> maxClusterSize;
	cout<<"Enter Minimum cluster points: ";
	cin>> minClusterSize;
	cout<<"Enter Tolerance: ";
	cin>> tolerance;


	//File where the filenames are kept
	fstream names;
	names.open(INPUTFILE, fstream::in);
	while(getline(names, fileName))
	{
		timePer=time(NULL);
		pclCluster *cluster= new pclCluster();
		cout<<"Testing: "<<fileName<<endl;
		try
		{
			//opening new cluser
			cluster->open(fileName);
			cloudUpper.push_back(*cluster);
			
			//used to find offset 
			double offsetX= cloudUpper[0].center.x;
			double offsetY= cloudUpper[0].center.y;
			
			//moving to the x axsis
			cloudUpper[0].translateCenter(0,0,cloudUpper[0].center.z);
			cloudUpper[0].translate('y', 0);
			
			//finding total x/y offsets
			offsetX = offsetX -cloudUpper[0].center.x;
			offsetY = offsetY -cloudUpper[0].center.y;

			//downsampling
			//cloudUpper[0].removeOutliers(POINTS, STDMUL);
			//cloudUpper[0].cloud=donFilter(SHORT, LARGE, MAG, cloudUpper[0].cloud);
			
			//croping z to remove ground and upper noise 
			cloudUpper = cropAll("z",maxZ,minZ,cloudUpper);

			//seporating upper and lower plots
			cloudLower= cropAll("x",-XINNER,-XOUTER,cloudUpper);
			cloudUpper= cropAll("x",XOUTER,XINNER,cloudUpper);
			
			//clustring 
			cloudUpper= cloudUpper[0].EuclideanCluster(minClusterSize,maxClusterSize,tolerance);
			cloudLower= cloudLower[0].EuclideanCluster(minClusterSize,maxClusterSize,tolerance);
			
			//filtering
			//cloudUpper=volumeFilter(MINVOLUME,MAXVOLUME, cloudUpper);
			//cloudLower=volumeFilter(MINVOLUME,MAXVOLUME, cloudLower);
			//cloudUpper=aspectRatioFilter(DIMENSIONS, MAXASPECT, cloudUpper);
			//cloudLower=aspectRatioFilter(DIMENSIONS, MAXASPECT, cloudLower);
			
			//getting the plot ID's 
			string lowerID = fileName.substr(27,3);
			string upperID = fileName.substr(31,3);
			
			//writing to files
			writeToFile(upperID, offsetY+NORTHING, offsetX+EASTING , cloudUpper);
			writeToFile(lowerID, offsetY+NORTHING, offsetX+EASTING , cloudLower);
			
			//clearing all clusters
			cloudUpper.clear();
			cloudLower.clear();
			cout<<"sucsess! ";
			end=time(NULL);
			seconds = difftime(end, timePer);
			printTime(seconds);
		}
		catch(...){cout<<"ERROR: "<<fileName<<endl;}
	}
	
	seconds = difftime(end, timeTotal);
	cout<<"Total ";
	printTime(seconds);
		
}


void writeHeader(FILE *file, string ID ,vector<pclCluster> cloud, double xAdjust, double yAdjust)
{

	
	double mean, stdDiv, meanY, meanZ, stdDivY, stdDivZ;	
	int dataType;
	
	fprintf(file,"AttributeID,Value,Units,,, \n"		);
	fprintf(file,"PlotNumber,%s,[],,, \n" , 		ID.c_str());
	fprintf(file,"ClusteringTolerance,%f,m,,, \n" , 	TOLERANCE );
	fprintf(file,"ClusteringMaximumZ,%f,m,,, \n" ,		MAXZ );
	fprintf(file,"ClusteringMinimumZ,%f,m,,, \n" ,		MINZ);
	fprintf(file,"MaximumClusterVolume,%f,cm^3,,, \n" ,	MAXVOLUME );
	fprintf(file,"MaxAspectRatio,%f,[],,, \n" , 		MAXASPECT );
	fprintf(file,"NumberOfClusters,%d,[],,, \n",		cloud.size() );
	
	dataType=2;
	findStandardDiv(&mean, &stdDiv, dataType, cloud);
	
	fprintf(file,"VolumeMean,%f,cm^3,,, \n" ,		mean);
	fprintf(file,"VolumeStandardDeviation,%f,cm^3,,, \n" ,	stdDiv);
	
	dataType=1;
	findStandardDiv(&mean, &stdDiv, dataType, cloud);
	
	fprintf(file,"MeanPointsPerCluster,%f,[],,, \n" ,		mean);
	fprintf(file,"StandardDeviation_PointsPerCluster,%f,[],,, \n",stdDiv );
	
	dataType=5;
	findStandardDiv(&mean, &stdDiv, dataType, cloud);
	dataType = 4;
	findStandardDiv(&meanY, &stdDivY, dataType, cloud);
	dataType=3;
	findStandardDiv(&meanZ, &stdDivZ, dataType, cloud);
	mean=mean + xAdjust;
	meanY=meanY + yAdjust;
	
	fprintf(file,"MeanXLocation,%f,m,,, \n" , 		mean );
	fprintf(file,"MeanYLocation,%f,m,,, \n" ,		meanY);
	fprintf(file,"MeanZLocation,%f,m,,, \n" ,		meanZ);
	fprintf(file,"StandardDeviationXLocation,%f,m,,, \n" ,stdDiv );
	fprintf(file,"StandardDeviationYLocation,%f,m,,, \n" ,stdDivY);
	fprintf(file,"StandardDeviationZLocation,%f,m,,, \n" ,stdDivZ);
	fprintf(file,"Cluster_ID,Volume,PointsPerCluster,UTM12NorthingY,UTM12EastingX,HeightZ \n" );
}



void writeToFile(string ID, double northAdjust, double eastAdjust , vector<pclCluster> cloud)
{
	
	double northing, easting;
	FILE * newFile;
	string outFile= "Cluster_Count_Plot_"+ID+".csv"; //todoFinishFileName
	newFile = fopen((OUTPUTDIR+outFile).c_str(),"w");
	writeHeader(newFile,ID,cloud,eastAdjust, northAdjust);
	

	for(int i=0; i<cloud.size(); i++)
	{
		//Cluster_ID,Volume,PointsPerCluster,UTM12NorthingY,UTM12EastingX,HeightZ
		fprintf(
			newFile, "%i,%f,%f,%f,%f,%f\n",
			i,cloud[i].getData(2),cloud[i].getData(1),
			cloud[i].getData(4)+northAdjust,
			cloud[i].getData(5)+eastAdjust,
			cloud[i].getData(3)
			);
	}
			
			
	//closing files
	fclose(newFile);
}


void printTime(int seconds)
{
	int hours, minuts=0;
	minuts = seconds/60;
	seconds = seconds %60;
	hours = minuts /60;
	minuts = minuts%60;
	cout<<"Time "<<hours<<" : "<<minuts <<" : " << seconds<<endl;
	
}













