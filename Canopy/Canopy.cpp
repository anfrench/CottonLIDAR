#include "Definitions.h"
#include "pclLib/pclCluster.h"
#include "pclLib/Functions.h"
#include "pclLib/Canopy.h"
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>
#include <pcl/io/vtk_io.h>
#include "PollyFit/Polynomial.hh"


// canopy
using namespace std;

string fileName;
void printTime(int seconds);
void writeToFile(pclCluster clusterCanopy);
void writeHeader(FILE *file, string ID);


int main(int argc, char*argv[])
{
	bool makeFloor=false;
	if(argc>= 2 && 0==strcmp(argv[1], "floor"))
	{
		makeFloor=true;
		cout<<"Making Floor\n";
	}

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

			canopy->setCloud(*cluster);
			if(makeFloor)
			{
				canopy->makeFloor(canopyRatio);
			}
			else
			{
			 	canopy->makeCanopy(canopyRatio);
			}
		
			*cluster=canopy->getCanopy();

			////////testImplementation
			
			/*vector<double> xyPoints, zPoints;
			for(int i=0; i<cluster->cloud->points.size(); i++)
			{
				xyPoints.push_back(cluster->cloud->points[i].x);
				xyPoints.push_back(cluster->cloud->points[i].y);
				
				zPoints.push_back(cluster->cloud->points[i].z);
				
			}
			
			Polynomial *polly = new Polynomial(xyPoints,zPoints,2,3 );
			
			pclCluster *pollyFit= new pclCluster();
			for(double i=0; i<150; i+=.001)
			{
				vector<double> xy;
				xy.push_back(i);
				xy.push_back(i);
				double z=0;// = polly->eval(xy);
				pcl::PointXYZ point;
				point.x=i;
				point.y=i;
				point.z=z;
				pollyFit->cloud->points.push_back(point);
			}
			
			pollyFit->cloud->width = pollyFit->cloud->points.size();
			pollyFit->cloud->height =1;
			pollyFit->cloud->is_dense=true;
			pollyFit->save("PollyFit.pcd");
*/
			//////////////

			
			if(makeFloor){fileName= "Floor_"+fileName;}
			else{fileName= "Canopy_"+fileName;}
			cluster->save(fileName);
			
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









