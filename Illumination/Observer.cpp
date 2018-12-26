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

    // 旋转视窗口, 使视窗口中心与摄像机与原点处于同一直线上
    rotateWindowToCamera();

    // 光线追踪
    for (int widthIndex = 0; widthIndex < window.size(); ++widthIndex) {
        for (int heightIndex = 0; heightIndex < window[widthIndex].size(); ++heightIndex) {
            Ray ray(camera, window[widthIndex][heightIndex]);
            double ratio = 1; // 光线衰减系数
            int depth = 1; // 递归深度
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
    * 1.获取视线与物体所有交点中, 离视线最近的交点
    */
    vector<Point> intersectPoints = getAllIntersectPointByRay(R);
    if (intersectPoints.size() == 0) {
        return;
    }
    Point closestPointBySight = R.P.getClosestPoint(intersectPoints);

    /*
    * 2.获取 向量(光源->离视线最近的交点)与物体的所有交点中, 离光源最近的交点
    */
    Ray light(lightSource, closestPointBySight);
    vector<Point> intersectPointsByLight = getAllIntersectPointByRay(light);
    Point intersectPointByLight = lightSource.getClosestPoint(intersectPointsByLight);
    Material material = objects[closestPointBySight.objectIndex].material; // 交点对应的物体的材质

    /*
    * 3.计算局部光照:
    * 判断N(离视线最近的交点对应的平面的法向量)和L(离视线最近的交点->光源)
    * 的cos值是否为负, 且是否为离光源最近的交点
    * 为负,或不是,即为无效局部光照
    */
    Vector localN = objects[closestPointBySight.objectIndex].planes[closestPointBySight.planeIndex].n, localL(closestPointBySight, lightSource),
        localV(closestPointBySight, R.P); // V(离视线最近的交点->视线)
    localL.unit();
    localV.unit();
    // cosa = N * L / |N| * |L|, N, L已经单位化, 简化运算
    double cosa = localN * localL;
    if (cosa >= 0 && intersectPointByLight == closestPointBySight) {
        // 求漫反射光强度
        // Id = Ie * Kd * cosa
        local_color = material.color * material.Ka * material.Ia_Ie_rate + material.color * material.Kd * cosa;
    }
    else {
        local_color = material.color * material.Ka * material.Ia_Ie_rate;
    }

    /*
    * 4.反射光与折射光
    */
    if (material.Kd != 0) {
        // 反射光 Rr = L - 2 * (L * N) * N
        Vector reflectedN = localN, reflectedL = localV * -1;
        Vector Rr = reflectedL - reflectedN * 2.0 * (reflectedL * reflectedN);
        Ray reflectedRay(closestPointBySight, Rr);
        rayTrace(reflectedRay, ratio * material.Kd, depth + 1, reflected_color);
    }
    if (material.Kt != 0) {
        // 折射光 Rt = L / e + N * (cosO1 / e - cosO2),
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
    cout << "像素大小: " << pixelSize << endl;
    cout << "宽度: " << doubleToString(widthStart) << "~" << doubleToString(widthEnd) << ", 宽度像素数: " << widthPixelCount << endl;
    cout << "高度: " << doubleToString(heightStart) << "~" << doubleToString(heightEnd) << ", 高度像素数: " << heightPixelCount << endl;
    cout << "距离: " << doubleToString(distance) << endl;
    cout << "摄像机: " << camera.toString() << endl;
    cout << "光源: " << lightSource.toString() << endl;
}

// 旋转视窗口, 使视窗口中心与摄像机与原点处于同一直线上
void Observer::rotateWindowToCamera() {
    // 分析边界情况
    vector<vector<double> > matrix1(3, vector<double>(3, 0)), matrix2(3, vector<double>(3, 0));
    bool needMatrxi2 = false;
    if (camera.x == 0 && camera.y == 0 && camera.z == 0) {

    }
    else if (camera.x == 0 && camera.y == 0 && camera.z != 0) { // 若x == 0
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

        // 若Pm.y < 0,  为逆时针，不需转置; 否则需转置
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
    else { // 若x ！= 0
           // 计算旋转角O1(绕y轴), O2(绕x轴), 初始视窗中心Pi(nitial), 旋转O1后的视窗中心Pt(emp), 最终视窗中心Pm(iddle)
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

        // 若Pt.x > 0 (Pm.y < 0),  为逆时针，不需转置; 否则需转置
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
        // 与L(原点->Pt)垂直的V(x,y,z), V指向L的右边
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
    cout << "视窗口变换矩阵:" << endl;
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

    // 初始化结果点数组, 先初始化与xy平面平行的像素平面
    // 其中每个像素对应的顶点的坐标取像素中心处，所以xy坐标均加上半个像素大小
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
            // 旋转变换
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
            // 为提高速度, 直接判断三角平面三个顶点与光线向量的关系
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