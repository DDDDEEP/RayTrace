#include "stdafx.h"
using namespace std;

Vector::Vector() {

}

Vector::Vector(Point point) {
    this->point = point;
}

Vector::Vector(Point start, Point end) {
    this->point = end - start;
}

Vector Vector::operator+ (const Vector &another) {
    Vector result = *this;
    result.point += another.point;
    return result;
}

Vector Vector::operator- (const Vector &another) {
    Vector result = *this;
    result.point -= another.point;
    return result;
}

Vector Vector::operator* (const double &another) {
    Vector result = *this;
    result.point *= another;
    return result;
}

Vector Vector::operator/ (const double &another) {
    Vector result = *this;
    result.point /= another;
    return result;
}

double Vector::operator*(const Vector &another) {
    double result = 0;
    result += this->point.x * another.point.x;
    result += this->point.y * another.point.y;
    result += this->point.z * another.point.z;
    return result;
}


Vector Vector::x(Vector another) {
    // | i j k |
    // | a b c |
    // | x y z |
    // = (bz - cy, cx - az, ay - bx)
    Point result;
    result.x = this->point.y * another.point.z - this->point.z * another.point.y;
    result.y = this->point.z * another.point.x - this->point.x * another.point.z;
    result.z = this->point.x * another.point.y - this->point.y * another.point.x;
    return Vector(result);
}

void Vector::unit() {
    double D = sqrt(pow(this->point.x, 2) + pow(this->point.y, 2) + pow(this->point.z, 2));
    this->point /= D;
}

string Vector::toString() {
    return "+=(" + doubleToString(point.x) + "," + doubleToString(point.y) + "," + doubleToString(point.z) + ")";
}

double cos(Vector vector1, Vector vector2) {
    return vector1 * vector2 / (sqrt(vector1 * vector1) * sqrt(vector2 * vector2));
}
