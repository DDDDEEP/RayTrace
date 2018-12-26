#ifndef HEADER_CONSTANT
#define HEADER_CONSTANT
#include <vector>
#include <string>
#include "Point.h"
#include "ObjLoader.h"
using namespace std;

/*
* ����
*/
const Color BLACK(0, 0, 0, 1);
const Color WHITE(1, 1, 1, 1);
const Color LIGHT_BLUE(0.1215, 0.8, 0.7, 1);
const Color LIGHT_RED(1, 0.8, 0.3, 1);
const Point INFINITY_POINT(INT_MAX);
const double DOUBLE_ERROR_RANGE = 1e-14; // ��������ݴ���
const double N_AIR = 1; // ����������

/*
* �������뺯��
*/
enum Coord { x = 0, y = 1, z = 2 };
string doubleToString(double num);

/*
* �Ӵ����������
*/
const double WIDTH = 2, HEIGHT = 2, DISTANCE = 2; // �۲����xy����Ĵ�С
const int PIXEL_COUNT_PER_SIZE = 300; // ÿ��λ���ȵ����ظ���

/*
* ��Դ���������س���
*/
const double THRESHOLD = 0.50; // ����׷����С����ָ��
const int MAXDEPTH = 15; // ����׷�����ݹ����
const Point CAMERA(2, 2, 4); // �ӵ�����
const Point LIGHT_SOURCE(2, 2, 1); // ��Դ


/*
* �������������
*/
// ��������, �������ⷴ����, ��������, ����ľ��淴����, �����͸����, ���ϵ�������, ���ϵ���ɫ
const Material OBJECT_MATERIAL = Material(0.8, 0.3, 0.2, 0.5, 0.8, 1.3, LIGHT_BLUE); // �������
const Material OBJECT2_MATERIAL = Material(0.8, 0.3, 0.2, 0.5, 0.65, 1.3, LIGHT_RED);
const Material MIRROR_MATERIAL = Material(0.8, 0.3, 0.2, 0.5, 0, 1.3, WHITE); // �������

//const Material OBJECT_MATERIAL = Material(0.8, 0.3, 0.2, 0, 0.8, 1.3, LIGHT_BLUE); // �������
//const Material OBJECT2_MATERIAL = Material(0.8, 0.3, 0.2, 0, 0.65, 1.3, LIGHT_RED);
//const Material MIRROR_MATERIAL = Material(0.8, 0.3, 0.2, 0, 0, 1.3, WHITE); // �������

const vector<string> OBJECT_PATHS = { "data/pyramid.obj", "data/cube.obj", "data/ground.obj" }; // obj�ļ�·��
const vector<Material> MATERIALS = { OBJECT_MATERIAL, OBJECT2_MATERIAL, MIRROR_MATERIAL }; // ��Ӧ�Ĳ���
#endif
