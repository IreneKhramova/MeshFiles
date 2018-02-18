#ifndef MESH_H
#define MESH_H

#include <vector>
#include <cmath>
#include <cstdio>

class PointIterator;

template <class T>
class MeshIterator;

template <class Predicate, class Iterator>
class FilterIterator;

class Point;
class Edge;
class Face;
class Cell;
/* Callback - функция для iterateCells() */
typedef void(*iterateCellsFunc)(Cell*);
/* Callback - функция для iterateFaces() */
typedef void(*iterateFacesFunc)(Face*);
/* Callback - функция для iterateEdges() */
typedef void(*iterateEdgesFunc)(Edge*);
/* Callback - функция для iteratePoints() */
typedef void(*iteratePointsFunc)(Point*);

using namespace std;

class Point
{
public:
	double x;
	double y;
	double z;
};

///////////////////////

class Cell;

class Edge
{
private:
	Point* p[2];

public:
	Edge(Point*, Point*);
    double getlength();
    static double getlength(Point*, Point*);
    static Point getVector(Point*, Point*);
};

////////////////////////

class Face
{
 private:
    int type;
    Point** p;
	Edge** e;
	Cell* c[2]; // указатель на смежный Cell
	int pCount;
	int eCount;
	int countOfUsing;
	double S;

 public:
    Face(const int&, Point*, Point*, Point*);
    Face(const int&, Point*, Point*, Point*, Point*);

    ~Face();

    void initFace(Cell*, Edge*, Edge*, Edge*);
    void initFace(Cell*, Edge*, Edge*, Edge*, Edge*);

    void area();

	Cell** getCells()
	{
		return c;
	}

 friend class Cell;
};

///////////////////

class Mesh;

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

 public:
    Cell(const int&, Point*, Point*, Point*, Point*);
    Cell(const int&, Point*, Point*, Point*, Point*, Point*, Point*);
    Cell(const int&, Point*, Point*, Point*, Point*, Point*, Point*, Point*, Point*);

    ~Cell();

    void volume();

    friend void vtkWriteUnstructuredGrid(const char *filename, Mesh* mesh);
    friend class Mesh;
    friend class MeshReaderUnv;
};

//////////////

class IsBoundaryFace {
public:
	bool operator()(Face* f);
};

class IsInnerFace {
public:
	bool operator()(Face* f);
};

//////////////

class Mesh
{
private:
    Point* points;
    unsigned int pCount;
    vector<Edge*> edges;
    vector<Face*> faces;
    vector<Cell*> cells;

public:
	Mesh() {}
    ~Mesh();

    void createPoints(Point*, unsigned int);
    void createPoints(const vector<Point>&);

    friend void vtkWriteUnstructuredGrid(const char *filename, Mesh* mesh);

    friend class MeshReaderUnv;

	typedef MeshIterator<Face> FaceIterator;
	typedef MeshIterator<Edge> EdgeIterator;
	typedef MeshIterator<Cell> CellIterator;
	typedef PointIterator PointIterator;
	typedef FilterIterator<IsBoundaryFace, FaceIterator> BoundaryFaceIterator;
	typedef FilterIterator<IsInnerFace, FaceIterator> InnerFaceIterator;

	CellIterator beginCell();
	CellIterator endCell();

	FaceIterator beginFace();
	FaceIterator endFace();

	EdgeIterator beginEdge();
	EdgeIterator endEdge();

	PointIterator beginPoint();
	PointIterator endPoint();

	void iterateCells(iterateCellsFunc);
	void iterateFaces(iterateFacesFunc);
	void iterateEdges(iterateEdgesFunc);
	void iteratePoints(iteratePointsFunc);

	BoundaryFaceIterator beginBoundaryFace();
	BoundaryFaceIterator endBoundaryFace();

	InnerFaceIterator beginInnerFace();
	InnerFaceIterator endInnerFace();
};

#endif // MESH_H