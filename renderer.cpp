// simple.cpp - simple introduction to OpenGL and GLUT programming in C++
// Compile on Linux with:  
//  g++ -L/usr/X11R6/lib simple.cpp -lglut -lGLU -lGL -lXmu -lX11 -lXi -lm

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <list>
//#include <GL/gl.h>
#include <GL/glut.h>
#include "clip.h"

using namespace std;

// ---------------- 2D point class ----------------------------------



struct Point2D {
    float x,y;

    Point2D() { x = 0.0; y = 0.0; }
    Point2D(float _x, float _y) : x(_x), y(_y) {}

    double dist(Point2D a);
    bool closeEnough(Point2D other);
};

// distance between this point and a
double
Point2D::dist(Point2D a)
{
    return sqrt((a.x - x)*(a.x - x) + (a.y - y)*(a.y - y));
}

// true if a is close enough to this point
bool
Point2D::closeEnough(Point2D a)
{
    return (dist(a) < 0.02);
}

//------------------- Interactive triangle class --------------------

class Triangle {
  protected:
    
    int dragPoint; // current drag corner if any (-1 if none)

  public:
	Point2D pts[3]; // the points defining the triangle
    Triangle();

    void draw(); // draw the triangle
    bool onDown(Point2D p); // call on mouse-down
    void onDrag(Point2D p); // call on mouse drag to move the corner
};

Triangle::Triangle()
{
    // init default triangle
    pts[0] = Point2D(0.30, 0.30);
    pts[1] = Point2D(0.15, 0.50);
    pts[2] = Point2D(0.15, 0.30);
    dragPoint = -1;
}

void
Triangle::draw()
{
    int i;

 //   // draw the white triangle using the 3 points
 //   glColor3f(1.0, 1.0, 1.0);
 //   glBegin(GL_TRIANGLES);
 //   for (i = 0; i < 3; ++i) {
	//glVertex2f(pts[i].x, pts[i].y);
 //   }
 //   glEnd();

    // draw the red outline of the triangle on top
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    for (i = 0; i < 3; ++i) {
	glVertex2f(pts[i].x, pts[i].y);
    }
    glEnd();
}

// test the mouse coordinate a against the 3 corners and return true
// if the mouse is close enough. Also sets the dragPoint of the
// triangle to corner that was hit if any.
bool
Triangle::onDown(Point2D a)
{
    int i;
    for (i = 0; i < 3; ++i) {
	if (a.closeEnough(pts[i])) {
	    dragPoint = i;
	    return true;
	}
    }
    return false;
}

// move the dragPoint corner along with the mouse
void
Triangle::onDrag(Point2D a)
{
    if (dragPoint != -1) {
	pts[dragPoint] = a;
    }
}

class Ply
{
  private:


  public:
	Ply();
	Point2D pts[6]; // the points defining the triangle
	int nv;
	void draw();
};

void Ply::draw()
{
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < nv; ++i) {
	glVertex2f(pts[i].x, pts[i].y);
    }
    glEnd();
}

Ply::Ply()
{
	nv = 0;
}




//------------------ main program ------------------------------------

// global variables
typedef list<Triangle *> TriList;
TriList triangles; // the list of our triangles
Triangle *activeTriangle = NULL; // the active triangle while dragging
int windowWidth, windowHeight; // dimensions of the window in pixel
Ply clipped;

inline void clipTwoTriangles()
{
	ClipMerge(
	//ClipTriangle(
		triangles.front()->pts[0].x, triangles.front()->pts[0].y, 
		triangles.front()->pts[1].x, triangles.front()->pts[1].y, 
		triangles.front()->pts[2].x, triangles.front()->pts[2].y, 
		triangles.back()->pts[0].x, triangles.back()->pts[0].y, 
		triangles.back()->pts[1].x, triangles.back()->pts[1].y, 
		triangles.back()->pts[2].x, triangles.back()->pts[2].y,
		clipped.pts[0].x, clipped.pts[0].y, 
		clipped.pts[1].x, clipped.pts[1].y, 
		clipped.pts[2].x, clipped.pts[2].y, 
		clipped.pts[3].x, clipped.pts[3].y, 
		clipped.pts[4].x, clipped.pts[4].y, 
		clipped.pts[5].x, clipped.pts[5].y, 
		clipped.nv);
}

void
init()
{
    triangles.push_back(new Triangle); // create initial triangle
	triangles.push_back(new Triangle); 
	//triangles.front()->pts[0].x
	clipTwoTriangles();

    // init some OpenGL state
    glClearColor(0.0, 0.0, 0.0, 0.0); // blue background
    glShadeModel(GL_FLAT);
}


inline void DrawPoint(Point2D pt, int shape, int color)
{
	switch (color)
	{
	case 0:
		glColor3f(1.0, 0.0, 0.0);
		break;
	case 1:
		glColor3f(0.0, 1.0, 0.0);
		break;
	case 2:
		glColor3f(0.0, 0.0, 1.0);
		break;
	default:
		break;
	}
	switch (shape)
	{
	case 0:
		glPointSize(4);
		break;
	case 1:
		glPointSize(8);
		break;
	default:
		break;
	}
	glBegin(GL_POINTS);
	glVertex2f(pt.x, pt.y);
	glEnd();
}

// called on window refresh events via glut
void
display()
{
    TriList::iterator i;

    glClear(GL_COLOR_BUFFER_BIT); // clear color buffer
	
	glColor4f(0.0, 0.0, 1.0, 1.0);
	clipped.draw();
	glColor4f(0.0, 1.0, 0.0, 1.0);
    // draw all the triangles
    //for (i = triangles.begin(); i != triangles.end(); ++i) {
	triangles.front()->draw();//)->draw();
	triangles.back()->draw();

	for(int i = 0; i < 3; i++)	{
		DrawPoint(triangles.front()->pts[i], 0, i);
		DrawPoint(triangles.back()->pts[i], 1, i);
	}
    //}
	glColor4f(0.0, 0.0, 0.0, 1.0);

    glFlush(); // flush gl pipe

}

// called initially and on window-resizing
void
reshape(int w, int h)
{
    // set the viewport to fill all of the window
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    windowWidth = w;
    windowHeight = h;

    // set top projection matrix to do 2D parallel projection, and make
    // the coordinate system of the window a unit square with origin at
    // the bottom left corner.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
}

// convert mouse coordinate in pixel into the window coordinate system
Point2D
mouseToPoint(int x, int y)
{
    return Point2D(float(x)/windowWidth, 1.0 - float(y)/windowHeight);
}

// called on mouse button events
void
mouse(int button, int state, int x, int y)
{
    TriList::iterator i;

    activeTriangle = NULL;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
	for (i = triangles.begin(); i != triangles.end(); ++i) {
	    if ((*i)->onDown(mouseToPoint(x, y))) {
		activeTriangle = (*i);
		break;
	    }
	}
    }
}

// called on mouse drag events
void
drag(int x, int y)
{
    if (activeTriangle != NULL) {
	// drag corner of active triangle with the mouse
	activeTriangle->onDrag(mouseToPoint(x, y));
	glutPostRedisplay(); // force re-display
    }
	clipTwoTriangles();

}

// called on popup-menu invocation
void
menuFunc(int item)
{
    switch (item)
    {
      case 1:
	  // create another triangle
	  triangles.push_back(new Triangle);
	  break;
      case 2:
	  exit(0);
	  break;
      default:
	  break;
    }
    glutPostRedisplay(); // tell GLUT to call display()
}

int
main(int argc, char *argv[])
{
    glutInit(&argc, argv); // init GLUT library
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB); // ask for true color and single buffer
    glutInitWindowSize(600, 500); // set initial window size
    glutCreateWindow("Triangles"); // open window with a title

    init();

    // register callback functions for redraw, mouse events etc.
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(drag);
    glutReshapeFunc(reshape);

    // create right button popup menu
    glutCreateMenu(menuFunc);
    glutAddMenuEntry("New Triangle", 1);
    glutAddMenuEntry("Quit", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop(); // enter main loop
    return 0;
}