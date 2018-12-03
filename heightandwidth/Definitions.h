//input(FileName)/ output(Dir path)
#define INPUTFILE "plots.txt"
#define OUTPUTDIR "ConvexHulls/"

//removing statistical outlyers
#define POINTS 100
#define STDMUL 1.80

//croping
#define MAXZ 2.0
#define MINZ 0.1
#define XOUTER 3.0
#define XINNER .00

//second Croping to clean data to find plant width
#define SECONDMINZ 0.1

//for printing defined atrabutes as strings
#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

// y starting and ending points
#define YSTART 0.0
#define YEND 12.5

//length along the y axis that is considered at a time
#define SCOPE .05

//step Size
#define STEP .01

//assumed crop midpoint (From lidar to center of a row positive)
#define CROPCENTER .55 //due to complications not currently being used.//

//The buffer used to smooth data
#define BUFFER 15
