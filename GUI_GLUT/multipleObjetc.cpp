#include <string>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
using namespace std;

//gcc -o objects multipleObjetc cpp  -framework Carbon -framework OpenGL -framework GLUT && ./objects


void drawline(float from_x, float from_y, float to_x, float to_y)
{
    // From coordinate position
    glVertex2f(from_x, from_y);

    // To coordinate position
    glVertex2f(to_x, to_y);
}

void DrawBoard(int w, int h, int n, int m) {
  bool color = true;
  int sw = w/n, sh = h/m; //square width and height respectively
  //for each width and height draw a rectangle with a specific color
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < m; ++j) {
      //oscillate the color per square of the board
      if(color)
        glColor3f(1, 1, 1);
      else
        glColor3f(0, 0, 0);
      color = !color;

      //draw a rectangle in the ith row and jth column
      glRecti(i*sw, j*sh, (i+1)*sw, (j+1)*sh);
    }
    if(m % 2 == 0) color = !color; //switch color order at end of row if necessary
  }
}

void drawShape()
{
    glColor3f(1.0, 1.0, 0.0); // Color (RGB): Yellow
    glLineWidth(2.0); // Set line width to 2.0

    // Draw line
    glBegin(GL_LINES);
    drawline(0.25, 0.5, 0.4, 0.5);
    drawline(0.4, 0.6, 0.4, 0.5);
    drawline(0.4, 0.4, 0.4, 0.5);
    drawline(0.6, 0.5, 0.75, 0.5);
    glEnd();

    // Draw triangle
    glBegin(GL_TRIANGLES);
    glVertex2f(0.4, 0.5);
    glVertex2f(0.6, 0.6);
    glVertex2f(0.6, 0.4);
    glEnd();
}

void render(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( 0.0, 4.0, 0.0, 4.0, -1, 1 );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw shape one
    glPushMatrix();
    glTranslatef(1.5, 1.5, 0.0);
    drawShape();
    glPopMatrix();

    // Draw shape two
    glPushMatrix();
    glTranslatef(2.5, 2.5, 0.0);
    drawShape();
    glPopMatrix();

    // Draw shape two
    glPushMatrix();
    DrawBoard(750,750,8,8);
    glPopMatrix();

    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize (750, 750);
    glutCreateWindow("My OpenGL program");
    glutDisplayFunc(render);
    glutMainLoop();
    return 0;
}
