#ifndef STKDTREE_H
#define STKDTREE_H

#include "../ntuple.h"
#include "../bkdtree.h"
#include "../ikdtree.h"

// Standard K Dimensional Tree
class StKDTree : public IKDTree
{
public:
    // Constructora, crea un arbre estandard buit de dimensió dim
    StKDTree(int dim);

    // Destructora
    ~StKDTree();

    // Funcions de IKDTree que cal implementar
    void clear() override;
    void insert(const NTuple &tuple) override;
    int cost_nearest_neighbor(NTuple &tuple) override;
    void print_tree() override;

private:
    BKDTree bkd_tree;
};
#endif

