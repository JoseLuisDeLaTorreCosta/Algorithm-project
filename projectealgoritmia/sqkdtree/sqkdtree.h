#ifndef SQKDTREE_H
#define SQKDTREE_H

#include "../ntuple.h"
#include "../bkdtree.h"
#include "../ikdtree.h"
#include <vector>

// Squarish K Dimensional Tree
class SqKDTree : public IKDTree
{
public:
    // Constructora, crea un arbre squarish buit de dimensió dim
    SqKDTree(int dim);

    // Destructora
    ~SqKDTree();

    // Funcions de IKDTree que cal implementar
    void clear() override;
    void insert(const NTuple &tuple) override;
    int cost_nearest_neighbor(NTuple &tuple) override;
    void print_tree() override;

private:
    BKDTree bkd_tree;
    int return_biggest_bb_dim(const std::vector<std::pair<double, double>> &bb);
};
#endif