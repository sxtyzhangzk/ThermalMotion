#ifndef TM_COMMON_H
#define TM_COMMON_H

#include <vector>
#include <cmath>

struct Vector
{
	double x, y;
	Vector() : x(0), y(0) {}
	Vector(double x, double y) : x(x), y(y) {}
	Vector(const Vector &other) : x(other.x), y(other.y) {}
	Vector operator+(const Vector &rhs) const
	{
		return Vector(x + rhs.x, y + rhs.y);
	}
	Vector operator-(const Vector &rhs) const
	{
		return Vector(x - rhs.x, y - rhs.y);
	}
	double dot(const Vector &rhs) const
	{
		return x * rhs.x + y * rhs.y;
	}
	double cross(const Vector &rhs) const
	{
		return x * rhs.y - y * rhs.x;
	}
	double operator*(const Vector &rhs) const
	{
		return dot(rhs);
	}
	Vector operator*(double a) const
	{
		return Vector(a * x, a * y);
	}
	double length() const
	{
		return sqrt(x * x + y * y);
	}
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