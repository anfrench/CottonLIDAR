#include "pclLib/pclCluster.h"
#include "pclLib/Functions.h"
#include "pclLib/Canopy.h"
#include <cmath>
#include<vector>
#include <string>
#include <sstream>
//git not working... test test
//input(FileName)/ output(Dir path)
#define INPUTFILE "plots.txt"
#define BOUNDS "Bounds.csv"

//for printing defined atrabutes as strings
#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

using namespace std;


/*
	Class
	Public:
	 	Average Height
	 	Standard Deveation
	 	And An Id.
	
*/
struct PlantHeights
{
	public:
	string id;
	float avHeight;
	float stdDev;
	
};
void printTime(int seconds);
PlantHeights findHeight(int id, pclCluster *row );
PlantHeights findquantileHeight(int id, pclCluster *row,int percentage );
PlantHeights findquantileDifference(int id, pclCluster *row,int percentage );
void utcScanner(string line, int *id, string* dim1, float*high1,float*low1,string* dim2, float*high2,float*low2);
float string2Float(string in);
string intToString(int in);


int main()
{
	float zHeight=10, zMin=-5;
	int canopyDencity;
	vector<PlantHeights> heights;
	vector<PlantHeights> quantileHeights;
	vector<PlantHeights> quantileDifference;//need to impliment this.
	//variables used for timeStamps
	time_t timeTotal, timePer, end;
	timeTotal=time(NULL);
	int seconds;

	cout<<"Float Enter Maximum Height: ";	
	cin>> zHeight;
	cout<<"Float Enter Minimum Height: ";	
	cin>> zMin;
	cout<<"Int Enter Canopy Dencity (5-10 recomended): ";
	cin>> canopyDencity;
		

	//File where the filenames are kept
	fstream names;
	names.open(INPUTFILE, fstream::in);
	string fileName;
	
	//proceses every file in the input file
	while(getline(names, fileName))
	{
		//opening output file
		FILE * outFile;
		outFile = fopen((fileName.substr(0,fileName.length()-4)+"_Heights.csv").c_str(),"w");
		//Reseting the time
		timePer=time(NULL);
		
		cout<<"Working on: "<<fileName<<endl;
		try
		{
			//Initalizing all classes with point cloud pointers
			pclCluster *cluster= new pclCluster();
			pclCluster *row= new pclCluster();
			pcl::PointCloud<pcl::PointXYZ>::Ptr temp( new pcl::PointCloud<pcl::PointXYZ>);
			row->cloud = temp;
			PointCanopy *canopy= new PointCanopy();
		
			cluster->open(fileName);
			cout<<fileName<<": Open!"<<endl;
			
			//passing cloud to canopy to be procesed
			canopy->setCloud(*cluster);
			canopy->makeCanopy(canopyDencity);
			cout<<"CanopyMade \n";

			//Canopy is done and passes cloud back
			*cluster=canopy->getCanopy();
			canopy->emptyCanopy();
			//delete(canopy);			
			cout<<"Canopy Passed Back\n";
			cluster->findSize();
			//cluster->findSize();

			cout<<"Canopy edited \n"<<endl;
			
			//creating croping variables
			int id;
			string dim1, dim2,bound;
			float high1,high2, low1, low2;	
			
			//Opening file where row bounds for f119 are kept
			fstream bounds;
			bounds.open(BOUNDS, fstream::in);
			while(getline(bounds, bound))
			{	
				//Setting row bounds and ID
				utcScanner(bound,&id,&dim1,&high1,&low1,&dim2,&high2,&low2);
				
				//coping cloud to row
				pcl::copyPointCloud<pcl::PointXYZ>(*(cluster->cloud),*(row->cloud));
				
				//croping plot to only one row
				row->crop(dim1, high1, low1);
				row->crop(dim2, high2, low2);
				
				//cout<<"Row Size: "<<row->cloud->points.size()<<endl;

				//finding plant height 
				heights.push_back(findHeight(id,row ));
				quantileHeights.push_back(findquantileHeight(id,row,95));
			}
			
			//deleting clouds
			cluster->cloud.reset();			

			//printing results to file
			fprintf(outFile, "Plot_ID, Average_Height, Standard_Deveatioin, averagequantile, quantileStandearDeveation\n" );
			for(int i=0; i<heights.size(); i++)
			{
				fprintf(outFile, "%s,%f,%f", heights[i].id.c_str(), heights[i].avHeight, heights[i].stdDev);
				fprintf(outFile, ",%f,%f\n", quantileHeights[i].avHeight, quantileHeights[i].stdDev);
		   	}
			
			//clearing vector
			heights.clear();
			quantileHeights.clear();
			fclose(outFile);
			bounds.close();
			//printing time per file
			cout<<"sucsess! ";
			end=time(NULL);
			seconds = difftime(end, timePer);
			printTime(seconds);
		}
		catch(...){cout<<"ERROR: "<<fileName<<endl;}
	}
	//printing total Time
	seconds = difftime(end, timeTotal);
	cout<<"Total ";
	printTime(seconds);
		
}




/*
	@Param:
			intager representing seconds
	@Returns:
	@Prints:
			Time (hours):(Minuts):(Seconds)
	
*/
void printTime(int seconds)
{
	int hours, minuts=0;
	minuts = seconds/60;
	seconds = seconds %60;
	hours = minuts /60;
	minuts = minuts%60;
	cout<<"Time "<<hours<<" : "<<minuts <<" : " << seconds<<endl;
	
}


/*
	@Param: 
		Int representing an id
		A pointer to a pclCluster
	@Returns:
		PlantHeightObject
	@Prints:
		PlantHeight vals
*/
PlantHeights findHeight(int id, pclCluster *row )
{
    PlantHeights temp;
	float sum=0;
	for(int i=0; i<row->cloud->points.size(); i++ )
	{
		sum += row->cloud->points[i].z/row->cloud->points.size();
	}
	
	temp.avHeight= sum;//row->cloud->points.size();
	
	sum = 0;
	
	for(int i=0; i<row->cloud->points.size(); i++ )
	{
		sum+= pow(row->cloud->points[i].z-temp.avHeight,2);
	}
	temp.stdDev= sqrt(sum/row->cloud->points.size());
	
	
	temp.id= intToString(id);
	//cout<<temp.id<<", "<<temp.avHeight<<", "<<temp.stdDev<<endl;
	return temp;
}

///working on this method.
PlantHeights findquantileHeight(int id, pclCluster *row,int percentage )
{
	vector<float> heights;
	PlantHeights temp;
	int numPoints =0;

	for(int i=0; i<row->cloud->points.size(); i++ )
	{
		heights.push_back(row->cloud->points[i].z);
	}
	percentage= (int)((percentage*heights.size())/100);
	sort(heights.begin(),heights.end());
	
	float sum=0;
	for(int i = percentage; i< heights.size(); i++)
	{
		sum += heights[i];
		numPoints ++;
	}
	temp.avHeight = sum/(numPoints);
	
	sum=0;
	for(int i = percentage; i< heights.size(); i++ )
	{
		sum+= pow(heights[i]-temp.avHeight,2);
	}
	temp.stdDev= sqrt(sum/numPoints);
	
	temp.id= intToString(id);
	//cout<<temp.id<<", "<<temp.avHeight<<", "<<temp.stdDev<<endl;
	return temp;
}



/*
	This is where I'm working.... 
	find a way to subtract the min from the max...
*/
PlantHeights findquantileDifference(int id, pclCluster *row,int percentage )
{
	vector<float> heights;
	PlantHeights temp;
	int numPoints =0;

	for(int i=0; i<row->cloud->points.size(); i++ )
	{
		heights.push_back(row->cloud->points[i].z);
	}
	percentage= (int)((percentage*heights.size())/100);
	sort(heights.begin(),heights.end());
	
	float sum=0;
	for(int i = percentage; i< heights.size(); i++)
	{
		sum += heights[i];
		numPoints ++;
	}
	temp.avHeight = sum/(numPoints);
	
	sum=0;
	for(int i = percentage; i< heights.size(); i++ )
	{
		sum+= pow(heights[i]-temp.avHeight,2);
	}
	temp.stdDev= sqrt(sum/numPoints);

	
	temp.id= intToString(id);
	//cout<<temp.id<<", "<<temp.avHeight<<", "<<temp.stdDev<<endl;
	return temp;
}




/*
	@Param:
	@Returns:
	@Prints:
	@uses:
		string to set all other variables
		
*/
void utcScanner(string line, int *id, string* dim1, float*high1,float*low1,string* dim2, float*high2,float*low2)
{
	string token;
	istringstream iss(line);
	
	getline(iss, token,',');
	*id = ((int)string2Float(token));
	
	getline(iss, token,',');
	*dim1 = token;
	
	getline(iss, token,',');
	*high1 = string2Float(token);
	
	getline(iss, token,',');
	*low1 = string2Float(token);
	
	getline(iss, token,',');
	*dim2 = token;
	
	getline(iss, token,',');
	*high2 = string2Float(token);
	
	getline(iss, token,',');
	*low2 = string2Float(token);
}

/*
	@Param:
		Number as a string
	@Returns:
		Number as a float
	@Prints:
		null
*/
float string2Float(string in)
{
	float out=0;
	stringstream ss;
	ss<<in;
	ss>>out;
	
	
	return out;
}


/*
	@param
		number as an int
	@param
		Number as a string
*/
string intToString(int in)
{
	string out="";
	stringstream ss;
	ss<<in;
	ss>>out;
	return out;
}










