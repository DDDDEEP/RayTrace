#include "stdafx.h"
using namespace std;

Ray::Ray(Point start, Point end) {
    P = start;
    D = Vector(start, end);
    D.unit();
}

Ray::Ray(Point point, Vector vector) {
    P = point;
    D = vector;
    D.unit();
}

string Ray::toString() {
    return "(" + doubleToString(P.x) + "," + doubleToString(P.y) + "," + doubleToString(P.z) + ")+=" +
        "(" + doubleToString(D.point.x) + "," + doubleToString(D.point.y) + "," + doubleToString(D.point.z) + ")";
}