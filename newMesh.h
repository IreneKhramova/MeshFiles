#ifndef MESH_H
#define MESH_H

#include <vector>
#include <set>
#include <list>
#include <cmath>
#include <cstdio>

using namespace std;

class Point
{
public:
	double x;
	double y;
	double z;
};


///////////////////////
class Edge
{
private:
	Point* p[2];
	int countOfUsing;

public:
	Edge(Point*, Point*);
    double getlength();
    static Point getVector(Point*, Point*);

	friend class Cell;
};


////////////////////////
class Face
{
 protected:
    int type;
 public:
    virtual void area() = 0;

 friend class Cell;
};

class Face_triangle : public Face
{
private:
    Point* p[3];
	Edge* e[3];
	int countOfUsing;
	double S;

public:
    Face_triangle(int&, Point*, Point*, Point*);
	void initFace(Edge*, Edge*, Edge*);
	void area();

    friend class Cell;
	friend class Cell_tetrahedron;
};

class Face_quad : public Face
{
private:
    Point* p[4];
	Edge* e[4];
	int countOfUsing;
	double S;

public:
    Face_quad(int&, Point*, Point*, Point*, Point*);
	void initFace(Edge*, Edge*, Edge*, Edge*);
    void area();

    friend class Cell;
	friend class Cell_hexahedron;
};

///////////////////

class Cell {

 protected:
    int type;
    Edge* getEdge(list<Edge*>&, Point*, Point*);
    Face* getFace(list<Face*>&, Point*, Point*, Point*);
    Face* getFace(list<Face*>&, Point*, Point*, Point*, Point*);
 public:
    virtual void volume() = 0;
};

class Cell_tetrahedron: public Cell
{
private:
    Point* p[4];
	Edge* e[6];
	Face_triangle* f[4];
	double V;

public:
	Cell_tetrahedron(list<Edge*>&, list<Face*>&, int&, Point*, Point*, Point*, Point*);
	void volume();
};

class Cell_hexahedron: public Cell
{
private:
    Point* p[8];
	Edge* e[12];
	Face_quad* f[6];
	double V;

public:
	Cell_hexahedron(list<Edge*>&, list<Face*>&, int&, Point*, Point*, Point*, Point*, Point*, Point*, Point*, Point*);
    void volume();
};

//////////////
class Mesh
{
private:
    Point* points;
    unsigned int pCount;
    list<Edge*> edges;
    list<Face*> faces;
    vector<Cell*> cells;


public:
    Mesh(Point*, unsigned int);
    Mesh(const vector<Point>&);
    ~Mesh();

    void createEdge(int, int);
    void createFace(int, int, int, int);
    void createFace(int, int, int, int, int);
    void createCell(int, int, int, int, int);
    void createCell(int, int, int, int, int, int, int, int, int);

};

#endif // MESH_H
