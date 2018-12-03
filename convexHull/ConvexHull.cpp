#include "Definitions.h"
#include "pclLib/pclCluster.h"
#include "pclLib/Functions.h"
#include "pclLib/Canopy.h"
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>



#include <pcl/io/vtk_io.h>



using namespace std;

string fileName;
float toRad(double angle, int steps);
void printTime(int seconds);
void writeToFile(string ID, double northAdjust, double eastAdjust , vector<pclCluster> cloud);
void writeHeader(FILE *file, string ID ,vector<pclCluster> cloud, double xAdjust, double yAdjust);

int main()
{
	//variables used for timeStamps
	time_t timeTotal, timePer, end;
	timeTotal=time(NULL);
	int seconds;


	int canopyRatio, maxNeighbor;
	
	float searchRad, mu;
	float maxSurfaceAngle;
	float minAngle, maxAngle;
	
	cout<<"Enter Canopy Ratio(5-10 recomended):";
	cin>> canopyRatio; 
	cout<<"Enter Search Radious:";
	cin>> searchRad; 
	cout<<"Enter Maximum Surface Angle:";
	cin>> maxSurfaceAngle; 
	cout<<"Enter Minimum Angle:";
	cin>> minAngle; 
	cout<<"Enter Maximum Angle:";
	cin>> maxAngle; 





	
	//File where the filenames are kept
	fstream names;
	names.open(INPUTFILE, fstream::in);
	while(getline(names, fileName))
	{
		timePer=time(NULL);
		pclCluster *cluster= new pclCluster();
		pclCluster *cloud= new pclCluster();
		PointCanopy *canopy= new PointCanopy();
		cout<<"Working on: "<<fileName<<endl;
		try
		{
			
			cluster->open(fileName);
			cluster->findSize();
			cluster->translateX(0);
			cluster->translateY(0);
			cluster->translateZ(0);
			cluster->findSize();
			cluster->save("Sample.pcd");
			canopy->setCloud(*cluster);
			canopy->makeCanopy(canopyRatio);
			cout<<"Canopy Made"<<endl;
			
			canopy->fillGaps(1);
			canopy->fillGaps(2);
			canopy->fillGaps(1);
			canopy->fillGaps(2);
			canopy->smooth(3);
			*cluster=canopy->getCanopy();
			cluster->crop("z", 5, -1);
			//------------------------------


			

  cout<<"// Normal estimation*\n";
  pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;
  pcl::PointCloud<pcl::Normal>::Ptr normals (new pcl::PointCloud<pcl::Normal>);
  pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);
  tree->setInputCloud (cluster->cloud);
  n.setInputCloud (cluster->cloud);
  n.setSearchMethod (tree);
  n.setKSearch (30);
  n.compute (*normals);
  //* normals should not contain the point normals + surface curvatures

  cout<<"// Concatenate the XYZ and normal fields*\n";
  pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals (new pcl::PointCloud<pcl::PointNormal>);
  pcl::concatenateFields (*(cluster->cloud), *normals, *cloud_with_normals);
  //* cloud_with_normals = cloud + normals

  cout<<"// Create search tree*\n";
  pcl::search::KdTree<pcl::PointNormal>::Ptr tree2 (new pcl::search::KdTree<pcl::PointNormal>);
  tree2->setInputCloud (cloud_with_normals);

  // Initialize objects
  pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;
  pcl::PolygonMesh triangles;

  // Set the maximum distance between connected points (maximum edge length)
  gp3.setSearchRadius (searchRad);

  // Set typical values for the parameters
  gp3.setMu (mu);
  gp3.setMaximumNearestNeighbors (maxNeighbor);
  gp3.setMaximumSurfaceAngle(toRad(maxSurfaceAngle, 360)); 
  gp3.setMinimumAngle(toRad(minAngle, 360)); 
  gp3.setMaximumAngle(toRad(maxAngle, 360)); 
  gp3.setNormalConsistency(true);

  // Get result
  gp3.setInputCloud (cloud_with_normals);
  gp3.setSearchMethod (tree2);
  gp3.reconstruct (triangles);

  // Additional vertex information
  //std::vector<int> parts = gp3.getPartIDs();
 // std::vector<int> states = gp3.getPointStates();
pcl::io::saveVTKFile((fileName.substr(0,fileName.length()-4)+"Mesh.vtk"), triangles);
		//----------------------------*/
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



float toRad(double angle, int steps)
{
	return M_PI * angle / (steps / 2 );
}









