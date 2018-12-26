#ifndef HEADER_OBSERVER
#define HEADER_OBSERVER
#include "Constant.h"
#include "Point.h"
#include "ObjLoader.h"
#include <vector>
using namespace std;

class Point;
class Material;
class ObjLoader;

// �۲���
class Observer {
public:
    int widthPixelCount, heightPixelCount; // ���ظ���
    double pixelSize, widthSize, heightSize, distance; // ��λ���صĴ�С, �۲����С
    double widthStart, widthEnd, heightStart, heightEnd; // �۲���߽�
    Point camera; // �ӵ�
    Point lightSource; // ��Դ
    vector<vector<ColorPoint> > window; // �Ӵ�����������
    vector<ObjLoader> objects;

    Observer(int widthPixelCount, int heightPixelCount, double pixelSize, double distance, Point camera, Point lightSource, vector<string> objetcPaths, vector<Material> materials);
    void rotateWindowToCamera(); // ��ת�Ӵ���, ʹ�Ӵ����������������ԭ�㴦��ͬһֱ����
    void rayTrace( Ray R, double ratio, int depth, Color &color, bool inAir = true); // �޸�color��ֵ
    vector<Point> getAllIntersectPointByRay(Ray ray); // ��ù���������ƽ��Ľ���
    void print();
};
#endif
