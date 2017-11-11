#ifndef MESH_H
#define MESH_H

#include <vector>
#include <set>
#include <list>
#include <cmath>


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

	friend class Cell;
};


////////////////////////
class Face
{
private:
    Point* p[4];
	Edge* e[4];
	int countOfUsing;

public:
    Face(Point*, Point*, Point*, Point*);
	void initFace(Edge*, Edge*, Edge*, Edge*);

	friend class Cell;
};

///////////////////
class Mesh;

class Cell
{
private:
    Point* p[8];
	Edge* e[12];
	Face* f[6];

	Edge* getEdge(list<Edge*>&, Point*, Point*);
    Face* getFace(list<Face*>&, Point*, Point*, Point*, Point*);

public:
	Cell(list<Edge*>&, list<Face*>&, Point*, Point*, Point*, Point*, Point*, Point*, Point*, Point*);

	friend void vtkWriteUnstructuredGrid(const char *filename, Mesh* mesh);
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
    ~Mesh();

    void createEdge(int, int);
    void createFace(int, int, int, int);
    void createCell(int, int, int, int, int, int, int, int);

	friend void vtkWriteUnstructuredGrid(const char *filename, Mesh* mesh);
};

#endif // MESH_H
