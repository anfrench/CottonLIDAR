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

  void writeFile(std::string fileName);
  void setShift(Point shiftIN);
  void setBounds(Point lower, Point upper);
  void setMountingHeight(double heightIn);
  void setRoll(double rollIN);
	void setPitch(double pitchIN);
	void setYaw(double yawIN);
};

