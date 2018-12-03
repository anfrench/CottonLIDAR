//input(FileName)/ output(Dir path)
#define INPUTFILE "plots.txt"
#define OUTPUTDIR "NoFilter/"

//utm adjustment

#define NORTHING 3659000
#define EASTING 409000
/*
#define NORTHING 0.0
#define EASTING 0.0
*/
//difrence of normals downsampling
#define SHORT 0.02
#define LARGE 0.07
#define MAG 0.5

//removing statistical outlyers
#define POINTS 100
#define STDMUL 2.0

//clustering
#define TOLERANCE .045

//croping
#define MAXZ 2.0
#define MINZ 0.03
#define XOUTER 1.05
#define XINNER .01

//aspect ratio filter
#define DIMENSIONS "y"
#define MAXASPECT 15.0

//Volume filter
#define MAXVOLUME 100.0
#define MINVOLUME 0.001

//for printing defined atrabutes as strings
#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

