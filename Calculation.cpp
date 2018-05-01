#include "Calculation.h"
#include "MeshIterator.h"
#include "FilterIterator.h"
#include "BndIterator.h"
#include "tinyxml2.h"

void Calculation::init(const char *filename)
{
    tinyxml2::XMLDocument xmlDoc;
    tinyxml2::XMLError eResult = xmlDoc.LoadFile(filename);
    if (eResult != tinyxml2::XML_SUCCESS)
    {
        printf("XMLERROR is %d\nXML loading unsuccessfull.\n", eResult);
        exit(eResult);
    }

    /* Температура для всех cell */
    tinyxml2::XMLElement* pNode1 = xmlDoc.FirstChildElement("regions");
    if (pNode1 == nullptr)
    {
        printf("XMLERROR: No such FirstChildElement.\n");
        exit(1);
    }
    tinyxml2::XMLElement* pNode2 = pNode1->FirstChildElement("region");
    if (pNode2 == nullptr)
    {
        printf("XMLERROR: No such FirstChildElement.\n");
        exit(1);
    }
    tinyxml2::XMLElement* pNode3 = pNode2->FirstChildElement("parameters");
    if (pNode3 == nullptr)
    {
        printf("XMLERROR: No such FirstChildElement.\n");
        exit(1);
    }
    tinyxml2::XMLElement* pNode4 = pNode3->FirstChildElement("T");
    if (pNode4 == nullptr)
    {
        printf("XMLERROR: No such FirstChildElement.\n");
        exit(1);
    }
    double t;
    eResult = pNode4->QueryDoubleAttribute("value", &t);
    if (eResult != tinyxml2::XML_SUCCESS)
    {
        printf("XMLERROR is %d\nXML loading unsuccessfull.\n", eResult);
        exit(eResult);
    }

    for (Mesh::CellIterator it = msh->beginCell(), ite = msh->endCell(); it != ite; ++it)
    {
        it->T = t;
    }

    /* Температура для заданных face */
    tinyxml2::XMLElement* pBnd = xmlDoc.FirstChildElement("boundaries");
    if (pBnd == nullptr)
    {
        printf("XMLERROR: No such FirstChildElement.\n");
        exit(1);
    }
    tinyxml2::XMLElement* pBndElement = pBnd->FirstChildElement("boundCond");
    if (pBndElement == nullptr)
    {
        printf("XMLERROR: No such FirstChildElement.\n");
        exit(1);
    }
    while (pBndElement != nullptr)
    {
        string str;
        tinyxml2::XMLElement* pName = pBndElement->FirstChildElement("name");
        if (pName == nullptr)
        {
            printf("XMLERROR: No such FirstChildElement.\n");
            exit(1);
        }
        str = pName->GetText();

        tinyxml2::XMLElement* pPar = pBndElement->FirstChildElement("parameters");
        if (pPar == nullptr)
        {
            printf("XMLERROR: No such FirstChildElement.\n");
            exit(1);
        }
        tinyxml2::XMLElement* pT = pPar->FirstChildElement("T");
        if (pT == nullptr)
        {
            printf("XMLERROR: No such FirstChildElement.\n");
            exit(1);
        }
        double temp;
        eResult = pT->QueryDoubleAttribute("value", &temp);
        if (eResult != tinyxml2::XML_SUCCESS)
        {
            printf("XMLERROR is %d\nXML loading unsuccessfull.\n", eResult);
            exit(eResult);
        }
        for (Mesh::FaceIterator it = msh->beginBndFace(str), ite = msh->endBndFace(str); it != ite; ++it)
        {
            it->fixedT = temp;
        }

        bndNames.push_back(str);

        pBndElement = pBndElement->NextSiblingElement("boundCond");
    }
}


void Calculation::calcHeatEquation(double t_max)
{
    double min_volume = msh->cells[0]->V;
    for (Mesh::CellIterator it = msh->beginCell(), ite = msh->endCell(); it != ite; ++it)
    {
        double vol = it->V;
        if (min_volume > vol)
            min_volume = vol;
    }

    double tau = min_volume / 2.1;

    double temp_value;
    double t = 0;

    while (t < t_max)
    {
        t += tau;
        for (Mesh::BndFaceIterator it = msh->beginBndFace(&(msh->bnd_faces), &bndNames), ite = msh->endBndFace(&(msh->bnd_faces), &bndNames); it != ite; ++it)
        {
            it->c[0]->T += (it->S*(it->fixedT - it->c[0]->T) / it->h)*tau / it->c[0]->V;
        }
        for (Mesh::InnerFaceIterator it = msh->beginInnerFace(), ite = msh->endInnerFace(); it != ite; ++it)
        {
            temp_value = it->S*(it->c[1]->T - it->c[0]->T) / it->h;
            it->c[0]->T += temp_value*tau / it->c[0]->V;
            it->c[1]->T -= temp_value*tau / it->c[1]->V;
        }
    }
}

void Calculation::save(const char *filename, const char *header)
{
    FILE *out;
    out = fopen(filename, "w");
    fprintf(out, "# vtk DataFile Version 3.0\n");
    //The header can be used to describe the data
    fprintf(out, "%s\n", header);
    fprintf(out, "ASCII\n");
    fprintf(out, "DATASET UNSTRUCTURED_GRID\n");
    fprintf(out, "POINTS %d double\n", msh->pCount);
    for (int i = 0; i < msh->pCount; i++)
    {
        fprintf(out, "%f %f %f\n", msh->points[i].x, msh->points[i].y, msh->points[i].z);
    }

    int cellCount = msh->cells.size();

    int cellSize = 0;//the size of the cell list (count of points in all cells)
    for (int i = 0; i < cellCount; i++)
    {
        cellSize += msh->cells[i]->pCount;
    }

    /*
    cellSize + cellCount :
    cellSize + one number for each cell - count of points in this cell
    */
    fprintf(out, "CELLS %d %d\n", cellCount, cellSize + cellCount);
    for (int i = 0; i < cellCount; i++)
    {
        fprintf(out, "%d", msh->cells[i]->pCount);
        for (int k = 0; k < msh->cells[i]->pCount; k++)
        {
            int ind = -1;
            Point* addr = msh->cells[i]->p[k];
            for (int j = 0; j < msh->pCount; j++)
            {
                if (&(msh->points[j]) == addr)
                    ind = j;
            }
            fprintf(out, " %d", ind);
        }
        fprintf(out, "\n");
    }
    fprintf(out, "CELL_TYPES %d\n", cellCount);
    for (int i = 0; i < cellCount; i++)
    {
        switch (msh->cells[i]->pCount)
        {
        case 4:
        {
            fprintf(out, "10\n"); //10 - VTK_TETRA
            break;
        }
        case 6:
        {
            fprintf(out, "13\n"); //13 - VTK_WEDGE
            break;
        }
        case 8:
        {
            fprintf(out, "12\n"); //12 - VTK_HEXAHEDRON
            break;
        }
        }

    }
    fprintf(out, "CELL_DATA %d\nSCALARS temperature double 1\nLOOKUP_TABLE default\n", cellCount);
    for (int i = 0; i < cellCount; i++)
    {
        fprintf(out, "%f\n", msh->cells[i]->T);
    }
    fclose(out);
}