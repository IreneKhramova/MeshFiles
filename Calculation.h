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

    void calc_heat_equation(double t_max); // ��������� ����������� �� ����� double t
    void save(const char *filename); // ������ � vtk
};

#endif // CALCULATION_H