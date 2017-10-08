#include "Mesh.h"
#include "CellIterator.h"
#include "PointIterator.h"

///////////////////////
Edge::Edge(Point* p1, Point* p2)
{
	countOfUsing = 4;
	p[0] = p1;
	p[1] = p2;
}

double Edge::getlength()
{
	return sqrt(pow(p[0]->x - p[1]->x, 2) + pow(p[0]->y - p[1]->y, 2) + pow(p[0]->z - p[1]->z, 2));
}


////////////////////////
Face::Face(Point* p1, Point* p2, Point* p3, Point* p4) {

	countOfUsing = 2;
	p[0] = p1;
	p[1] = p2;
	p[2] = p3;
	p[3] = p4;
}

void Face::initFace(Edge* e1, Edge* e2, Edge* e3, Edge* e4)
{
	e[0] = e1;
	e[1] = e2;
	e[2] = e3;
	e[3] = e4;
}

///////////////////


Cell::Cell(list<Edge*> &edges, list<Face*> &faces, Point* p1, Point* p2, Point* p3, Point* p4, Point* p5, Point* p6, Point* p7, Point* p8) {

	p[0] = p1;
	p[1] = p2;
	p[2] = p3;
	p[3] = p4;
	p[4] = p5;
	p[5] = p6;
	p[6] = p7;
	p[7] = p8;

	e[0] = getEdge(edges, p[0], p[1]);
	e[1] = getEdge(edges, p[1], p[2]);
	e[2] = getEdge(edges, p[2], p[3]);
	e[3] = getEdge(edges, p[3], p[0]);
	e[4] = getEdge(edges, p[4], p[5]);
	e[5] = getEdge(edges, p[5], p[6]);
	e[6] = getEdge(edges, p[6], p[7]);
	e[7] = getEdge(edges, p[7], p[4]);
	e[8] = getEdge(edges, p[0], p[4]);
	e[9] = getEdge(edges, p[1], p[5]);
	e[10] = getEdge(edges, p[2], p[6]);
	e[11] = getEdge(edges, p[3], p[7]);

	f[0] = getFace(faces, p[0], p[1], p[4], p[5]);
	f[1] = getFace(faces, p[1], p[2], p[5], p[6]);
	f[2] = getFace(faces, p[2], p[3], p[6], p[7]);
	f[3] = getFace(faces, p[0], p[3], p[4], p[7]);
	f[4] = getFace(faces, p[0], p[1], p[2], p[3]);
	f[5] = getFace(faces, p[4], p[5], p[6], p[7]);

	if (f[0]->countOfUsing == 1)
		f[0]->initFace(e[0], e[9], e[4], e[8]);
	if (f[1]->countOfUsing == 1)
		f[1]->initFace(e[1], e[10], e[5], e[9]);
	if (f[2]->countOfUsing == 1)
		f[2]->initFace(e[2], e[11], e[6], e[10]);
	if (f[3]->countOfUsing == 1)
		f[3]->initFace(e[3], e[8], e[7], e[11]);
	if (f[4]->countOfUsing == 1)
		f[4]->initFace(e[0], e[1], e[2], e[3]);
	if (f[5]->countOfUsing == 1)
		f[5]->initFace(e[4], e[5], e[6], e[7]);
}


Edge* Cell::getEdge(list<Edge*> &edges, Point* p1, Point* p2) {

	for (list<Edge*>::iterator it = edges.begin(); it != edges.end(); ++it) {
		if (((*it)->p[0] == p1 && (*it)->p[1] == p2) || ((*it)->p[0] == p2 && (*it)->p[1] == p1))
		{
			(*it)->countOfUsing--;
			if ((*it)->countOfUsing == 0) {

				Edge* result = *it;
				list<Edge*>::iterator it_temp = it;
				++it;
				edges.erase(it_temp);

				while (it != edges.end() && result->countOfUsing < (*it)->countOfUsing) {
					++it;
				}

				edges.insert(it, result);
				return result;
			}
			else
				return (*it);
		}
	}
}


Face* Cell::getFace(list<Face*> &faces, Point* p1, Point* p2, Point* p3, Point* p4) {

	set<Point*> searchFace;

	searchFace.insert(p1);
	searchFace.insert(p2);
	searchFace.insert(p3);
	searchFace.insert(p4);

	set<Point*> tempFace;

	for (list<Face*>::iterator it = faces.begin(); it != faces.end(); ++it) {
		tempFace.clear();
		tempFace.insert((*it)->p[0]);
		tempFace.insert((*it)->p[1]);
		tempFace.insert((*it)->p[2]);
		tempFace.insert((*it)->p[3]);

		if (tempFace == searchFace) {

			(*it)->countOfUsing--;

			if ((*it)->countOfUsing == 0) {

				Face* result = *it;
				list<Face*>::iterator it_temp = it;
				++it;
				faces.erase(it_temp);

				while (it != faces.end() && result->countOfUsing < (*it)->countOfUsing) {
					++it;
				}

				faces.insert(it, result);
				return result;
			}
			else
				return (*it);
		}
	}
}

////////////////////

Mesh::Mesh(Point* p, unsigned int pCount) {

	this->pCount = pCount;
	points = new Point[pCount];
	for (unsigned int i = 0; i < pCount; i++) {
		points[i] = p[i];
	}
}

Mesh::~Mesh() {
	delete[] points;

	for (list<Edge*>::iterator it = edges.begin(); it != edges.end(); ++it)
		delete (*it);

	for (list<Face*>::iterator it = faces.begin(); it != faces.end(); ++it)
		delete (*it);

	for (vector<Cell*>::iterator it = cells.begin(); it != cells.end(); ++it)
		delete (*it);
}

void Mesh::createEdge(int ind1, int ind2) {
	edges.push_back(new Edge(&points[ind1], &points[ind2]));
}

void Mesh::createFace(int ind1, int ind2, int ind3, int ind4) {
	faces.push_back(new Face(&points[ind1], &points[ind2], &points[ind3], &points[ind4]));
}

void Mesh::createCell(int ind1, int ind2, int ind3, int ind4, int ind5, int ind6, int ind7, int ind8) {
	cells.push_back(new Cell(edges, faces, &points[ind1], &points[ind2], &points[ind3], &points[ind4], &points[ind5], &points[ind6], &points[ind7], &points[ind8]));
}

CellIterator Mesh::beginCell() { return CellIterator(cells.begin()); }
CellIterator Mesh::endCell() { return CellIterator(cells.end()); }

PointIterator Mesh::beginPoint() { return PointIterator(points); }
PointIterator Mesh::endPoint() { return PointIterator(&points[pCount+1]); }