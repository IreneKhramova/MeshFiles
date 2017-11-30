#include "Mesh.h"

///////////////////////

Edge::Edge(Point* p1, Point* p2)
{
    countOfUsing = 4;
    p[0] = p1;
    p[1] = p2;
}

double Edge::getlength()
{
    return sqrt( pow(p[0]->x - p[1]->x, 2) + pow(p[0]->y - p[1]->y, 2) + pow(p[0]->z - p[1]->z, 2) );
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
    delete [] p;
    delete [] e;
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
  if(countOfUsing == 1) { // инициализируем в первый раз

    e[0] = e1;
    e[1] = e2;
    e[2] = e3;

    this->c[0] = c;
 }
 else  {
    this->c[1] = c;
 }
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
  if(countOfUsing == 1) { // инициализируем в первый раз

    e[0] = e1;
    e[1] = e2;
    e[2] = e3;
    e[3] = e4;

    this->c[0] = c;
 }
 else  {
    this->c[1] = c;
 }
}

// Вычисление площади грани
void Face::area()
{
    S = 0;
     switch( type )
     {
      case 91: // треугольник( код формата unv - 91)
      {
            double a = e[0]->getlength();
            double b = e[1]->getlength();
            double c = e[2]->getlength();

            double pp = (a + b + c)/2;
            S = sqrt(pp*(pp-a)*(pp-b)*(pp-c));
        break;
      }
      case 94: // четырехугольник ( код формата unv - 94)
      {
            double a = e[0]->getlength();
            double b = e[1]->getlength();
            double c = e[2]->getlength();
            double d = e[3]->getlength();

            double p1 = (a + b + c)/2;
            double p2 = (a + d + c)/2;

            S = sqrt(p1*(p1-a)*(p1-b)*(p1-c)) + sqrt(p2*(p2-a)*(p2-d)*(p2-c));
        break;
      }
    }
}

///////////////////

Edge* Cell::getEdge(list<Edge*> &edges, Point* p1, Point* p2)
{
        for(list<Edge*>::iterator it = edges.begin(); it != edges.end(); ++it)
        {
            if( ( (*it)->p[0] == p1 && (*it)->p[1] == p2) || ((*it)->p[0] == p2 && (*it)->p[1] == p1))
            {
                (*it)->countOfUsing--;
                if( (*it)->countOfUsing == 0)
                {
                    Edge* result = *it;
                    list<Edge*>::iterator it_temp = it;
                    ++it;
                    edges.erase(it_temp);

                    while(it != edges.end() && result->countOfUsing < (*it)->countOfUsing)
                    {
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

Face* Cell::getFace(list<Face*> &faces, Point* p1, Point* p2, Point* p3, Point* p4)
{
        set<Point*> searchFace;

        searchFace.insert(p1);
        searchFace.insert(p2);
        searchFace.insert(p3);
        searchFace.insert(p4);

        set<Point*> tempFace;

        for(list<Face*>::iterator it = faces.begin(); it != faces.end(); ++it)
        {
            if( (*it)->pCount != 4)
                continue;

            tempFace.clear();
            tempFace.insert((*it)->p[0]);
            tempFace.insert((*it)->p[1]);
            tempFace.insert((*it)->p[2]);
            tempFace.insert((*it)->p[3]);

            if( tempFace == searchFace )
            {
                (*it)->countOfUsing--;

                if( (*it)->countOfUsing == 0)
                {
                    Face* result = *it;
                    list<Face*>::iterator it_temp = it;
                    ++it;
                    faces.erase(it_temp);

                    while(it != faces.end() && result->countOfUsing < (*it)->countOfUsing)
                    {
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

Face* Cell::getFace(list<Face*> &faces, Point* p1, Point* p2, Point* p3)
{
        set<Point*> searchFace;

        searchFace.insert(p1);
        searchFace.insert(p2);
        searchFace.insert(p3);

        set<Point*> tempFace;

        for(list<Face*>::iterator it = faces.begin(); it != faces.end(); ++it)
        {
            if( (*it)->pCount != 3)
                continue;

            tempFace.clear();
            tempFace.insert((*it)->p[0]);
            tempFace.insert((*it)->p[1]);
            tempFace.insert((*it)->p[2]);

            if( tempFace == searchFace )
            {
                 (*it)->countOfUsing--;

                if((*it)->countOfUsing == 0)
                {
                    Face* result = *it;
                    list<Face*>::iterator it_temp = it;
                    ++it;
                    faces.erase(it_temp);

                    while(it != faces.end() && (result)->countOfUsing < (*it)->countOfUsing)
                    {
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


Cell::~Cell()
{
    delete [] p;
    delete [] e;
    delete [] f;
}

Cell::Cell(list<Edge*> &edges, list<Face*> &faces, int& type, Point* p1, Point* p2, Point* p3, Point* p4)
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

    e[0] = getEdge(edges, p[0], p[1]);
    e[1] = getEdge(edges, p[1], p[2]);
    e[2] = getEdge(edges, p[0], p[2]);
    e[3] = getEdge(edges, p[0], p[3]);
    e[4] = getEdge(edges, p[1], p[3]);
    e[5] = getEdge(edges, p[2], p[3]);

    f[0] = getFace(faces, p[0], p[1], p[2]);
    f[1] = getFace(faces, p[0], p[1], p[3]);
    f[2] = getFace(faces, p[0], p[2], p[3]);
    f[3] = getFace(faces, p[1], p[2], p[3]);

    f[0]->initFace(this, e[0], e[1], e[2]);
    f[1]->initFace(this, e[0], e[4], e[3]);
    f[2]->initFace(this, e[2], e[5], e[3]);
    f[3]->initFace(this, e[1], e[5], e[4]);
}


Cell::Cell(list<Edge*> &edges, list<Face*> &faces, int& type, Point* p1, Point* p2, Point* p3, Point* p4, Point* p5, Point* p6, Point* p7, Point* p8)
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

    f[0]->initFace(this, e[0], e[9], e[4], e[8]);
    f[1]->initFace(this, e[1], e[10], e[5], e[9]);
    f[2]->initFace(this, e[2], e[11], e[6], e[10]);
    f[3]->initFace(this, e[3], e[8], e[7], e[11]);
    f[4]->initFace(this, e[0], e[1], e[2], e[3]);
    f[5]->initFace(this, e[4], e[5], e[6], e[7]);
}

// Вычисление объема клетки
void Cell::volume()
{
    V = 0;

    switch( type )
    {

     case 111: // 111 - тетраэдр
        {

        Point vec_a = Edge::getVector(p[0], p[1]);
        Point vec_b = Edge::getVector(p[0], p[2]);
        Point vec_c = Edge::getVector(p[0], p[3]);

        V = abs( vec_a.x*(vec_b.y*vec_c.z - vec_b.z*vec_c.y) - vec_a.y*(vec_b.x*vec_c.z - vec_b.z*vec_c.x) + vec_a.z*(vec_b.x*vec_c.y - vec_b.y*vec_c.x) ) / 3;

        break;
        }
     case 115: // 115 - шестигранник
        {

        Point vec_17 = Edge::getVector(p[1], p[7]);
        Point vec_14 = Edge::getVector(p[1], p[4]);
        Point vec_10 = Edge::getVector(p[1], p[0]);

        V += abs( vec_17.x*(vec_14.y*vec_10.z - vec_14.z*vec_10.y) - vec_17.y*(vec_14.x*vec_10.z - vec_14.z*vec_10.x) + vec_17.z*(vec_14.x*vec_10.y - vec_14.y*vec_10.x) ) / 3;

        Point vec_12 = Edge::getVector(p[1], p[2]);
        Point vec_16 = Edge::getVector(p[1], p[6]);

        V += abs( vec_17.x*(vec_12.y*vec_16.z - vec_12.z*vec_16.y) - vec_17.y*(vec_12.x*vec_16.z - vec_12.z*vec_16.x) + vec_17.z*(vec_12.x*vec_16.y - vec_12.y*vec_16.x) ) / 3;

        Point vec_56 = Edge::getVector(p[5], p[6]);
        Point vec_54 = Edge::getVector(p[5], p[4]);

        Point vec_norm;

        vec_norm.x = vec_56.y * vec_54.z - vec_56.z * vec_54.y;
        vec_norm.y = - vec_56.x * vec_54.z + vec_56.z * vec_54.x;
        vec_norm.z = vec_56.x * vec_54.y - vec_56.y * vec_54.x;

        double norm = sqrt(vec_norm.x * vec_norm.x + vec_norm.y * vec_norm.y + vec_norm.z * vec_norm.z);

        Point vec_51 = Edge::getVector(p[5], p[1]);
        double h = abs( vec_norm.x * vec_51.x + vec_norm.y * vec_51.y + vec_norm.z * vec_51.z ) / norm;

        f[5]->area();

        V += f[5]->S * h / 3;

        vec_norm.x = vec_10.y * vec_12.z - vec_10.z * vec_12.y;
        vec_norm.y = - vec_10.x * vec_12.z + vec_10.z * vec_12.x;
        vec_norm.z = vec_10.x * vec_12.y - vec_10.y * vec_12.x;

        norm = sqrt(vec_norm.x * vec_norm.x + vec_norm.y * vec_norm.y + vec_norm.z * vec_norm.z);

        h = abs( vec_norm.x * vec_17.x + vec_norm.y * vec_17.y + vec_norm.z * vec_17.z ) / norm;

        f[4]->area();

        V += f[4]->S * h / 3;

        break;
        }
    }
}

////////////////////

Mesh::Mesh(Point* p, unsigned int pCount)
{
        this->pCount = pCount;
        points = new Point[pCount];
        for(int i = 0; i < pCount; i++)
        {
            points[i] = p[i];
        }
    }

Mesh::Mesh(const vector<Point>& p)
{
        this->pCount = p.size();
        points = new Point[pCount];
        for(int i = 0; i < pCount; i++)
        {
            points[i] = p[i];
        }
}

Mesh::~Mesh()
{
        delete [] points;

        for(list<Edge*>::iterator it = edges.begin(); it != edges.end(); ++it)
            delete (*it);

         for(list<Face*>::iterator it = faces.begin(); it != faces.end(); ++it)
            delete (*it);

         for(vector<Cell*>::iterator it = cells.begin(); it != cells.end(); ++it)
            delete (*it);
    }

void Mesh::createEdge(int ind1, int ind2)
{
        edges.push_back( new Edge(&points[ind1], &points[ind2]) );
}

// Создание треугольника
void Mesh::createFace(int type, int ind1, int ind2 ,int ind3)
{
        faces.push_back( new Face(type, &points[ind1], &points[ind2], &points[ind3]) );
}

// Создание четырехугольника
void Mesh::createFace(int type, int ind1, int ind2 ,int ind3 ,int ind4)
{
        faces.push_back( new Face(type, &points[ind1], &points[ind2], &points[ind3], &points[ind4]) );
}

// Создание тетраэдра
void Mesh::createCell(int type, int ind1, int ind2, int ind3, int ind4)
{
        cells.push_back( new Cell(edges, faces, type, &points[ind1], &points[ind2], &points[ind3], &points[ind4]));
}

// Создание шестиугольника
void Mesh::createCell(int type, int ind1, int ind2, int ind3, int ind4, int ind5, int ind6, int ind7, int ind8)
{
        cells.push_back( new Cell(edges, faces, type, &points[ind1], &points[ind2], &points[ind3], &points[ind4], &points[ind5], &points[ind6], &points[ind7], &points[ind8]) );
}

////////////////////

void vtkWriteUnstructuredGrid(const char *filename, Mesh* mesh)
{
	FILE *out;
	out = fopen(filename, "w");
	fprintf(out, "# vtk DataFile Version 3.0\n");
	fprintf(out, "Mesh_test\n");
	fprintf(out, "ASCII\n");
	fprintf(out, "DATASET UNSTRUCTURED_GRID\n");
	fprintf(out, "POINTS %d double\n", mesh->pCount);
	for (int i = 0; i < mesh->pCount; i++)
	{
		fprintf(out, "%f %f %f\n", mesh->points[i].x, mesh->points[i].y, mesh->points[i].z);
	}

	int cellCount = mesh->cells.size();

	int cellSize=0;//the size of the cell list (count of points in all cells)
	for (int i = 0; i < cellCount; i++)
	{
		cellSize += mesh->cells[i]->pCount;
	}

	/*
		cellSize + cellCount :
		cellSize + one number for each cell - count of points in this cell
	*/
	fprintf(out, "CELLS %d %d\n", cellCount, cellSize + cellCount);
	for (int i = 0; i < cellCount; i++)
	{
		fprintf(out, "%d", mesh->cells[i]->pCount);
		for (int k = 0; k < mesh->cells[i]->pCount; k++)
		{
			int ind = -1;
			Point* addr = mesh->cells[i]->p[k];
			for (int j = 0; j < mesh->pCount; j++)
			{
				if (&(mesh->points[j]) == addr)
					ind = j;
			}
			fprintf(out, " %d", ind);
		}
		fprintf(out, "\n");
	}
	fprintf(out, "CELL_TYPES %d\n", cellCount);
	for (int i = 0; i < cellCount; i++)
	{
		fprintf(out, "12\n"); //12 - VTK_HEXAHEDRON
	}
	fclose(out);
}
