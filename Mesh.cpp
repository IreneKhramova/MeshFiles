#include "Mesh.h"
#include "MeshIterator.h"
#include "PointIterator.h"

///////////////////////

Edge::Edge(Point* p1, Point* p2)
{
	p[0] = p1;
	p[1] = p2;
}

double Edge::getlength()
{
	return sqrt(pow(p[0]->x - p[1]->x, 2) + pow(p[0]->y - p[1]->y, 2) + pow(p[0]->z - p[1]->z, 2));
}

double Edge::getlength(Point* p1, Point* p2)
{
	return sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2) + pow(p1->z - p2->z, 2));
}

Point Edge::getVector(Point* p1, Point* p2)
{
	Point vec;

	vec.x = p2->x - p1->x;
	vec.y = p2->y - p1->y;
	vec.z = p2->z - p1->z;

	return vec;
}

////////////////////////
Face::~Face()
{
	delete[] p;
	delete[] e;
}

Face::Face(const int& type, Point* p1, Point* p2, Point* p3)
{
	this->type = type;
	pCount = 3;
	eCount = 3;
	countOfUsing = 2;
	c[0] = 0;
	c[1] = 0;

	p = new Point*[pCount];
	e = new Edge*[eCount];

	p[0] = p1;
	p[1] = p2;
	p[2] = p3;
}

void Face::initFace(Cell* c, Edge* e1, Edge* e2, Edge* e3)
{
	if (countOfUsing == 2) { // �������������� � ������ ���

		e[0] = e1;
		e[1] = e2;
		e[2] = e3;

		this->c[0] = c;
	}
	else {
		this->c[1] = c;
	}
	countOfUsing--;
}


Face::Face(const int& type, Point* p1, Point* p2, Point* p3, Point* p4)
{
	this->type = type;
	pCount = 4;
	eCount = 4;
	countOfUsing = 2;
	c[0] = 0;
	c[1] = 0;

	p = new Point*[pCount];
	e = new Edge*[eCount];

	p[0] = p1;
	p[1] = p2;
	p[2] = p3;
	p[3] = p4;
}

void Face::initFace(Cell* c, Edge* e1, Edge* e2, Edge* e3, Edge* e4)
{
	if (countOfUsing == 2) { // �������������� � ������ ���

		e[0] = e1;
		e[1] = e2;
		e[2] = e3;
		e[3] = e4;

		this->c[0] = c;
	}
	else {
		this->c[1] = c;
	}
	countOfUsing--;
}

// ���������� ������� �����
void Face::area()
{
	S = 0;
	switch (type)
	{
	case 91: // �����������( ��� ������� unv - 91)
	{
		double a = e[0]->getlength();
		double b = e[1]->getlength();
		double c = e[2]->getlength();

		double pp = (a + b + c) / 2;

		S = sqrt(pp*(pp - a)*(pp - b)*(pp - c));
		break;
	}
	case 94: // ��������������� ( ��� ������� unv - 94)
	{
		double a = Edge::getlength(p[0], p[1]);
		double b = Edge::getlength(p[1], p[2]);
		double diag = Edge::getlength(p[0], p[2]);
		double c = Edge::getlength(p[2], p[3]);
		double d = Edge::getlength(p[3], p[0]);

		double p1 = (a + b + diag) / 2;
		double p2 = (c + d + diag) / 2;

		S = sqrt(p1*(p1 - a)*(p1 - b)*(p1 - diag)) + sqrt(p2*(p2 - c)*(p2 - d)*(p2 - diag));
		break;
	}
	}
}

///////////////////


Cell::~Cell()
{
	delete[] p;
	delete[] e;
	delete[] f;
}

Cell::Cell(const int& type, Point* p1, Point* p2, Point* p3, Point* p4)
{
	this->type = type;
	pCount = 4;
	eCount = 6;
	fCount = 4;

	p = new Point*[pCount];
	e = new Edge*[eCount];
	f = new Face*[fCount];

	p[0] = p1;
	p[1] = p2;
	p[2] = p3;
	p[3] = p4;
}

Cell::Cell(const int& type, Point* p1, Point* p2, Point* p3, Point* p4, Point* p5, Point* p6)
{
	this->type = type;
	pCount = 6;
	eCount = 9;
	fCount = 5;

	p = new Point*[pCount];
	e = new Edge*[eCount];
	f = new Face*[fCount];

	p[0] = p1;
	p[1] = p2;
	p[2] = p3;
	p[3] = p4;
	p[4] = p5;
	p[5] = p6;
}

Cell::Cell(const int& type, Point* p1, Point* p2, Point* p3, Point* p4, Point* p5, Point* p6, Point* p7, Point* p8)
{
	this->type = type;
	pCount = 8;
	eCount = 12;
	fCount = 6;

	p = new Point*[pCount];
	e = new Edge*[eCount];
	f = new Face*[fCount];

	p[0] = p1;
	p[1] = p2;
	p[2] = p3;
	p[3] = p4;
	p[4] = p5;
	p[5] = p6;
	p[6] = p7;
	p[7] = p8;
}

// ���������� ������ ������
void Cell::volume()
{
	V = 0;

	switch (type) {

	case 111: // 111 - ��������
	{

		Point vec_a = Edge::getVector(p[0], p[1]);
		Point vec_b = Edge::getVector(p[0], p[2]);
		Point vec_c = Edge::getVector(p[0], p[3]);

		V = abs(vec_a.x*(vec_b.y*vec_c.z - vec_b.z*vec_c.y) - vec_a.y*(vec_b.x*vec_c.z - vec_b.z*vec_c.x) + vec_a.z*(vec_b.x*vec_c.y - vec_b.y*vec_c.x)) / 3;

		break;
	}

	case 112:
	{

		break;
	}

	case 115: // 115 - ������������
	{
		Point vec_17 = Edge::getVector(p[1], p[7]);
		Point vec_14 = Edge::getVector(p[1], p[4]);
		Point vec_10 = Edge::getVector(p[1], p[0]);

		V += abs(vec_17.x*(vec_14.y*vec_10.z - vec_14.z*vec_10.y) - vec_17.y*(vec_14.x*vec_10.z - vec_14.z*vec_10.x) + vec_17.z*(vec_14.x*vec_10.y - vec_14.y*vec_10.x)) / 3;

		Point vec_12 = Edge::getVector(p[1], p[2]);
		Point vec_16 = Edge::getVector(p[1], p[6]);

		V += abs(vec_17.x*(vec_12.y*vec_16.z - vec_12.z*vec_16.y) - vec_17.y*(vec_12.x*vec_16.z - vec_12.z*vec_16.x) + vec_17.z*(vec_12.x*vec_16.y - vec_12.y*vec_16.x)) / 3;

		Point vec_56 = Edge::getVector(p[5], p[6]);
		Point vec_54 = Edge::getVector(p[5], p[4]);

		Point vec_norm;

		vec_norm.x = vec_56.y * vec_54.z - vec_56.z * vec_54.y;
		vec_norm.y = -vec_56.x * vec_54.z + vec_56.z * vec_54.x;
		vec_norm.z = vec_56.x * vec_54.y - vec_56.y * vec_54.x;

		double norm = sqrt(vec_norm.x * vec_norm.x + vec_norm.y * vec_norm.y + vec_norm.z * vec_norm.z);

		Point vec_51 = Edge::getVector(p[5], p[1]);
		double h = abs(vec_norm.x * vec_51.x + vec_norm.y * vec_51.y + vec_norm.z * vec_51.z) / norm;

		f[5]->area();

		V += f[5]->S * h / 3;

		vec_norm.x = vec_10.y * vec_12.z - vec_10.z * vec_12.y;
		vec_norm.y = -vec_10.x * vec_12.z + vec_10.z * vec_12.x;
		vec_norm.z = vec_10.x * vec_12.y - vec_10.y * vec_12.x;

		norm = sqrt(vec_norm.x * vec_norm.x + vec_norm.y * vec_norm.y + vec_norm.z * vec_norm.z);

		h = abs(vec_norm.x * vec_17.x + vec_norm.y * vec_17.y + vec_norm.z * vec_17.z) / norm;

		f[4]->area();

		V += f[4]->S * h / 3;

		break;
	}
	}
}

////////////////////

void Mesh::createPoints(Point* p, unsigned int pCount)
{
	this->pCount = pCount;
	points = new Point[pCount];
	for (int i = 0; i < pCount; i++)
	{
		points[i] = p[i];
	}
}

void Mesh::createPoints(const vector<Point>& p)
{
	this->pCount = p.size();
	points = new Point[pCount];
	for (int i = 0; i < pCount; i++)
	{
		points[i] = p[i];
	}
}

Mesh::~Mesh()
{
	delete[] points;

	for (vector<Edge*>::iterator it = edges.begin(); it != edges.end(); ++it)
		delete (*it);

	for (vector<Face*>::iterator it = faces.begin(); it != faces.end(); ++it)
		delete (*it);

	for (vector<Cell*>::iterator it = cells.begin(); it != cells.end(); ++it)
		delete (*it);
}

Mesh::CellIterator Mesh::beginCell() { return CellIterator(cells.begin()); }
Mesh::CellIterator Mesh::endCell() { return CellIterator(cells.end()); }

Mesh::FaceIterator Mesh::beginFace() { return FaceIterator(faces.begin()); }
Mesh::FaceIterator Mesh::endFace() { return FaceIterator(faces.end()); }

Mesh::EdgeIterator Mesh::beginEdge() { return EdgeIterator(edges.begin()); }
Mesh::EdgeIterator Mesh::endEdge() { return EdgeIterator(edges.end()); }

PointIterator Mesh::beginPoint() { return PointIterator(points); }
PointIterator Mesh::endPoint() { return PointIterator(&points[pCount+1]); }

//int main()
//{
//	Mesh* msh = new Mesh();
//	MeshReaderUnv mru("EFE3.unv");
//	mru.read(msh);
//}