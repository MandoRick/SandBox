#include "PointsOnCircle.h"
#include <math.h>

// Define the radians function macro
#define radians(x) ((x) * M_PI / 180.0)

PointsOnCircle::PointsOnCircle() {}

PointsOnCircle::Point PointsOnCircle::calculate_point_on_circle(int radius, int centerX, int centerY, float angle_degrees) {
  Point point;
  float angle_radians = radians(angle_degrees);
  point.x = centerX + radius * cos(angle_radians);
  point.y = centerY + radius * sin(angle_radians);
  return point;
}
