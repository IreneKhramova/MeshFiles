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
    static Point getVector(Point*, Point*);

	friend class Cell;
};


////////////////////////
class Face
{
 private:
    int type;
    Point** p;
	Edge** e;
	int pCount;
	int eCount;
	int countOfUsing;
	double S;

 public:
    Face(const int&, Point*, Point*, Point*);
    Face(const int&, Point*, Point*, Point*, Point*);

    ~Face();

    void initFace(Edge*, Edge*, Edge*);
    void initFace(Edge*, Edge*, Edge*, Edge*);

    void area();

 friend class Cell;
};

///////////////////

class Cell {

 private:
    int type;
    Point** p;
    Edge** e;
    Face** f;
    int pCount;
    int eCount;
    int fCount;
    double V;

    Edge* getEdge(list<Edge*>&, Point*, Point*);
    Face* getFace(list<Face*>&, Point*, Point*, Point*);
    Face* getFace(list<Face*>&, Point*, Point*, Point*, Point*);

 public:
    Cell(list<Edge*>&, list<Face*>&, int&, Point*, Point*, Point*, Point*);
    Cell(list<Edge*>&, list<Face*>&, int&, Point*, Point*, Point*, Point*, Point*, Point*, Point*, Point*);

    ~Cell();

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
