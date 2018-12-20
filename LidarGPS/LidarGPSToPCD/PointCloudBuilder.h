#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <sstream>

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
    double mountingHeight;
    double roll, pitch, yaw;

    bool adjust;
    int numberofPoints=0;
    Point minPoint;
    std::vector<Point> workingRow;
    std::ofstream cloud;
    void adjustPoint(Point *p);
    void updateMin(Point p);
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

  void writeFile(std::string fileName);
  void setMin(int x, int y, int z);
  void setMountingHeight(double heightIn);
  void setRoll(double rollIN);
	void setPitch(double pitchIN);
	void setYaw(double yawIN);
};

