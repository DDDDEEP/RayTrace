#ifndef HEADER_RAY
#define HEADER_RAY
#include "Point.h"
#include "Constant.h"
#include "Vector.h"
#include <string>
using namespace std;

class Point;

// 光线
class Ray {
public:
    Point P; // 起始点
    Vector D; // 射线方向
    double t;
    
    Ray(Point start, Point end);
    Ray(Point point, Vector vector);
    string toString();
};
#endif