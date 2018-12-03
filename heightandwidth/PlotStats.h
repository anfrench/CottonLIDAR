#include "pclLib/pclCluster.h"
#include "pclLib/Functions.h"



class PlotStats
{
	public:
	double start, stop, step,scope;
	pclCluster *plot;
	vector<vector<double> > widthMax;
	vector<vector<double> > widthMin;
	vector<double> heightMax;
	vector<double> heightMin;
	int numLayers;
	
	PlotStats(pcl::PointCloud<pcl::PointXYZ>::Ptr cloudIN);
	PlotStats();
	~PlotStats();
	void setCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr cloudIN);
	void setup(float high, float low, int pointNum, float stdMul);
	void setScanValues(double stepIn, double scopeIn, int layersIn);
	void findHeight();
	void findWidth();
	void findLayerWidth(pclCluster *cloud);
};

PlotStats::PlotStats(pcl::PointCloud<pcl::PointXYZ>::Ptr cloudIN)
{
	this->plot= new pclCluster();
	pcl::copyPointCloud( *cloudIN, *(plot->cloud));
	this->plot->findSize();
}

PlotStats::PlotStats(){ this->plot= new pclCluster();}

PlotStats::~PlotStats(){delete plot;}

void PlotStats::setCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr cloudIN)
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr temp(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::copyPointCloud(*cloudIN, *temp);
	this->plot->cloud=temp;	 
	this->plot->findSize();
	start = this->plot->minY;
	stop  = this->plot->maxY;
}

void PlotStats::setup(float high, float low, int pointNum, float stdMul)
{
	cout<<"plot setup\n";
	cout<<plot->cloud->points.size()<<" Points \n";
	cout<<high<<" : " <<low<<" High/low\n";
	plot->crop("x",high, low);
	cout<<plot->cloud->points.size()<<" Points \n";
	plot->translateX(0);
	plot->removeOutliers(pointNum, stdMul);
}


void PlotStats::setScanValues(double stepIn, double scopeIn, int layersIn)
{
	step=stepIn;
	scope=scopeIn;
	numLayers = layersIn;
}

void PlotStats::findHeight()
{
	double max, min;
	//calculating height values
	for(double yCurr=start; yCurr<stop; yCurr+=step)
	{
		
		//Finding min/max height for each cloud
		plot->localizedMaxMin('z', yCurr, yCurr+scope, &max, &min);
		heightMax.push_back(max);
		heightMin.push_back(min);
	}
}


void PlotStats::findWidth()
{
	pclCluster *layer= new pclCluster;
	pcl::PointCloud<pcl::PointXYZ>::Ptr temp(new pcl::PointCloud<pcl::PointXYZ>);
	layer->cloud=temp;
	//calculating layer size
	double lvlHeight;
	double lvlBase;
	
	lvlHeight = (plot->maxZ-plot->minZ)/numLayers;
	lvlBase= plot->minZ;
	
	for(int i=0; i<numLayers; i++)
	{		
		cout<<lvlBase<< " Layer base "<<lvlHeight<<"Height\n";
		pcl::copyPointCloud(*(this->plot->cloud), *(layer->cloud));
		cout<<"before crop layer size"<<layer->cloud->points.size();
		layer->crop("z", lvlBase+lvlHeight, lvlBase);
		cout<<"layer size"<<layer->cloud->points.size();
		findLayerWidth(layer);
		lvlBase+= lvlHeight;
	}
}

void PlotStats::findLayerWidth(pclCluster *cloud)
{
	vector<double> tempMax;
	vector<double> tempMin;
	double max, min;
	//calculating height values
	for(double yCurr=start; yCurr<stop; yCurr+=step)
	{
		//Finding min/max height for each cloud
		cloud->localizedMaxMin('x', yCurr, yCurr+scope, &max, &min);
		tempMax.push_back(max);
		tempMin.push_back(min);
	}
	this->widthMax.push_back(tempMax);
	this->widthMin.push_back(tempMin);
}




















