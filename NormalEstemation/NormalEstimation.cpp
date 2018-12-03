#include "pclLib/pclCluster.h"
#include "pclLib/Functions.h"
#include <iostream>

using namespace std;

vector<pclCluster> filterByNormals(vector<pclCluster> clusters, int dim, float max,float min);


int main()
{
	vector<pclCluster> clusters;
	string fileNameIn,fileNameOut;
	float searchRad,clusteringTolerance;
	int maxClusterSize,minCluserSize;
	

	cout<<"What file would you like to proces?: \n";
	getline(cin, fileNameIn);
	cout<<"What would you like to name the result?:\n";
	getline(cin, fileNameOut);
	cout<<"Whats the maximum cluster Size?: \n";
	cin>> maxClusterSize;
	cout<<"What's the minimum cluster size?: \n";
	cin>> minCluserSize;
	cout<<"Whats the clustering Tolerance?: \n";
	cin>> clusteringTolerance;
	cout<<"What NormalEstimation search radious would you like to use?: ";
	cin>>searchRad;
	
	
	// Load input file 
	pclCluster *cluster= new pclCluster();
	cluster->open(fileNameIn);
	
	//clusters = cluster->EuclideanCluster(minCluserSize,maxClusterSize,clusteringTolerance);
	clusters = cluster->regonGrowingSegmentation();	


	for(int i=0; i<clusters.size(); i++)
	{
		clusters[i].cloud = clusters[i].getNormalCloud(searchRad);
	}
	
	clusters = filterByNormals(clusters, 3, 5 , .5);
	
	*cluster = combineVectorCloud(clusters);
	
	
	cluster->cloud = stereographicProjection(cluster->cloud);

	// Save output
	cluster->save(fileNameOut);
}



vector<pclCluster> filterByNormals(vector<pclCluster> clusters, int dim, float max,float min)
{
	vector<pclCluster> filteredClusters;
		
	for(int i=0; i<clusters.size(); i++)
	{
		clusters[i].findSize();
		if(clusters[i].getDimAverage(dim)> min && clusters[i].getDimAverage(dim)< max)
		{
			filteredClusters.push_back(clusters[i]);
			cout<< "Used : ";
		}
		cout<<"I :"<< i << "Normal :"<< clusters[i].getDimAverage(dim) << endl;
	}
	return filteredClusters;
}

