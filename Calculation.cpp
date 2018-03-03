#include "Calculation.h"
#include "MeshIterator.h"
#include "FilterIterator.h"

void Calculation::calc_heat_equation(double t_max)
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

        for (Mesh::FaceIterator it = msh->beginBndFace("top"), ite = msh->endBndFace("top"); it != ite; ++it)
        {
            it->c[0]->T += (it->S*(393 - it->c[0]->T) / it->h)*tau / it->c[0]->V;
        }

        for (Mesh::InnerFaceIterator it = msh->beginInnerFace(), ite = msh->endInnerFace(); it != ite; ++it)
        {
            temp_value = it->S*(it->c[1]->T - it->c[0]->T) / it->h;
            it->c[0]->T += temp_value*tau / it->c[0]->V;
            it->c[1]->T -= temp_value*tau / it->c[1]->V;
        }
    }
}

void Calculation::save(const char *filename)
{
    FILE *out;
    out = fopen(filename, "w");
    fprintf(out, "# vtk DataFile Version 3.0\n");
    fprintf(out, "Mesh\n");
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