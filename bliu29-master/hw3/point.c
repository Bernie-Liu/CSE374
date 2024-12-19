// Simple example of a point struct.
// CSE 374 demo

#include <stdio.h>

// Declaring a new struct of type "struct Point" and
// using typedef to simplify the name to "Point"
typedef struct Point {
  int x;
  int y;
} Point;

// Forward declarations
Point newPoint();
//Point* newDanglingPoint();
void translateX(Point* p, int deltaX);
void translateX_wrong(Point p, int deltaX);
void print(Point* p);
void print_point(Point p);

// main tests the Point struct
int main(int argc, char **argv) {
  Point p = newPoint();
  printf ("Show original point and translated point.\n");
  print(&p);    // pass by reference so that the original can be modified
  translateX(&p, 12);
  print(&p);

  printf ("Show incorrectly translated point doesn't move.\n");
  translateX_wrong(p, 12);
  print(&p);

  printf ("But pass by value works for print.\n");
  print_point (p);
}

// constructor for a new Point
Point newPoint() {
  Point p;
  p.x = 0;
  p.y = 0;
  return p;
}

// construct a dangling pointer to no where
/*Point* newDanglingPoint() {
  Point p;
  p.x = 0;
  p.y = 0;
  return &p;  // Best case:  You crash.
  }*/

// translateX moves one point horizontally by deltax
void translateX(Point* p, int deltaX) {
  p->x += deltaX;
  // OR
  // (*p).x += deltaX;
}

// translateX_wrong won't move the original point
void translateX_wrong(Point p, int deltaX) {
  p.x += deltaX;
}

// print out the point.
void print(Point* p) {
  printf("p = (%d, %d)\n", p->x, p->y);
}

// note:  here we could pass by value
void print_point(Point p) {
  printf("p = (%d, %d)\n", p.x, p.y);
}
