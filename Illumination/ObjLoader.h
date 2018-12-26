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

// 材料参数类
class Material {
public:
    double Kd = 1; // 漫反射率
    double Ka = 0; // 环境泛光反射率
    double Ia_Ie_rate = 0; // 泛光亮度, 即Ia/Ie的比例, 一般取0.02~0.2
    double Ks = 0; // 表面的镜面反射率
    double Kt = 0; // 表面的透射率
    double Nt = 1; // 材料的折射率
    Color color;

    Material();
    Material(double Kd, double Ka, double Ia_Ie_rate, double Ks, double Kt, double Nt, Color color);
    Material& operator= (const Material &another);
    void print();
};

// obj加载器
class ObjLoader {
public:
    vector<Point> vSets; // 存放顶点对象
    vector<vector<int> > fSets; // 存放面对应的顶点索引, 索引值从0开始, 应使用枚举值Coord获取
    vector<Plane> planes; // 存放面对象
    Point gravity; // 重心
    Material material; // 材质

    ObjLoader();
    ObjLoader(string filename, Material material);
    void print();
};
#endif