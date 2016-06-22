#ifndef TM_COMMON_H
#define TM_COMMON_H

#include <vector>

struct Vector
{
	double x, y;
};
typedef Vector Point;
struct particle
{
	Point pos;
	Vector v;
	int r, g, b;
};
typedef std::vector<Point> obstacle;

extern std::vector<particle> Particles;
extern std::vector<obstacle> Obstacles;
extern const double SceneSize;
extern double pRadius;

#endif