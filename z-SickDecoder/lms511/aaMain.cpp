#include <string>
#include <fstream>
#include "ModGPS.h"
using namespace std;

int main()
{

	string line;
	ifstream gps;
	gps.open("File.txt");
	if (gps.is_open())
	{
		cout << "open";
	}
	ModGPS mod;
	int i = 0;
	while (getline(gps, line) && i < 500)
	{
		mod.readGpsString(line);
		cout << "\t\t\t\t\tNorthing: " << mod.getNorthing() << " Easting: " << mod.getEasting() << endl;
		i++;
	}

	return 0;
}
