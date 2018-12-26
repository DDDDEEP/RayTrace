#include "stdafx.h"
using namespace std;

Observer::Observer(int xPixelCount, int yPixelCount, double pixelSize, double zSize, Point camera, Point lightSource, vector<string> objectPaths, vector<Material> materials) {
    this->pixelSize = pixelSize;
    this->widthPixelCount = xPixelCount;
    this->heightPixelCount = yPixelCount;
    this->widthSize = pixelSize * xPixelCount;
    this->heightSize = pixelSize * yPixelCount;
    this->distance = zSize;
    this->camera = camera;
    this->lightSource = lightSource;
    this->objects = objects;

    this->widthStart = -widthSize / 2;
    this->heightStart = -heightSize / 2;
    this->widthEnd = -widthStart;
    this->heightEnd = -heightStart;
    for (int pathIndex = 0; pathIndex < objectPaths.size(); ++pathIndex) {
        Material material = (pathIndex + 1 > materials.size()) ? materials[materials.size() - 1] : materials[pathIndex];
        objects.push_back(ObjLoader(objectPaths[pathIndex], material));
    }

    print();

    // ��ת�Ӵ���, ʹ�Ӵ����������������ԭ�㴦��ͬһֱ����
    rotateWindowToCamera();

    // ����׷��
    for (int widthIndex = 0; widthIndex < window.size(); ++widthIndex) {
        for (int heightIndex = 0; heightIndex < window[widthIndex].size(); ++heightIndex) {
            Ray ray(camera, window[widthIndex][heightIndex]);
            double ratio = 1; // ����˥��ϵ��
            int depth = 1; // �ݹ����
            rayTrace(ray, ratio, depth, window[widthIndex][heightIndex].color);
        }
    }
}

void Observer::rayTrace(Ray R, double ratio, int depth, Color &color, bool inAir) {
    if (ratio < THRESHOLD || depth > MAXDEPTH) {
        return;
    }
    Color local_color, reflected_color, transmitted_color;

    /*
    * 1.��ȡ�������������н�����, ����������Ľ���
    */
    vector<Point> intersectPoints = getAllIntersectPointByRay(R);
    if (intersectPoints.size() == 0) {
        return;
    }
    Point closestPointBySight = R.P.getClosestPoint(intersectPoints);

    /*
    * 2.��ȡ ����(��Դ->����������Ľ���)����������н�����, ���Դ����Ľ���
    */
    Ray light(lightSource, closestPointBySight);
    vector<Point> intersectPointsByLight = getAllIntersectPointByRay(light);
    Point intersectPointByLight = lightSource.getClosestPoint(intersectPointsByLight);
    Material material = objects[closestPointBySight.objectIndex].material; // �����Ӧ������Ĳ���

    /*
    * 3.����ֲ�����:
    * �ж�N(����������Ľ����Ӧ��ƽ��ķ�����)��L(����������Ľ���->��Դ)
    * ��cosֵ�Ƿ�Ϊ��, ���Ƿ�Ϊ���Դ����Ľ���
    * Ϊ��,����,��Ϊ��Ч�ֲ�����
    */
    Vector localN = objects[closestPointBySight.objectIndex].planes[closestPointBySight.planeIndex].n, localL(closestPointBySight, lightSource),
        localV(closestPointBySight, R.P); // V(����������Ľ���->����)
    localL.unit();
    localV.unit();
    // cosa = N * L / |N| * |L|, N, L�Ѿ���λ��, ������
    double cosa = localN * localL;
    if (cosa >= 0 && intersectPointByLight == closestPointBySight) {
        // ���������ǿ��
        // Id = Ie * Kd * cosa
        local_color = material.color * material.Ka * material.Ia_Ie_rate + material.color * material.Kd * cosa;
    }
    else {
        local_color = material.color * material.Ka * material.Ia_Ie_rate;
    }

    /*
    * 4.������������
    */
    if (material.Kd != 0) {
        // ����� Rr = L - 2 * (L * N) * N
        Vector reflectedN = localN, reflectedL = localV * -1;
        Vector Rr = reflectedL - reflectedN * 2.0 * (reflectedL * reflectedN);
        Ray reflectedRay(closestPointBySight, Rr);
        rayTrace(reflectedRay, ratio * material.Kd, depth + 1, reflected_color);
    }
    if (material.Kt != 0) {
        // ����� Rt = L / e + N * (cosO1 / e - cosO2),
        // e = sinO1/sinO2 = n2/n1, cosO1 = -N * L, cosO2 = sqrt(1 - (1 - cosO1 * cosO1 / e * e))
        Vector transmissionN = localN, transmissionL = localV * -1;
        double n1 = inAir ? N_AIR : material.Nt, n2 = !inAir ? N_AIR : material.Nt;
        double e = n2 / n1;
        double cosO1 = transmissionN * -1 * transmissionL;
        double cosO2 = sqrt(1 - (1 - pow(cosO1, 2) / pow(e, 2)));
        Vector Rt = transmissionL / e + transmissionN * (cosO1 / e - cosO2);
        Ray transmissionRay(closestPointBySight, Rt);
        rayTrace(transmissionRay, ratio * material.Kt, depth + 1, transmitted_color);
    }

    color += local_color + reflected_color * material.Ks + transmitted_color * material.Kt;
}

void Observer::print() {
    cout << "���ش�С: " << pixelSize << endl;
    cout << "���: " << doubleToString(widthStart) << "~" << doubleToString(widthEnd) << ", ���������: " << widthPixelCount << endl;
    cout << "�߶�: " << doubleToString(heightStart) << "~" << doubleToString(heightEnd) << ", �߶�������: " << heightPixelCount << endl;
    cout << "����: " << doubleToString(distance) << endl;
    cout << "�����: " << camera.toString() << endl;
    cout << "��Դ: " << lightSource.toString() << endl;
}

// ��ת�Ӵ���, ʹ�Ӵ����������������ԭ�㴦��ͬһֱ����
void Observer::rotateWindowToCamera() {
    // �����߽����
    vector<vector<double> > matrix1(3, vector<double>(3, 0)), matrix2(3, vector<double>(3, 0));
    bool needMatrxi2 = false;
    if (camera.x == 0 && camera.y == 0 && camera.z == 0) {

    }
    else if (camera.x == 0 && camera.y == 0 && camera.z != 0) { // ��x == 0
        if (camera.z > 0) {
            matrix1[0][0] = matrix1[1][1] = matrix1[2][2] = 1;
        }
        else {
            matrix1[0][0] = matrix1[2][2] = -1;
            matrix1[1][1] = 1;
        }
    }
    else if (camera.x == 0 && camera.y != 0 && camera.z == 0) {
        if (camera.y > 0) {
            matrix1[0][0] = matrix1[1][2] = 1;
            matrix1[2][1] = -1;
        }
        else {
            matrix1[0][0] = matrix1[2][1] = 1;
            matrix1[1][2] = -1;
        }
    }
    else if (camera.x == 0 && camera.y != 0 && camera.z != 0) {
        Point Pt, Pm;
        Pm.y = distance / sqrt(1 + pow(camera.z, 2) / pow(camera.y, 2));
        Pm.y *= camera.y > 0 ? 1 : -1;
        Pm.z = camera.z / camera.y * Pm.y;

        Pt.z = camera.z > 0 ? distance : -distance;

        double cosO2 = Vector(Pm) * Vector(Pt) / pow(distance, 2);
        double sinO2 = sqrt(1 - pow(cosO2, 2));

        // ��Pm.y < 0,  Ϊ��ʱ�룬����ת��; ������ת��
        if (Pm.y < 0) {
            matrix1[0][0] = 1; matrix1[0][1] = 0; matrix1[0][2] = 0;
            matrix1[1][0] = 0; matrix1[1][1] = cosO2; matrix1[1][2] = -sinO2;
            matrix1[2][0] = 0; matrix1[2][1] = sinO2; matrix1[2][2] = cosO2;
        }
        else {
            matrix1[0][0] = 1; matrix1[0][1] = 0; matrix1[0][2] = 0;
            matrix1[1][0] = 0; matrix1[1][1] = cosO2; matrix1[1][2] = sinO2;
            matrix1[2][0] = 0; matrix1[2][1] = -sinO2; matrix1[2][2] = cosO2;
        }
    }
    else { // ��x ��= 0
           // ������ת��O1(��y��), O2(��x��), ��ʼ�Ӵ�����Pi(nitial), ��תO1����Ӵ�����Pt(emp), �����Ӵ�����Pm(iddle)
        needMatrxi2 = true;
        Point Pi(0, 0, distance), Pt, Pm;
        Pm.x = distance / sqrt(1 + pow(camera.y, 2) / pow(camera.x, 2) + pow(camera.z, 2) / pow(camera.x, 2));
        Pm.x *= camera.x > 0 ? 1 : -1;
        Pm.y = camera.y / camera.x * Pm.x;
        Pm.z = camera.z / camera.x * Pm.x;

        Pt.x = distance / sqrt(1 + pow(camera.z, 2) / pow(camera.x, 2));
        Pt.x *= camera.x > 0 ? 1 : -1;
        Pt.z = camera.z / camera.x * Pt.x;

        double cosO1 = Vector(Pt) * Vector(Pi) / pow(distance, 2), cosO2 = Vector(Pm) * Vector(Pt) / pow(distance, 2);
        double sinO1 = sqrt(1 - pow(cosO1, 2)), sinO2 = sqrt(1 - pow(cosO2, 2));
        cout << "sinO1: " << sinO1 << ", cosO1: " << cosO1 << endl;
        cout << "sinO2: " << sinO2 << ", cosO2: " << cosO2 << endl;

        // ��Pt.x > 0 (Pm.y < 0),  Ϊ��ʱ�룬����ת��; ������ת��
        if (Pt.x > 0) {
            matrix1[0][0] = cosO1; matrix1[0][1] = 0; matrix1[0][2] = sinO1;
            matrix1[1][0] = 0; matrix1[1][1] = 1; matrix1[1][2] = 0;
            matrix1[2][0] = -sinO1; matrix1[2][1] = 0; matrix1[2][2] = cosO1;
        }
        else if (Pt.x < 0) {
            matrix1[0][0] = cosO1; matrix1[0][1] = 0; matrix1[0][2] = -sinO1;
            matrix1[1][0] = 0; matrix1[1][1] = 1; matrix1[1][2] = 0;
            matrix1[2][0] = sinO1; matrix1[2][1] = 0; matrix1[2][2] = cosO1;
        }
        // ��L(ԭ��->Pt)��ֱ��V(x,y,z), Vָ��L���ұ�
        // |xx(1-c)+c, xy(1-c)-zs, xz(1-c)+ys|
        // |yx(1-c)+zs, yy(1-c)+c, yz(1-c)-xs|
        // |xz(1-c)-ys, yz(1-c)+xs, zz(1-c)+c|
        Point V(Pt.z / 2, 0, -Pt.x / 2);
        if (Pm.y > 0) {
            sinO2 *= -1;
        }
        matrix2[0][0] = pow(V.x, 2) * (1 - cosO2) + cosO2; matrix2[0][1] = V.x * V.y * (1 - cosO2) - V.z * sinO2; matrix2[0][2] = V.x * V.z * (1 - cosO2) + V.y * sinO2;
        matrix2[1][0] = V.y * V.x * (1 - cosO2) + V.z * sinO2; matrix2[1][1] = pow(V.y, 2) * (1 - cosO2) + cosO2; matrix2[1][2] = V.y * V.z * (1 - cosO2) - V.x * sinO2;
        matrix2[2][0] = V.x * V.z * (1 - cosO2) - V.y * sinO2; matrix2[2][1] = V.y * V.z * (1 - cosO2) + V.x * sinO2; matrix2[2][2] = pow(V.z, 2) * (1 - cosO2) + cosO2;
    }
    cout << "�Ӵ��ڱ任����:" << endl;
    cout << "|" << matrix1[0][0] << " " << matrix1[0][1] << " " << matrix1[0][2] << "|" << endl;
    cout << "|" << matrix1[1][0] << " " << matrix1[1][1] << " " << matrix1[1][2] << "|" << endl;
    cout << "|" << matrix1[2][0] << " " << matrix1[2][1] << " " << matrix1[2][2] << "|" << endl;
    cout << endl;
    if (needMatrxi2) {
        cout << "|" << matrix2[0][0] << " " << matrix2[0][1] << " " << matrix2[0][2] << "|" << endl;
        cout << "|" << matrix2[1][0] << " " << matrix2[1][1] << " " << matrix2[1][2] << "|" << endl;
        cout << "|" << matrix2[2][0] << " " << matrix2[2][1] << " " << matrix2[2][2] << "|" << endl;
        cout << endl;
    }

    // ��ʼ�����������, �ȳ�ʼ����xyƽ��ƽ�е�����ƽ��
    // ����ÿ�����ض�Ӧ�Ķ��������ȡ�������Ĵ�������xy��������ϰ�����ش�С
    double halfPixelSize = pixelSize / 2;
    double widthIndexSize = widthStart + halfPixelSize;
    for (int widthIndex = 0; widthIndex < widthPixelCount; ++widthIndex) {
        vector<ColorPoint> resultItem;
        double heightIndexSize = heightStart + halfPixelSize;
        for (int heightIndex = 0; heightIndex < heightPixelCount; ++heightIndex) {
            resultItem.push_back(ColorPoint(widthIndexSize, heightIndexSize, distance, Color()));
            heightIndexSize += pixelSize;

            //int testIndex = 299;
            //if (widthIndex == testIndex && heightIndex == testIndex) {
            //    cout << resultItem[widthIndex].toString() << endl;
            //}
            // ��ת�任
            Point rawItem(resultItem.back());
            resultItem.back().x = matrix1[0][0] * rawItem.x + matrix1[0][1] * rawItem.y + matrix1[0][2] * rawItem.z;
            resultItem.back().y = matrix1[1][0] * rawItem.x + matrix1[1][1] * rawItem.y + matrix1[1][2] * rawItem.z;
            resultItem.back().z = matrix1[2][0] * rawItem.x + matrix1[2][1] * rawItem.y + matrix1[2][2] * rawItem.z;
            if (needMatrxi2) {
                Point rawItem(resultItem.back());
                resultItem.back().x = matrix2[0][0] * rawItem.x + matrix2[0][1] * rawItem.y + matrix2[0][2] * rawItem.z;
                resultItem.back().y = matrix2[1][0] * rawItem.x + matrix2[1][1] * rawItem.y + matrix2[1][2] * rawItem.z;
                resultItem.back().z = matrix2[2][0] * rawItem.x + matrix2[2][1] * rawItem.y + matrix2[2][2] * rawItem.z;
            }
            //if (widthIndex == testIndex && heightIndex == testIndex) {
            //    cout << resultItem[widthIndex].toString() << endl;
            //}
        }
        window.push_back(resultItem);
        widthIndexSize += pixelSize;
    }
}

vector<Point> Observer::getAllIntersectPointByRay(Ray ray) {
    vector<Point> intersectPoints;
    for (int objectIndex = 0; objectIndex < objects.size(); ++objectIndex) {
        for (int planeIndex = 0; planeIndex < objects[objectIndex].planes.size(); ++planeIndex) {
            // Ϊ����ٶ�, ֱ���ж�����ƽ��������������������Ĺ�ϵ
            Point point0 = objects[objectIndex].planes[planeIndex].points[0];
            Point point1 = objects[objectIndex].planes[planeIndex].points[1];
            Point point2 = objects[objectIndex].planes[planeIndex].points[2];
            if ((ray.D.point.x > 0 && ray.P.x >= point0.x && ray.P.x >= point1.x && ray.P.x >= point2.x)
                || (ray.D.point.x < 0 && ray.P.x <= point0.x && ray.P.x <= point1.x && ray.P.x <= point2.x)
                || (ray.D.point.y > 0 && ray.P.y >= point0.y && ray.P.y >= point1.y && ray.P.y >= point2.y)
                || (ray.D.point.y < 0 && ray.P.y <= point0.y && ray.P.y <= point1.y && ray.P.y <= point2.y)
                || (ray.D.point.z > 0 && ray.P.z >= point0.z && ray.P.z >= point1.z && ray.P.z >= point2.z)
                || (ray.D.point.z < 0 && ray.P.z <= point0.z && ray.P.z <= point1.z && ray.P.z <= point2.z)) {
                continue;
            }
            Point tempIntersectPoint = objects[objectIndex].planes[planeIndex].getIntersectPointByRay(ray);
            if (tempIntersectPoint != INFINITY_POINT && objects[objectIndex].planes[planeIndex].isInside(tempIntersectPoint)) {
                tempIntersectPoint.objectIndex = objectIndex;
                tempIntersectPoint.planeIndex = planeIndex;
                intersectPoints.push_back(tempIntersectPoint);
            }
        }
    }
    return intersectPoints;
}