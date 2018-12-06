#include <string>
#include "ModGPS.h"
using namespace std;

int main()
{
	ModGPS mod;
	mod.readGpsString("74, 2017, 07, 10, 11, 34, 04, .551,3582556445, $GPGGA,183343.00,3304.28504364,N,11158.48747268,W,4,13,0.9,360.942,M,-27.204,M,1.0,0001*72$GPRMC,183343.00,A,3304.28504364,N,11158.48747268,W,0.026,89.279,100717,999.9000,E,D*23");

	cout<<"\t\t\t\t\tNorthing: "<<mod.getNorthing()<<" Easting: "<<mod.getEasting()<<endl;
	
	return 0;
}



































