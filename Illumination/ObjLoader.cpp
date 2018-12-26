#include "stdafx.h"
using namespace std;

Material::Material() {
    
}

// 漫反射率, 环境泛光反射率, 泛光亮度, 表面的镜面反射率, 表面的透射率, 材料的折射率, 材料的颜色
Material::Material(double Kd, double Ka, double Ia_Ie_rate, double Ks, double Kt, double Nt, Color color) {
    this->Kd = Kd;
    this->Ka = Ka;
    this->Ia_Ie_rate = Ia_Ie_rate;
    this->Ks = Ks;
    this->Kt = Kt;
    this->Nt = Nt;
    this->color = color;
}

Material& Material::operator= (const Material &another) {
    this->Kd = another.Kd;
    this->Ka = another.Ka;
    this->Ia_Ie_rate = another.Ia_Ie_rate;
    this->Ks = another.Ks;
    this->Kt = another.Kt;
    this->Nt = another.Nt;
    this->color = another.color;
    return *this;
}

void Material::print() {
    cout << "漫反射率 Kd: " << Kd << endl;
    cout << "环境泛光反射率 Ka: " << Ka << endl;
    cout << "泛光亮度 Ia_Ie_rate: " << Ia_Ie_rate << endl;
    cout << "表面的镜面反射率 Ks: " << Ks << endl;
    cout << "表面的透射率 Kt: " << Kt << endl;
    cout << "材料的折射率 Nt: " << Nt << endl;
    cout << "材料的颜色 Color: " << color.toString() << endl;
}

ObjLoader::ObjLoader() {

}

ObjLoader::ObjLoader(string filename, Material material)
{
    this->material = material;

    string line;
    int lineIndex = 0;
    fstream f;
    f.open(filename, ios::in);
    if (!f.is_open()) {
        cout << "Can not open file" << endl;
        return;
    }
    
    while (!f.eof()) {
        // 读入行并保存每行的参数值
        getline(f, line);
        ++lineIndex;
        vector<string> params; // 该行对应的参数
        string tailMark = " ";
        string paramItemStr = ""; // 坐标字符串
        line = line.append(tailMark); // 保证末尾带有空格
        if (line.length() > 0 && line[0] == '/') {
            continue;
        }
        for (int i = 0; i < line.length(); i++) {
            char ch = line[i];
            if (ch != ' ') {    // 若读入字符不为空格，则添加至坐标字符串
                paramItemStr += ch;
            }
            else {  // 若读入字符为空格，则将当前坐标字符串推入集合并置空
                params.push_back(paramItemStr);
                paramItemStr = "";
            }
        }

        // 根据参数值推入v、f集合中
        if (params.size() != 4) {
            cout << lineIndex++ << ": this line size is not equal 4" << endl;
        }
        else {
            if (params[0] == "v") {   // 如果是顶点的话，存放顶点坐标
                Point vertex(atof(params[1].c_str()), atof(params[2].c_str()), atof(params[3].c_str()));
                vSets.push_back(vertex);
                this->gravity += vertex;
            }
            else if (params[0] == "f") {   // 如果是面的话，存放三个顶点的索引
                vector<int> pointIndexs;
                vector<Point> points;
                for (int i = 1; i < 4; i++) {
                    string fParamItem = params[i];
                    string vertexIndexStr = "";
                    for (int j = 0; j < fParamItem.length(); j++) {   // 添加字符直至遇到'/'，以获得点索引
                        char ch = fParamItem[j];
                        if (ch != '/') {
                            vertexIndexStr += ch;
                        }
                        else {
                            break;
                        }
                    }
                    int vertexIndex = atof(vertexIndexStr.c_str()) - 1; // 因为顶点索引在obj文件中是从1开始的，因此要减1
                    pointIndexs.push_back(vertexIndex);
                    points.push_back(vSets[vertexIndex]);
                }
                fSets.push_back(pointIndexs);
                planes.push_back(Plane(points));
            }
        }
    }
    f.close();

    this->gravity /= vSets.size();

    //// 使每个平面的法向量均指向三维模型外面, 假设重心均处于模型内部
    //// 将平面上某点沿当前法向量增加1单位得到的点, 代入平面方程, 再将重心代入平面方程, 若结果同号, 则应对法向量取反
    //for (int planeIndex = 0; planeIndex < planes.size(); ++planeIndex) {
    //    Point tempPoint = planes[planeIndex].points[0] + planes[planeIndex].n.point;
    //    double result1 = planes[planeIndex].A * tempPoint.x + planes[planeIndex].B * tempPoint.y
    //        + planes[planeIndex].C * tempPoint.z + planes[planeIndex].D;
    //    double result2 = planes[planeIndex].A * gravity.x + planes[planeIndex].B * gravity.y
    //        + planes[planeIndex].C * gravity.z + planes[planeIndex].D;
    //    if ((result1 > 0 && result2 > 0) || (result1 < 0 && result2 < 0)) {
    //        planes[planeIndex].n.point *= -1;
    //    }
    //}
    print();
}

void ObjLoader::print() {
    for (int i = 0; i < vSets.size(); ++i) {
        cout << "v" << i + 1 << ": " << vSets[i].x << " " << vSets[i].y << " " << vSets[i].z << endl;
    }
    for (int i = 0; i < fSets.size(); ++i) {
        if (fSets[i].size() < 3) {
            continue;
        }
        cout << "f" << i + 1 << ": " << fSets[i][Coord::x] + 1 << " " << fSets[i][Coord::y] + 1 << " " << fSets[i][Coord::z] + 1 << endl;
    }
    for (int i = 0; i < planes.size(); ++i) {
        if (planes[i].points.size() < 3) {
            continue;
        }
        cout << "fn" << i + 1 << ": " <<
            planes[i].n.point.x << " " << planes[i].n.point.y << " " << planes[i].n.point.z << endl;
    }
}
