#include "stdafx.h"
using namespace std;

Color::Color() {

}

Color::Color(double red, double green, double blue, double alpha) {
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;
    unit();
}

Color& Color::operator= (const Color &another) {
    this->red = another.red;
    this->green = another.green;
    this->blue = another.blue;
    this->alpha = another.alpha;
    unit();
    return *this;
}

bool Color::operator== (const Color &another) {
    return this->red == another.red && this->green == another.green && this->blue == another.blue;
}

bool Color::operator!= (const Color &another) {
    return this->red != another.red && this->green != another.green && this->blue != another.blue;
}

Color Color::operator+ (const Color &another) {
    Color result = *this;
    result.red += another.red;
    result.green += another.green;
    result.blue += another.blue;
    result.unit();
    return result;
}

Color Color::operator* (const double &another) {
    Color result = *this;
    result.red *= another;
    result.green *= another;
    result.blue *= another;
    result.unit();
    return result;
}

Color& Color::operator+= (const Color &another) {
    this->red += another.red;
    this->green += another.green;
    this->blue += another.blue;
    unit();
    return *this;
}

Color& Color::operator*= (const double &another) {
    this->red *= another;
    this->green *= another;
    this->blue *= another;
    unit();
    return *this;
}

void Color::setColor(double red, double green, double blue, double alpha) {
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;
    unit();
}

void Color::unit() {
    if (this->red > 1) {
        this->red = 1;
    }
    if (this->red < 0) {
        this->red = 0;
    }
    if (this->green > 1) {
        this->green = 1;
    }
    if (this->green < 0) {
        this->green = 0;
    }
    if (this->blue > 1) {
        this->blue = 1;
    }
    if (this->blue < 0) {
        this->blue = 0;
    }
}

string Color::toString() {
    return "(r:" + doubleToString(red) + ",g:" + doubleToString(green) + ",b:" + doubleToString(blue) + ",a:" + doubleToString(alpha) + ")";
}

Point::Point() {

}

Point::Point(double value) {
    this->x = this->y = this->z = value;
}

Point::Point(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

string Point::toString() {
    return "(" + doubleToString(x) + "," + doubleToString(y) + "," + doubleToString(z) + ")";
}

Point& Point::operator= (const Point &another) {
    this->x = another.x;
    this->y = another.y;
    this->z = another.z;
    this->objectIndex = another.objectIndex;
    this->planeIndex = another.planeIndex;
    return *this;
}

bool Point::operator== (const Point &another) {
    return abs(this->x - another.x) < DOUBLE_ERROR_RANGE && abs(this->y - another.y) < DOUBLE_ERROR_RANGE && abs(this->z - another.z) < DOUBLE_ERROR_RANGE;
}

bool Point::operator!= (const Point &another) {
    return !(*this == another);
}

Point Point::operator+ (const Point &another) {
    Point result = *this;
    result.x += another.x;
    result.y += another.y;
    result.z += another.z;
    return result;
}

Point Point::operator- (const Point &another) {
    Point result = *this;
    result.x -= another.x;
    result.y -= another.y;
    result.z -= another.z;
    return result;
}

Point Point::operator* (const double &another) {
    Point result = *this;
    result.x *= another;
    result.y *= another;
    result.z *= another;
    return result;
}

Point Point::operator/ (const double &another) {
    Point result = *this;
    result.x /= another;
    result.y /= another;
    result.z /= another;
    return result;
}

Point& Point::operator+= (const Point &another) {
    this->x += another.x;
    this->y += another.y;
    this->z += another.z;
    return *this;
}

Point& Point::operator-= (const Point &another) {
    this->x -= another.x;
    this->y -= another.y;
    this->z -= another.z;
    return *this;
}

Point& Point::operator*= (const double &another) {
    this->x *= another;
    this->y *= another;
    this->z *= another;
    return *this;
}

Point& Point::operator/= (const double &another) {
    this->x /= another;
    this->y /= another;
    this->z /= another;
    return *this;
}

double Point::getCoord(int index) {
    switch (index) {
    case Coord::x:
        return x;
    case Coord::y:
        return y;
    case Coord::z:
        return z;
    default:
        return INFINITY;
    }
}

Point Point::getClosestPoint(vector<Point> points) {
    if (points.size() == 0) {
        return *this;
    }

    int minDinstancePointIndex = 0;
    double minD = pointDistance(points[0], *this);
    for (int intersectPointIndex = 1; intersectPointIndex < points.size(); ++intersectPointIndex) {
        double nowD = pointDistance(points[intersectPointIndex], *this);
        if (minD > nowD) {
            minDinstancePointIndex = intersectPointIndex;
            minD = nowD;
        }
    }
    return points[minDinstancePointIndex];
}

double pointDistance(Point left, Point right) {
    return sqrt(pow(left.x - right.x, 2) + pow(left.y - right.y, 2) + pow(left.z - right.z, 2));
}

ColorPoint::ColorPoint() {

}

ColorPoint::ColorPoint(double x, double y, double z, Color color) : Point(x, y, z) {
    this->color = color;
}

string ColorPoint::toString() {
    return Point::toString() + color.toString();
}
