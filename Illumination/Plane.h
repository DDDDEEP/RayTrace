#ifndef HEADER_PLANE
#define HEADER_PLANE
#include "Constant.h"
#include "Point.h"
#include "Vector.h"
#include "Ray.h"
#include <vector>
using namespace std;

class Point;
class Vector;
class Ray;

// 平面
class Plane {
public:
    vector<Point> points;
    Vector n; // 法向量
    double A, B, C, D; // 平面方程系数

    Plane();
    Plane(vector<Point> points);
    Point getIntersectPointByRay(Ray ray); // 获得光线与平面的交点, 若为无效交点返回最大值点
    bool isInside(Point point); // 判断点是否在三角平面内
    string toString();

	Point computeGravity();
	

};
#endif
