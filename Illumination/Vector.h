#ifndef HEADER_VECTOR
#define HEADER_VECTOR
#include "Constant.h"
#include "Point.h"
using namespace std;

class Point;

// ����, Ϊ������㣬��������������ʾ��ʽ��Ϊ��ԭ��ָ��ָ����һ��
class Vector {
public:
    Point point; // ��ԭ��ָ��ĵ�
    
    Vector();
    Vector(Point point);
    Vector(Point start, Point end);
    Vector operator+ (const Vector &another);
    Vector operator- (const Vector &another);
    Vector operator* (const double &another);
    Vector operator/ (const double &another);
    double operator*(const Vector &another); // ���
    Vector x(Vector another); // ���
    void unit(); // ��λ��
    string toString();
};
double cos(Vector vector1, Vector vector2);
#endif