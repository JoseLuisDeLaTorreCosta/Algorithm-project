#ifndef NTUPLE_H
#define NTUPLE_H

#include <vector>
#include <random>

/*
Potser se m'ha anat una mica fent una classe NTuple, però conté el codi de generar-ne
aleatòries i permet limitar l'accés a les dades (no es poden modificar)
*/

class NTuple
{
public:
    // Constructor per defecte, genera una NTuple de dimensió 0
    NTuple();

    // Constructor, genera una NTuple aleatòria de dimensió dim amb el generador gen
    NTuple(int dim, std::mt19937 &gen);

    // Operador [] per accedir a les dades de la NTuple sense permís de modificació
    const double &operator[](int index) const;

    // Calcula la distància euclidiana entre dues NTuples
    double distance(const NTuple &other) const;

private:
    std::vector<double> data;
};
#endif