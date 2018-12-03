
#include <iostream>
#include <vector>
using namespace std;

int
main()
{
	vector<int> colors;
	int red, green, blue,rgb;
	rgb=1;
	while(true)
	{
		rgb =0;
		cout<<"------------\n";
		cout<<"Red: ";
		cin>>red;
		cout<<"Green: ";
		cin>>green;
		cout<<"Blue: ";
		cin>>blue;
		rgb = (red<<16)|(green<<8)|blue;
		cout<<"RGB: "<<rgb;
		
		if(rgb==0){break;}
		colors.push_back(rgb);
	}
	
	cout<<"\n \nconst int colors["<<colors.size()<<"]{";
	for(int i=0; i<colors.size()-1; i++){ cout<<colors[i]<<",";}
	cout<<colors.back()<<"};\n"; 
	cout<<endl;
	
	

}
