#ifndef HEADER_VECTOR
#define HEADER_VECTOR
#include "Constant.h"
#include "Point.h"
using namespace std;

class Point;

// 向量, 为方便计算，假设所有向量表示形式均为从原点指向指定的一点
class Vector {
public:
    Point point; // 从原点指向的点
    
    Vector();
    Vector(Point point);
    Vector(Point start, Point end);
    Vector operator+ (const Vector &another);
    Vector operator- (const Vector &another);
    Vector operator* (const double &another);
    Vector operator/ (const double &another);
    double operator*(const Vector &another); // 点乘
    Vector x(Vector another); // 叉乘
    void unit(); // 单位化
    string toString();
};
double cos(Vector vector1, Vector vector2);
#endif