#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include "Point.h"

#define _USE_MATH_DEFINES
#define PROGRESS 1

class PointCloudBuilder
{
  //todo add Normals to points
  /*
    Some how Add Normals to the points.
              Perhaps add I as well?

    Add normals to points as they are being calculated.

        Step1: calculate a normal bassed on lidar angle alone.
        Step2: Change Normals for pitch.
        Step3: Change normals due to rotation of cloud for heading. 

    Change print statements.
        Change point type
        change fields
        change length of fields
        print normals with points.
    
    Test solution.
        Run same config.
        put in cloud compare.
        make mesh.
  */
 
  private:
    double mountingHeight;
    double roll, pitch, yaw;
    bool noBounds;
    int numberofPoints=0;
    Point shiftValue, boundMax, boundMin;
    std::vector<Point> workingRow;
    std::ofstream cloud;

    bool inBounds(Point p);
    Point readPointString(std::string pointString);
  const double PI  =3.141592653589793238463;
  protected:
  public:
  PointCloudBuilder();
  void addPoints(std::vector<int> distance, double angle, double stepAngle, double scale);
  void rotateRow(double heading);
  void placeRow(double northing, double easting);

  double toRad(double angle, int steps);
  double toDegree(double angle, int steps);
  double findXYDist(Point p);
  double findXYNormDist(Point p);

  void writeFile(std::string fileName);
  void setShift(Point shiftIN);
  void setBounds(Point lower, Point upper);
  void setMountingHeight(double heightIn);
  void setRoll(double rollIN);
	void setPitch(double pitchIN);
	void setYaw(double yawIN);
};

