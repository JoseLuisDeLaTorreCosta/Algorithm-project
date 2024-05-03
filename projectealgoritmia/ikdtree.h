#ifndef IKDTREE_H
#define IKDTREE_H

#include "ntuple.h"

// Interface K Dimensional Tree
class IKDTree
{
public:
    // Destructor
    virtual ~IKDTree(){};

    // Esborra tot l'arbre en O(n), on n és la quantitat de nodes
    virtual void clear() = 0;

    // Inserta una tupla a l'arbre
    virtual void insert(const NTuple &tuple) = 0;

    // Calcula el cost de cerca de la tupla més propera a tuple
    virtual int cost_nearest_neighbor(NTuple &tuple) = 0;

    // Imprimeix l'arbre en inordre
    virtual void print_tree() = 0;
};
#endif