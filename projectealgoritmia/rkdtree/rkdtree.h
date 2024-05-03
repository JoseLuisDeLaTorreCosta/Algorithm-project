#ifndef RKDTREE_H
#define RKDTREE_H

#include "../ntuple.h"
#include "../bkdtree.h"
#include "../ikdtree.h"

// Random K Dimensional Tree
class RKDTree : public IKDTree
{
public:
    // Constructora, crea un arbre random buit de dimensió dim
    RKDTree(int dim, std::mt19937 &gen);

    // Destructora
    ~RKDTree();

    // Funcions de IKDTree que cal implementar
    void clear() override;
    void insert(const NTuple &tuple) override;
    int cost_nearest_neighbor(NTuple &tuple) override;
    void print_tree() override;

private:
    BKDTree bkd_tree;
    std::mt19937 &gen;
};
#endif