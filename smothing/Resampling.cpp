#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/surface/mls.h>
#include <iostream>
#include <string>
#include <iostream>

using namespace std;

int
main (int argc, char** argv)
{
  string fileNameIn,fileNameOut;
  float searchRad;
  cout<<"What file would you like to proces?: \n";
  getline(cin, fileNameIn);
  cout<<"What would you like to name the result?:\n";
  getline(cin, fileNameOut);
  cout<<"What search radious would you like to use?: ";
  cin>>searchRad;
  // Load input file into a PointCloud<T> with an appropriate type
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ> ());
  // Load bun0.pcd -- should be available with the PCL archive in test 
  pcl::io::loadPCDFile (fileNameIn, *cloud);

  // Create a KD-Tree
  pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);

  // Output has the PointNormal type in order to store the normals calculated by MLS
  pcl::PointCloud<pcl::PointNormal> mls_points;

  // Init object (second point type is for the normals, even if unused)
  pcl::MovingLeastSquares<pcl::PointXYZ, pcl::PointNormal> mls;
 
  mls.setComputeNormals (true);

  // Set parameters
  mls.setInputCloud (cloud);
  mls.setPolynomialFit (true);
  mls.setSearchMethod (tree);
  mls.setSearchRadius (searchRad);

  // Reconstruct
  mls.process (mls_points);

	std::cout<<mls_points.points.size();

  // Save output
  pcl::io::savePCDFile (fileNameOut, mls_points);
}

