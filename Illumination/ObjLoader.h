#ifndef HEADER_OBJLOADER
#define HEADER_OBJLOADER
#include "Constant.h"
#include "Point.h"
#include "Plane.h"
#include <vector>
#include <string>
using namespace std;

class Point;
class Plane;

// ���ϲ�����
class Material {
public:
    double Kd = 1; // ��������
    double Ka = 0; // �������ⷴ����
    double Ia_Ie_rate = 0; // ��������, ��Ia/Ie�ı���, һ��ȡ0.02~0.2
    double Ks = 0; // ����ľ��淴����
    double Kt = 0; // �����͸����
    double Nt = 1; // ���ϵ�������
    Color color;

    Material();
    Material(double Kd, double Ka, double Ia_Ie_rate, double Ks, double Kt, double Nt, Color color);
    Material& operator= (const Material &another);
    void print();
};

// obj������
class ObjLoader {
public:
    vector<Point> vSets; // ��Ŷ������
    vector<vector<int> > fSets; // ������Ӧ�Ķ�������, ����ֵ��0��ʼ, Ӧʹ��ö��ֵCoord��ȡ
    vector<Plane> planes; // ��������
    Point gravity; // ����
    Material material; // ����

    ObjLoader();
    ObjLoader(string filename, Material material);
    void print();
};
#endif