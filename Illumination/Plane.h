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

// ƽ��
class Plane {
public:
    vector<Point> points;
    Vector n; // ������
    double A, B, C, D; // ƽ�淽��ϵ��

    Plane();
    Plane(vector<Point> points);
    Point getIntersectPointByRay(Ray ray); // ��ù�����ƽ��Ľ���, ��Ϊ��Ч���㷵�����ֵ��
    bool isInside(Point point); // �жϵ��Ƿ�������ƽ����
    string toString();

	Point computeGravity();
	

};
#endif
