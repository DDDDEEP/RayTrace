#ifndef HEADER_RAY
#define HEADER_RAY
#include "Point.h"
#include "Constant.h"
#include "Vector.h"
#include <string>
using namespace std;

class Point;

// ����
class Ray {
public:
    Point P; // ��ʼ��
    Vector D; // ���߷���
    double t;
    
    Ray(Point start, Point end);
    Ray(Point point, Vector vector);
    string toString();
};
#endif