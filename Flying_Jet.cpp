/*********
STUDENT : SYED RAFEH HUSSAIN
STUDENT NUMBER : 22001502
SECTION : 01
HOMEWORK: #1
----------
PROBLEMS: The Jet passes out of TV screen on top
----------
ADDITIONAL FEATURES: Jet speed can be increased by pressing F and decreased by pressing S
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  700
#define WINDOW_HEIGHT 400

#define TIMER_ON 1; // 0:disable timer, 1:enable timer

#define D2R 0.0174532;

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

int yR = 0; // Jet animation value
double timer_period = 25; // Jet Speed

#define OFF 0 // TV turned off
#define RUN 1 // TV running
#define PAUSE 2 // TV paused

int TVState = OFF; // TV turned off at start

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void drawJet() {
    
    glColor3ub(255, 0, 0);
    glRectf(-10, -140 + yR, 10, -100 + yR); // jet body

    glColor3ub(0, 0, 255);
    glBegin(GL_TRIANGLES); // jet top
     glVertex2f(-10, -100 + yR);
     glVertex2f(10, -100 + yR);
     glVertex2f(0, -85 + yR);
    glEnd();

    vprint(-6, -130 + yR, GLUT_BITMAP_HELVETICA_18, "R"); // jet symbol

    glColor3ub(25, 125, 15);
    glBegin(GL_QUADS); // jet wings

    glVertex2f(10, -130 + yR);
    glVertex2f(10, -110 + yR);
    glVertex2f(25, -140 + yR);
    glVertex2f(25, -120 + yR);

    glVertex2f(-10, -130 + yR);
    glVertex2f(-10, -110 + yR);
    glVertex2f(-25, -140 + yR);
    glVertex2f(-25, -120 + yR);

    glEnd();

    glColor3ub(127, 94, 12); // jet windows
    circle(-15, -120 + yR, 3);
    circle(15, -120 + yR, 3);

    glColor3ub(255, 215, 0);
    glBegin(GL_TRIANGLE_STRIP); // jet missile launcher

    glVertex2f(10, -105 + yR);
    glVertex2f(15, -115 + yR);
    glVertex2f(20, -102 + yR);
    glVertex2f(25, -112 + yR);
    glVertex2f(30, -105 + yR);
    glVertex2f(35, -115 + yR);


    glVertex2f(-10, -105 + yR);
    glVertex2f(-15, -115 + yR);
    glVertex2f(-20, -102 + yR);
    glVertex2f(-25, -112 + yR);
    glVertex2f(-30, -105 + yR);
    glVertex2f(-35, -115 + yR);

    glEnd();

    glColor3ub(255, 0, 0);
    glBegin(GL_LINES); // jet missiles
    glVertex2f(25, -112 + yR);
    glVertex2f(40, -97 + yR);
    glVertex2f(-25, -112 + yR);
    glVertex2f(-40, -97 + yR);
    glEnd();
}


void display_TV() {
    glColor3ub(211, 211, 211);
    glRectf(-320, -180, 320, 180); // TV body
    glColor3ub(0, 90, 0);
    glRectf(-310, -140, 310, 170); // TV screen
    glColor3ub(190, 190, 190);
    glRectf(-200, -175, -120, -148); // On & Off Button
    glRectf(200, -175, 40, -148); // Pause & Restart Button
    glColor3ub(0, 0, 0);
    vprint(-195, -168, GLUT_BITMAP_HELVETICA_18, "ON/OFF");
    vprint(45, -168, GLUT_BITMAP_HELVETICA_18, "PAUSE/RESTART");
}

void run_TV() {
    glColor3ub(135, 206, 235);
    glRectf(-310, -140, 310, 170); // sky
    glColor3ub(255, 0, 0); // instructions
    vprint(-300, 150, GLUT_BITMAP_HELVETICA_18, "Press F to increase");
    vprint(-300, 130, GLUT_BITMAP_HELVETICA_18, "rocket speed");
    vprint(140, 150, GLUT_BITMAP_HELVETICA_18, "Press S to decrease");
    vprint(140, 130, GLUT_BITMAP_HELVETICA_18, "rocket speed");
    drawJet();
    }

//
// To display onto window using OpenGL commands
//
void display() {

    // check TV state

    switch (TVState) {
    case OFF: display_TV(); break;
    case RUN: run_TV(); break;
    case PAUSE: break;
    }

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (key == 70 || key == 102) // increasing speed
            timer_period /= 1.3;
 

    if (key == 83 || key == 115) // decreasing speed
        timer_period *= 1.3;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && x >= 150 && x <= 230 && y <= 375 && y >= 348) { // On & Off Button
        if (TVState == OFF)
            TVState = RUN;
        else if (TVState == RUN || TVState == PAUSE)
            TVState = OFF;
        }

    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && x >= 390 && x <= 550 && y <= 375 && y >= 348) { // Pause & Restart Button
        if (TVState == RUN)
            TVState = PAUSE;
        else if (TVState == PAUSE)
            TVState = RUN;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(timer_period, onTimer, 0);
    // Write your codes here.
    if (TVState == RUN || TVState == OFF) // jet does not move when TV is paused
    {
        if (yR < winHeight - 55) // jet moving up (55 is jet total length)
            yR++;
        else
            yR = 0; // jet starting movement again from start after reaching the top
    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("HW #1: Television (by Rafeh Hussain)");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(timer_period, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}
