#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>

#define _USE_MATH_DEFINES

class Point
{
  public:
    double x;
    double y;
    double z;
};

class PointCloudBuilder
{
  private:
    int numberofPoints=0;
    Point minPoint;
    std::vector<Point> workingRow;
    std::ofstream cloud;


  const double PI  =3.141592653589793238463;
  protected:
  public:
  PointCloudBuilder();
  void addPoints(std::vector<int> distance, double angle, double stepAngle, double scale);
  void rotateRow(double heading);
  void placeRow(double northing, double easting);

  double toRad(double angle, int steps);
  double toDegree(double angle, int steps);

  void writeFile(std::string fileName);

  void adjustPoint(Point *p);
  void updateMin(Point p);
};

