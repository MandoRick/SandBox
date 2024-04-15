#ifndef POINTSONCIRCLE_H
#define POINTSONCIRCLE_H
#include <math.h>

class PointsOnCircle {
public:
  struct Point {
    int x;
    int y;
  };

  PointsOnCircle();
  Point calculate_point_on_circle(int radius, int centerX, int centerY, float angle_degrees);

private:
};

#endif
