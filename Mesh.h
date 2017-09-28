#ifndef MESH_H
#define MESH_H

class Point
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

	~Cell();
};

#endif // MESH_H