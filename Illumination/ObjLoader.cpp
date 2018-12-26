#include "stdafx.h"
using namespace std;

Material::Material() {
    
}

// ��������, �������ⷴ����, ��������, ����ľ��淴����, �����͸����, ���ϵ�������, ���ϵ���ɫ
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
    cout << "�������� Kd: " << Kd << endl;
    cout << "�������ⷴ���� Ka: " << Ka << endl;
    cout << "�������� Ia_Ie_rate: " << Ia_Ie_rate << endl;
    cout << "����ľ��淴���� Ks: " << Ks << endl;
    cout << "�����͸���� Kt: " << Kt << endl;
    cout << "���ϵ������� Nt: " << Nt << endl;
    cout << "���ϵ���ɫ Color: " << color.toString() << endl;
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
        // �����в�����ÿ�еĲ���ֵ
        getline(f, line);
        ++lineIndex;
        vector<string> params; // ���ж�Ӧ�Ĳ���
        string tailMark = " ";
        string paramItemStr = ""; // �����ַ���
        line = line.append(tailMark); // ��֤ĩβ���пո�
        if (line.length() > 0 && line[0] == '/') {
            continue;
        }
        for (int i = 0; i < line.length(); i++) {
            char ch = line[i];
            if (ch != ' ') {    // �������ַ���Ϊ�ո�������������ַ���
                paramItemStr += ch;
            }
            else {  // �������ַ�Ϊ�ո��򽫵�ǰ�����ַ������뼯�ϲ��ÿ�
                params.push_back(paramItemStr);
                paramItemStr = "";
            }
        }

        // ���ݲ���ֵ����v��f������
        if (params.size() != 4) {
            cout << lineIndex++ << ": this line size is not equal 4" << endl;
        }
        else {
            if (params[0] == "v") {   // ����Ƕ���Ļ�����Ŷ�������
                Point vertex(atof(params[1].c_str()), atof(params[2].c_str()), atof(params[3].c_str()));
                vSets.push_back(vertex);
                this->gravity += vertex;
            }
            else if (params[0] == "f") {   // �������Ļ�������������������
                vector<int> pointIndexs;
                vector<Point> points;
                for (int i = 1; i < 4; i++) {
                    string fParamItem = params[i];
                    string vertexIndexStr = "";
                    for (int j = 0; j < fParamItem.length(); j++) {   // ����ַ�ֱ������'/'���Ի�õ�����
                        char ch = fParamItem[j];
                        if (ch != '/') {
                            vertexIndexStr += ch;
                        }
                        else {
                            break;
                        }
                    }
                    int vertexIndex = atof(vertexIndexStr.c_str()) - 1; // ��Ϊ����������obj�ļ����Ǵ�1��ʼ�ģ����Ҫ��1
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

    //// ʹÿ��ƽ��ķ�������ָ����άģ������, �������ľ�����ģ���ڲ�
    //// ��ƽ����ĳ���ص�ǰ����������1��λ�õ��ĵ�, ����ƽ�淽��, �ٽ����Ĵ���ƽ�淽��, �����ͬ��, ��Ӧ�Է�����ȡ��
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
