#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include "Point.h"

#define _USE_MATH_DEFINES

class PointCloudBuilder
{
  private:
    std::string tempFileName="tempFile";
    int maxPoints=0, leadingPoints=0;
    int numberofPoints=0;
    double mountingHeight=0;
    double roll=0, pitch=0, yaw=0;
    double stepAngle=0, scale=0;
    bool noBounds=true;
    Point shiftValue, boundMax, boundMin, maxVal, minVal, rowMaxVal, rowMinVal;
    std::vector<Point> workingRow;
    std::vector<int> distances;
    std::ofstream cloud;
    bool Normals;

    bool inBounds(Point p);
    Point readPointString(std::string pointString);
  const double PI  =3.141592653589793238463;

  protected:
  void openTempFile();
  void makePoints();
  void rotateRow();
  void setHeight();
  void checkRow();

  void findBestRollOffset(double decPlace);

  void discardCurrent();

  public:

  PointCloudBuilder();
  ~PointCloudBuilder();

  void doNormals();
  void placeRow(double northing, double easting);

  double toRad(double angle, int steps);
  double toDegree(double angle, int steps);
  double findXYDist(Point p);
  double findXYNormDist(Point p);

  Point getMaxValues();
  Point getMinValues();

  void writeFile(std::string fileName);
  void writeFileWithNormals(std::string fileName);
  void writeFileNoNormal(std::string fileName);
  void setShift(Point shiftIN);
  void setBounds(Point lower, Point upper);
  void setMountingHeight(double heightIn);
  void setRoll(double rollIN);
	void setPitch(double pitchIN);
	void setYaw(double yawIN);
  void setMaxPoints(int maxPointsIN);
  void setLeadingPoints(int leadingPointsIN);
  void setDistValues(std::vector<int> vectorIn);
  void setAngularStep(double stepAngleIN);
  void setScale(double scaleIN);

  void process();

  void findBestRoll();

  bool isDone();
};

