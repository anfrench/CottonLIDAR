#include "Definitions.h"
#include "PlotStats.h"
#include "pclLib/pclCluster.h"
#include "pclLib/Functions.h"
#include <bits/stdc++.h>
#include <sstream>
#include <fstream>

using namespace std;
void getBounds(string fileName, int i);
string fileName;
void printTime(int seconds);
void writeToFile(string ID, vector<double> pointHeightMax, vector<double>  pointHeightMin, vector< vector<double> > pointWidthMax, vector< vector<double> > pointWidthMin);
void writeHeader(FILE *file, string ID);

vector<double> smooth(vector<double> data, int buffer); 


struct plotDeviders
{
	float upper;
	float lower;
};

vector<plotDeviders> bounds; 
vector<plotDeviders> IDbounds;

double maxx, minn;


float cropUpper, cropLower;
int layers, plotNumb, pointsOR;
float stdMulOR, startY,endY;


int main()
{
	fstream names;
	names.open(INPUTFILE, fstream::in);
	getline(names, fileName);
	names.close();
	
	cout<<"Enter The number plots per file: ";
	cin>>plotNumb;
	cout<<"Enter The number of layers: ";
	cin>>layers;
	cout<<"Enter The Max Height: ";
	cin>>cropUpper;
	cout<<"Enter The Min Height: ";
	cin>>cropLower;	
	cout<<"Outlyer Removal-Points to consider: ";
	cin>>pointsOR;
	cout<<"Outlyer Removal-Standard deveation multiplyer: ";
	cin>>stdMulOR;
	
	bounds.resize(plotNumb);	
	IDbounds.resize(plotNumb);
	for(int i=0; i<bounds.size(); i++)
	{
		cout<<"Upper bounds for plot ["<< i <<"](From Center of cloud): ";
		cin>>bounds[i].upper;
		cout<<"Lower bounds for plot ["<< i <<"](From Center of cloud) : ";
		cin>>bounds[i].lower;
		getBounds(fileName, i);
	}

	//variables used for timeStamps
	time_t timeTotal, timePer, end;
	timeTotal=time(NULL);
	int seconds;
	
	//File where the filenames are kept
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
			cout<< " after opening   \n";
			//moving to the x axsis
			cluster->translateCenter(0,0,cluster->center.z);
			cluster->translate('y', 0);
			cout<< " after translating   \n";
			//croping z to remove ground and plot noise
			//needs to be an input value 
			cluster->crop("z",cropUpper,cropLower);
			cout<< " after crop   \n";
			for(int i = 0; i<plotNumb; i++)
			{
				PlotStats *stats= new PlotStats();
				stats->setCloud(cluster->cloud);
				stats->setScanValues(STEP, SCOPE, layers);
				stats->setup(bounds[i].upper,bounds[i].lower,pointsOR, stdMulOR);
				cout<< " after croping single   \n";
				stats->findHeight();
				stats->findWidth();
				cout<< " after tests   \n";
				//smoothing height data
				stats->heightMax=smooth(stats->heightMax, BUFFER);	
				stats->heightMin=smooth(stats->heightMin, BUFFER);
			
				//smoothing width data
				for(int j=0; j<layers; j++)
				{
					stats->widthMax[i] =smooth(stats->widthMax[i], BUFFER);
					stats->widthMin[i] =smooth(stats->widthMin[i], BUFFER);
				}
				
				//getting the plot ID's 
				string plotID = fileName.substr(IDbounds[i].upper,IDbounds[i].lower);
				startY=stats->plot->minY;
				endY=stats->plot->maxY;
				//writing to files
				writeToFile(plotID, stats->heightMax, stats->heightMin, stats->widthMax, stats->widthMin);
				
			}
		
			cout<<"Sucsess! ";
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


void writeHeader(FILE *file, string ID )
{

	
	fprintf(file,"AttributeID,Value,Units, \n");
	fprintf(file,"Plot,%s,[], \n" , 	ID.c_str());
	fprintf(file,"#_layers,%i,[],\n" ,	layers);
	fprintf(file,"Outlier_Removal_points,%i,[],\n" ,	pointsOR);
	fprintf(file,"Outlier_Removal_standard_deveation_multiplyer,%f,[],\n" ,	stdMulOR);
	fprintf(file,"MaximumZ,%f,m, \n" ,	MAXZ );
	fprintf(file,"MinimumZ,%f,m, \n" ,	MINZ);
	fprintf(file,"StartingY,%f,m,\n" ,	startY);
	fprintf(file,"EndingY,%f,m,\n" ,	endY);
	fprintf(file,"StepSize,%f,m,\n" ,	STEP);
	fprintf(file,"Scope,%f,m,\n" ,		SCOPE);
	fprintf(file,"Smoothing_Buffer,%d,[],\n" ,	BUFFER);
	
	fprintf(file,"Distance_from_Y0, Height_Maximum, Height_Minimum, " );

	for(int i=0; i<layers; i++)
	{
		fprintf(file, "Width_Maximum[%i],",i);
	}
	for(int i=0; i<layers; i++)
	{
		fprintf(file, "Width_Minimum[%i],",i  );
	}
	fprintf(file, "\n");
}



void writeToFile(string ID, vector<double> pointHeightMax, vector<double>  pointHeightMin, vector< vector<double> > pointWidthMax, vector< vector<double> > pointWidthMin)
{
	double northing, easting;
	FILE * newFile;
	string outFile= "Envelope_"+ID+".csv"; 
	newFile = fopen((OUTPUTDIR+outFile).c_str(),"w");
	writeHeader(newFile,ID);
	
	//printing vectors
	float location;
	location=YSTART;
	for(int i=0; i<pointHeightMin.size(); i++)
	{
		
		// location height width
		fprintf(newFile, "%f, %f, %f,",location, pointHeightMax[i], pointHeightMin[i] );
		
		for(int j=0; j<layers; j++)
		{
			fprintf(newFile, "%f,",pointWidthMax[j][i] );
		}
		for(int j=0; j<layers; j++)
		{
			fprintf(newFile, "%f,",pointWidthMin[j][i] );
		}
		fprintf(newFile, "\n");
		
		location+=STEP;
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


vector<double> smooth(vector<double> data, int buffer)
{
	vector<double> smoother;
	double average;
	int size;
	size = data.size();	

	for(int i=0; i<data.size(); i++)
	{
		smoother.push_back(data[i]);
		if(smoother.size()>buffer)
		{smoother.erase(smoother.begin());}

		average=0;
		for(int j=0; j<smoother.size(); j++)
		{average+= smoother[j];}

		data[i]=average/smoother.size();
	}
	
	while(!smoother.empty())
	{
		average=0;
		for(int i=0; i<smoother.size(); i++)
		{average+=smoother[i];}
		data.push_back(average/smoother.size());
		smoother.erase(smoother.begin());
	}
	
	return data;
}


void getBounds(string fileName, int i)
{
	int cut,length;
	bool again = true;
	char yn;
	string sub;
	while(again)
	{
		cout<< "Where's the id in the fileName start? plot["<< i <<"] 0-" <<fileName.length()<<": ";
		cin>>cut;
		cout<<"How long is the ID?  ";
		cin>>length;
		
		sub = fileName.substr(cut,length);
		
		cout<<"Is this the correct Identifier? "<<sub;
		cout << " (Y/N)\n";
		cin>>yn;
		again = !(yn=='Y' || yn == 'y');
	}
	IDbounds[i].upper=cut;
	IDbounds[i].lower=length;
	
}







