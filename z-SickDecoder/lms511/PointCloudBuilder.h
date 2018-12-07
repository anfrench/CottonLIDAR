#include <vector>
#include <math.h>

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
    std::vector<Point> cloud, workingRow;

  protected:
  public:
  PointCloudBuilder();
  void addPoints(std::vector<int> distance, double angle, double stepAngle);
  void rotateRow(double heading);
  void placeRow(double northing, double easting);
};