#include <cmath>
#include <iostream>
#include "Mesh.h"

using std::cout;
using std::endl;

///////////////////////
void Edge::initEdge(Point* p1, Point* p2)
{
    p[0] = p1;
    p[1] = p2;
}

double Edge::getlength()
{
    return sqrt( pow(p[0]->x - p[1]->x, 2) + pow(p[0]->y - p[1]->y, 2) + pow(p[0]->z - p[1]->z, 2) );
}


////////////////////////
void Face::initFace(Edge* e1, Edge* e2, Edge* e3, Edge* e4)
{
  e[0] = e1;
  e[1] = e2;
  e[2] = e3;
  e[3] = e4;
}

///////////////////

Cell::Cell(Point* p)
{
    this->p = p;
    e = new Edge[12];
    f = new Face[6];

    e[0].initEdge(&p[0], &p[1]);
    e[1].initEdge(&p[1], &p[2]);
    e[2].initEdge(&p[2], &p[3]);
    e[3].initEdge(&p[3], &p[0]);
    e[4].initEdge(&p[4], &p[5]);
    e[5].initEdge(&p[5], &p[6]);
    e[6].initEdge(&p[6], &p[7]);
    e[7].initEdge(&p[7], &p[4]);
    e[8].initEdge(&p[0], &p[4]);
    e[9].initEdge(&p[1], &p[5]);
    e[10].initEdge(&p[2], &p[6]);
    e[11].initEdge(&p[3], &p[7]);

    f[0].initFace(&e[0], &e[9], &e[4], &e[8]);
    f[1].initFace(&e[1], &e[10], &e[5], &e[9]);
    f[2].initFace(&e[2], &e[11], &e[6], &e[10]);
    f[3].initFace(&e[3], &e[8], &e[7], &e[11]);
    f[4].initFace(&e[0], &e[1], &e[2], &e[3]);
    f[5].initFace(&e[4], &e[5], &e[6], &e[7]);
}

Cell::~Cell()
{
    delete [] e;
    delete [] f;
}

int main()
{
    Point p[8];
    p[0].x = 0; p[0].y = 0; p[0].z = 0;
    p[1].x = 0; p[1].y = 1; p[1].z = 0;
    p[2].x = 1; p[2].y = 1; p[2].z = 0;
    p[3].x = 1; p[3].y = 0; p[3].z = 0;
    p[4].x = 0; p[4].y = 0; p[4].z = 1;
    p[5].x = 0; p[5].y = 1; p[5].z = 1;
    p[6].x = 1; p[6].y = 1; p[6].z = 1;
    p[7].x = 1; p[7].y = 0; p[7].z = 1;
    //Единичный куб(координаты нужно вводить в определенном порядке)
    Cell cell(p);
    //Вычисляет наибольшую длину ребра
    return 0;
}