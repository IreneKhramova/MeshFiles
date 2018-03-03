#ifndef CALCULATION_H
#define CALCULATION_H

#include "Mesh.h"

class Calculation
{
private:
    Mesh* msh;
public:
    Calculation(Mesh* m) : msh(m) {}
    ~Calculation() {}

    void calc_heat_equation(double t_max); // изменение температуры за время double t
    void save(const char *filename); // запись в vtk
};

#endif // CALCULATION_H