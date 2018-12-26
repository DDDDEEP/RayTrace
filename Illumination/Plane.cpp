#include "stdafx.h"
using namespace std;

Plane::Plane() {

}

Plane::Plane(vector<Point> points) {
    this->points = points;

    // 计算法向量
    if (points.size() < 3) {
        return;
    }

    Point firstPoint = points[0];
    Point secondPoint = points[1];
    Point thirdPoint = points[2];

    // 计算法向量：求出两边的向量，对向量进行叉乘，并作令z方向绝对值为1的单位化处理
    Vector vector1 = Vector(secondPoint, firstPoint),
        vector2 = Vector(thirdPoint, firstPoint);
    this->n = vector1.x(vector2);
    this->n.unit();

    this->A = n.point.x;
    this->B = n.point.y;
    this->C = n.point.z;
    this->D = -A * points[0].x - B * points[0].y - C * points[0].z;
}

string Plane::toString() {
    return doubleToString(A) + "x+" + doubleToString(B) + "y+" + doubleToString(C) + "z+" + doubleToString(D) + "=0";
}

Point Plane::computeGravity()
{
	double xAvg = 0.0, yAvg = 0.0, zAvg = 0.0;
	for (int k = 0; k < points.size(); ++k)
	{
		xAvg += points[k].x;
		yAvg += points[k].y;
		zAvg += points[k].z;
	}

	xAvg /= points.size();
	yAvg /= points.size();
	zAvg /= points.size();

	return Point(xAvg, yAvg, zAvg);

}



Point Plane::getIntersectPointByRay(Ray ray) {
    double dividend = ray.D * n;

    if (dividend == 0) {
        return INFINITY_POINT;
    }
    else {
        // t = n * (Pn - P) / (D * n)
        double t = n * Vector(points[0] - ray.P) / dividend;
        if (t <= DOUBLE_ERROR_RANGE) {
            return INFINITY_POINT;
        }
        else {
            return ray.P + ray.D.point * t;
        }
    }

}

bool Plane::isInside(Point point) {
    // 判断点是否于平面内，为方便暂时只实现三角形平面的判断
    // 使用同向法: 求投影后的三条边对应的方程, 然后代入对应的第三点与被判点求出两个D, 判断两个D是否同号，对三条边均进行计算
    Point Pa = points[0];
    Point Pb = points[1];
    Point Pc = points[2];
    double Dp, Dthird, divided1, divided2;
    int coord1, coord2;
    if (abs(A) >= max(abs(B), abs(C))) {
        // yz
        coord1 = Coord::y;
        coord2 = Coord::z;
    }
    else if (abs(B) >= max(abs(A), abs(C))) {
        // xz
        coord1 = Coord::z;
        coord2 = Coord::x;
    }
    else {
        // xy
        coord1 = Coord::x;
        coord2 = Coord::y;
    }

    divided1 = (Pb.getCoord(coord1) - Pc.getCoord(coord1));
    divided2 = (Pb.getCoord(coord2) - Pc.getCoord(coord2));
    Dp = (point.getCoord(coord1) - Pc.getCoord(coord1)) / divided1
        - (point.getCoord(coord2) - Pc.getCoord(coord2)) / divided2;
    Dthird = (Pa.getCoord(coord1) - Pc.getCoord(coord1)) / divided1
        - (Pa.getCoord(coord2) - Pc.getCoord(coord2)) / divided2;
    if (!((Dp >= 0 && Dthird >= 0) || (Dp <= 0 && Dthird <= 0))) {
        return false;
    }

    divided1 = (Pc.getCoord(coord1) - Pa.getCoord(coord1));
    divided2 = (Pc.getCoord(coord2) - Pa.getCoord(coord2));
    Dp = (point.getCoord(coord1) - Pa.getCoord(coord1)) / divided1
        - (point.getCoord(coord2) - Pa.getCoord(coord2)) / divided2;
    Dthird = (Pb.getCoord(coord1) - Pa.getCoord(coord1)) / divided1
        - (Pb.getCoord(coord2) - Pa.getCoord(coord2)) / divided2;
    if (!((Dp >= 0 && Dthird >= 0) || (Dp <= 0 && Dthird <= 0))) {
        return false;
    }

    divided1 = (Pa.getCoord(coord1) - Pb.getCoord(coord1));
    divided2 = (Pa.getCoord(coord2) - Pb.getCoord(coord2));
    Dp = (point.getCoord(coord1) - Pb.getCoord(coord1)) / divided1
        - (point.getCoord(coord2) - Pb.getCoord(coord2)) / divided2;
    Dthird = (Pc.getCoord(coord1) - Pb.getCoord(coord1)) / divided1
        - (Pc.getCoord(coord2) - Pb.getCoord(coord2)) / divided2;
    if (!((Dp >= 0 && Dthird >= 0) || (Dp <= 0 && Dthird <= 0))) {
        return false;
    }
    return true;

}