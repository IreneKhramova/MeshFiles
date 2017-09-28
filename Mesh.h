#ifndef MESH_H
#define MESH_H

template <class T>
class MeshIterator;

class Pointf
{
public:
//private:	
	double x;
	double y;
	double z;
};


///////////////////////
class Edge
{
private:
	Point* p[2];

public:
	void initEdge(Point* p1, Point* p2);

	Point* getPoint(int i);

	double getlength();

	friend class Cell;
};


////////////////////////
class Face
{
private:
	Edge* e[4];
public:
	void initFace(Edge* e1, Edge* e2, Edge* e3, Edge* e4);

	Edge* getEdge(int i);

	friend class Cell;
};

///////////////////
class Cell
{
private:
	Edge* e;
	Face* f;
	Point* p;

public:
	Cell(Point* p);

	typedef MeshIterator<Face> FaceIterator;
	typedef MeshIterator<Edge> EdgeIterator;
	typedef MeshIterator<Point> PointIterator;

	FaceIterator beginFace();
	FaceIterator endFace();

	EdgeIterator beginEdge();
	EdgeIterator endEdge();

	PointIterator beginPoint();
	PointIterator endPoint();

	~Cell();
};

#endif // MESH_H