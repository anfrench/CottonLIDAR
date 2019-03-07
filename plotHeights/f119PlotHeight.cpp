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
	double avHeight;
	double stdDev;
	
};
void printTime(int seconds);
PlantHeights findHeight(int id, pclCluster *row );
PlantHeights findquantileHeight(int id, pclCluster *row,int percentage );
void utcScanner(string line, int *id, string* dim1, double*high1,double*low1,string* dim2, double*high2,double*low2);
double string2double(string in);
string intToString(int in);

vector<double> findAverage(vector<double> data);

int main()
{
	int canopyDencity;
	vector<PlantHeights> heights;
	vector<PlantHeights> quantileHeights;
	//variables used for timeStamps
	time_t timeTotal, timePer, end;
	timeTotal=time(NULL);
	int seconds;

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
			canopy->makeGround(canopyDencity);
			canopy->makeHeights(canopyDencity);
			cout<<"CanopyMade \n";

			//Canopy is done and passes cloud back
			*cluster=canopy->getHeights();
			canopy->emptyCanopy();
			//delete(canopy);			
			cout<<"Canopy Passed Back\n";
			cluster->findSize();
			//cluster->findSize();

			cout<<"Canopy edited \n"<<endl;
			
			//creating croping variables
			int id;
			string dim1, dim2,bound;
			double high1,high2, low1, low2;	
			
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
	double average =0, count=1;
	vector<double> data;
	cout<<"--------------"<<endl;
	for(int i=0; i<row->cloud->points.size(); i++ )
	{
		cout<<row->cloud->points[i].z<<endl;
		data.push_back(row->cloud->points[i].z);
	}
	
	temp.avHeight= findAverage(data)[0];
	
	
	for(int i=0; i<data.size(); i++ )
	{
		data[i]=pow(data[i]-temp.avHeight,2);
	}
	
	temp.stdDev= sqrt(findAverage(data)[0]);
	
	temp.id= intToString(id);
	return temp;
}

///working on this method.
PlantHeights findquantileHeight(int id, pclCluster *row,int percentage )
{
	vector<double> heights, data;
	PlantHeights temp;

	for(int i=0; i<row->cloud->points.size(); i++ )
	{
		heights.push_back(row->cloud->points[i].z);
	}
	percentage= (int)((percentage*heights.size())/100);
	sort(heights.begin(),heights.end());
	
	for(int i = percentage; i< heights.size(); i++)
	{
		data.push_back(heights[i]);
	}
	
	temp.avHeight= findAverage(data)[0];
	
	
	for(int i=0; i<data.size(); i++ )
	{
		data[i]=pow(data[i]-temp.avHeight,2);
	}
	
	temp.stdDev= sqrt(findAverage(data)[0]);
	
	temp.id= intToString(id);
	return temp;
	
}

/*
	@Param:
	@Returns:
	@Prints:
	@uses:
		string to set all other variables
		
*/
void utcScanner(string line, int *id, string* dim1, double*high1,double*low1,string* dim2, double*high2,double*low2)
{
	string token;
	istringstream iss(line);
	
	getline(iss, token,',');
	*id = ((int)string2double(token));
	
	getline(iss, token,',');
	*dim1 = token;
	
	getline(iss, token,',');
	*high1 = string2double(token);
	
	getline(iss, token,',');
	*low1 = string2double(token);
	
	getline(iss, token,',');
	*dim2 = token;
	
	getline(iss, token,',');
	*high2 = string2double(token);
	
	getline(iss, token,',');
	*low2 = string2double(token);
}

/*
	@Param:
		Number as a string
	@Returns:
		Number as a double
	@Prints:
		null
*/
double string2double(string in)
{
	double out=0;
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


vector<double> findAverage(vector<double> data)
{
	vector<double> average;
	double sum=0, count=0;
	if(data.size()==0){average.push_back(0); return average;}
	else if(data.size()==1){return data;}
	else
	{
		for(int i=0; i<data.size(); i++)
		{
			if(count==1000)
			{
				average.push_back(sum/count);
				sum=0;
				count=0;
			}
			sum+=data[i];
			count++;
		}
		average.push_back(sum/count);
		return findAverage(average);
	}

}






