// Illumination.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "interfaces.h"
using namespace std;

int xPixelCount = WIDTH * PIXEL_COUNT_PER_SIZE, yPixelCount = HEIGHT * PIXEL_COUNT_PER_SIZE; // 观察面xy方向的像素个数
Observer observer(xPixelCount, yPixelCount, 1 / (double)PIXEL_COUNT_PER_SIZE, DISTANCE, CAMERA, LIGHT_SOURCE, OBJECT_PATHS, MATERIALS);

void myinit()
{
    // attributes
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glColor3f(1.0, 1.0, 1.0);

    // set up viewing
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, xPixelCount, 0.0, yPixelCount);
    glMatrixMode(GL_MODELVIEW);
}

void display()
{
    glBegin(GL_POINTS);
    for (int xIndex = 0; xIndex < observer.window.size(); ++xIndex) {
        for (int yIndex = 0; yIndex < observer.window[xIndex].size(); ++yIndex) {
            ColorPoint point = observer.window[xIndex][yIndex];
            glColor3f(point.color.red, point.color.green, point.color.blue);
            GLfloat pixel[3] = { xIndex, yIndex, 0.0 };
            glVertex3fv(pixel);
        }
    }
    glEnd();
    glFlush();
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(xPixelCount, yPixelCount);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Ray Tracing");
    glutDisplayFunc(display);
	BSPtree(observer.objects);
	saveBmp(observer.window, "data/out.bmp");
    myinit();

	

    glutMainLoop();
    
    //// debug
    //Ray tempRay(Point(0, 0, 1.5), Point(0.003333, 0.003333, 1));
    //Plane tempPlane = object.planes[2];
    //Point intersectPoint = tempPlane.getIntersectByRay(tempRay);
    //Point test(-2.5289, -3.02859, 1.2292);
    //cout << tempRay.toString() << endl;
    //cout << tempPlane.toString() << endl;
    //cout << intersectPoint.toString() << endl;
    //cout << (tempPlane.isInside(intersectPoint) ? "true" : "false") << endl;
    //return 0;
}

