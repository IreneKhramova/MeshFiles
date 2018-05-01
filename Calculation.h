#ifndef CALCULATION_H
#define CALCULATION_H

#include "Mesh.h"

class Calculation
{
private:
    Mesh* msh;
    vector<string> bndNames;
public:
    Calculation(Mesh* m) : msh(m) {}
    ~Calculation() {}

    void init(const char *filename);
    void calc_heat_equation(double t_max); // изменение температуры за время t_max
    void save(const char *filename, const char *header); // запись в vtk
};

#endif // CALCULATION_H