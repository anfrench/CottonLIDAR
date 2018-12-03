#include "Definitions.h"
#include "pclLib/pclCluster.h"
#include "pclLib/Functions.h"
#include "pclLib/Canopy.h"
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>
#include <pcl/io/vtk_io.h>



using namespace std;

string fileName;
void printTime(int seconds);
void writeToFile(pclCluster clusterCanopy);
void writeHeader(FILE *file, string ID);

int main()
{
	//variables used for timeStamps
	time_t timeTotal, timePer, end;
	timeTotal=time(NULL);
	int seconds;

	int canopyRatio;
	cout<<"What's the Canopy Ratio? (5-10 recomended): ";
	cin>>canopyRatio;

	//File where the filenames are kept
	fstream names;
	names.open(INPUTFILE, fstream::in);
	while(getline(names, fileName))
	{
		timePer=time(NULL);
		pclCluster *cluster= new pclCluster();
		//pclCluster *cloud= new pclCluster();
		PointCanopy *canopy= new PointCanopy();
		cout<<"Working on: "<<fileName<<endl;
		try
		{
			
			cluster->open(fileName);
			cluster->findSize();
			cluster->translateX(0);
			cluster->translateY(0);
			cluster->findSize();
			canopy->setCloud(*cluster);
			canopy->makeCanopy(canopyRatio);
			*cluster=canopy->getCanopy();
			cluster->findSize();
			cluster->crop("z", 5, -1);
			writeToFile(*cluster);			

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




void printTime(int seconds)
{
	int hours, minuts=0;
	minuts = seconds/60;
	seconds = seconds %60;
	hours = minuts /60;
	minuts = minuts%60;
	cout<<"Time "<<hours<<" : "<<minuts <<" : " << seconds<<endl;
	
}




void writeToFile(pclCluster clusterCanopy)
{
	string nFileName;
	pcl::PointXYZ p;
	FILE * outFile;
	nFileName = fileName.substr(0,fileName.length()-4)+".csv";
	outFile = fopen((OUTPUTDIR+nFileName).c_str(),"w");
	cout<<"before header";
	writeHeader(outFile, fileName);
	cout<<"after writing header \n";
	for(int i=0; i<clusterCanopy.cloud->points.size(); i++)
	{
		p= clusterCanopy.cloud->points[i];
		fprintf(outFile, "%f,%f,%f \n", p.x, p.y, p.z );
	}
	fclose(outFile);
}




void writeHeader(FILE *file, string ID)
{
	fprintf(file,"AttributeID,Value,Units \n"		);
	fprintf(file,"PlotNumber,%s,[] \n" , 		ID.c_str());
	fprintf(file,"X,Y,Z \n"		);
}









