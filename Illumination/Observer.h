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

// 观察者
class Observer {
public:
    int widthPixelCount, heightPixelCount; // 像素个数
    double pixelSize, widthSize, heightSize, distance; // 单位像素的大小, 观察面大小
    double widthStart, widthEnd, heightStart, heightEnd; // 观察面边界
    Point camera; // 视点
    Point lightSource; // 光源
    vector<vector<ColorPoint> > window; // 视窗口像素数组
    vector<ObjLoader> objects;

    Observer(int widthPixelCount, int heightPixelCount, double pixelSize, double distance, Point camera, Point lightSource, vector<string> objetcPaths, vector<Material> materials);
    void rotateWindowToCamera(); // 旋转视窗口, 使视窗口中心与摄像机与原点处于同一直线上
    void rayTrace( Ray R, double ratio, int depth, Color &color, bool inAir = true); // 修改color的值
    vector<Point> getAllIntersectPointByRay(Ray ray); // 获得光线与所有平面的交点
    void print();
};
#endif
