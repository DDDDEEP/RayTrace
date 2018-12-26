#ifndef HEADER_CONSTANT
#define HEADER_CONSTANT
#include <vector>
#include <string>
#include "Point.h"
#include "ObjLoader.h"
using namespace std;

/*
* 常量
*/
const Color BLACK(0, 0, 0, 1);
const Color WHITE(1, 1, 1, 1);
const Color LIGHT_BLUE(0.1215, 0.8, 0.7, 1);
const Color LIGHT_RED(1, 0.8, 0.3, 1);
const Point INFINITY_POINT(INT_MAX);
const double DOUBLE_ERROR_RANGE = 1e-14; // 浮点计算容错率
const double N_AIR = 1; // 空气折射率

/*
* 辅助类与函数
*/
enum Coord { x = 0, y = 1, z = 2 };
string doubleToString(double num);

/*
* 视窗口属性相关
*/
const double WIDTH = 2, HEIGHT = 2, DISTANCE = 2; // 观察面的xy方向的大小
const int PIXEL_COUNT_PER_SIZE = 300; // 每单位长度的像素个数

/*
* 光源与摄像机相关常量
*/
const double THRESHOLD = 0.50; // 光线追踪最小削弱指数
const int MAXDEPTH = 15; // 光线追踪最大递归深度
const Point CAMERA(2, 2, 4); // 视点坐标
const Point LIGHT_SOURCE(2, 2, 1); // 光源


/*
* 材料与物体相关
*/
// 漫反射率, 环境泛光反射率, 泛光亮度, 表面的镜面反射率, 表面的透射率, 材料的折射率, 材料的颜色
const Material OBJECT_MATERIAL = Material(0.8, 0.3, 0.2, 0.5, 0.8, 1.3, LIGHT_BLUE); // 物体材质
const Material OBJECT2_MATERIAL = Material(0.8, 0.3, 0.2, 0.5, 0.65, 1.3, LIGHT_RED);
const Material MIRROR_MATERIAL = Material(0.8, 0.3, 0.2, 0.5, 0, 1.3, WHITE); // 镜面材质

//const Material OBJECT_MATERIAL = Material(0.8, 0.3, 0.2, 0, 0.8, 1.3, LIGHT_BLUE); // 物体材质
//const Material OBJECT2_MATERIAL = Material(0.8, 0.3, 0.2, 0, 0.65, 1.3, LIGHT_RED);
//const Material MIRROR_MATERIAL = Material(0.8, 0.3, 0.2, 0, 0, 1.3, WHITE); // 镜面材质

const vector<string> OBJECT_PATHS = { "data/pyramid.obj", "data/cube.obj", "data/ground.obj" }; // obj文件路径
const vector<Material> MATERIALS = { OBJECT_MATERIAL, OBJECT2_MATERIAL, MIRROR_MATERIAL }; // 对应的材质
#endif
