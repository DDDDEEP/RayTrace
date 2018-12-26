#ifndef HEADER_POINT
#define HEADER_POINT
#include "Constant.h"
#include <string>
#include <vector>
using namespace std;

class Color {
public:
    double red = 0.0, green = 0.0, blue = 0.0, alpha = 1.0;
    Color();
    Color(double red, double green, double blue, double alpha);
    Color& operator= (const Color &another);
    bool operator!= (const Color &another);
    bool operator== (const Color &another);
    Color operator+ (const Color &another);
    Color operator* (const double &another);
    Color& operator+= (const Color &another);
    Color& operator*= (const double &another);
    void setColor(double red, double green, double blue, double alpha);
    void unit();
    string toString();
};

class Point {
public:
    double x = 0, y = 0, z = 0;
    int objectIndex = -1; // 所属的物体的索引
    int planeIndex = -1; // 所属的平面的索引

    Point();
    Point(double value);
    Point(double x, double y, double z);
    string toString();
    Point& operator= (const Point &another);
    bool operator!= (const Point &another);
    bool operator== (const Point &another);
    Point operator+ (const Point &another);
    Point operator- (const Point &another);
    Point operator* (const double &another);
    Point operator/ (const double &another);
    Point& operator+= (const Point &another);
    Point& operator-= (const Point &another);
    Point& operator*= (const double &another);
    Point& operator/= (const double &another);
    double getCoord(int index);
    Point getClosestPoint(vector<Point> points);
};
double pointDistance(Point left, Point right);

// 颜色点
class ColorPoint : public Point {
public:
    Color color;
    ColorPoint();
    ColorPoint(double x, double y, double z, Color color);
    string toString();
};
#endif