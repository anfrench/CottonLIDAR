#include "Definitions.h"
#include "pclLib/pclCluster.h"
#include "pclLib/Functions.h"
#include "pclLib/Canopy.h"
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>
#include <pcl/io/vtk_io.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/filters/conditional_removal.h>


// canopy
using namespace std;

string fileName;
void printTime(int seconds);
void writeToFile(pclCluster clusterCanopy);
void writeHeader(FILE *file, string ID);
bool pointsEqual(pcl::PointXYZ p1, pcl::PointXYZ p2);

int main(int argc, char*argv[])
{
	//variables used for timeStamps
	time_t timeTotal, timePer, end;
	timeTotal=time(NULL);
	int seconds;

	int canopyRatio, iterations=6;
	cout<<"How many points per meter?: ";
	cin>>canopyRatio;

	//File where the filenames are kept
	fstream names;
	names.open(INPUTFILE, fstream::in);
	while(getline(names, fileName))
	{
		timePer=time(NULL);
		pclCluster *cluster= new pclCluster();
		cout<<"Working on: "<<fileName<<endl;
		try
		{
			for(int iter=0; iter<iterations; iter++)
			{
			cluster->open(fileName);
			PointCanopy canopy;
			canopy.setCloud(*cluster);
			
			pclCluster ground,groundFiltered;
			pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZ>);
			groundFiltered.cloud=cloud_filtered;
			
			
			canopy.makeGround(canopyRatio);
			ground=canopy.getGround();
			
			int minNeighbors=1;
			double searchRad=.07;
			
			double area=(ground.maxX-ground.minX)*(ground.maxY-ground.minY);
			minNeighbors = (int) ground.cloud->points.size()/area;
			minNeighbors=(int) (minNeighbors*searchRad)/20;
			
			minNeighbors++;
			cout<<"area: " << area << "points: "<<ground.cloud->points.size()<<endl;
			cout<<"min "<<minNeighbors<<endl;
			
			// build the filter
			pcl::RadiusOutlierRemoval<pcl::PointXYZ> outrem;
			outrem.setInputCloud(ground.cloud);
    		outrem.setRadiusSearch(searchRad);
   		 	outrem.setMinNeighborsInRadius (minNeighbors);
    		// apply filter
    		outrem.filter (*groundFiltered.cloud);
    		
    		for(int i=0; i<groundFiltered.cloud->points.size(); i++){
    			for(int k=0; k<cluster->cloud->points.size(); k++){
    				if(pointsEqual(groundFiltered.cloud->points[i],cluster->cloud->points[k])){
    					cluster->cloud->points[k].z=100;
    					break;
    				}
    			}
    		}
    		
    		cluster->cloud->width = cluster->cloud->points.size();
			cluster->cloud->height = 1;
			cluster->cloud->is_dense = true;
			cluster->crop("z", 50, -50);
    		
    		
    		if(iter ==iterations-1)
    		{
    		cluster->translateX(7);
    		groundFiltered.translateX(5);
			
			ground.save("Ground"+fileName);	
			groundFiltered.save("GroundFiltered"+fileName);
			cluster->save("diff.pcd");
			}
				
			}
			
			
			
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


bool pointsEqual(pcl::PointXYZ p1,pcl::PointXYZ p2)
{
	bool areEqual=true;
	
	if(p1.x!=p2.x || p1.y!=p2.y || p1.z!=p2.z)
	{
		areEqual=false;
	}
	
	return areEqual;
}






