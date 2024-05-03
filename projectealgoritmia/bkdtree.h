#ifndef BKDTREE_H
#define BKDTREE_H

#include "ntuple.h"

// Base KDTree conté una estructura de dades bàsica per als arbres KD utilitzada
// per les implementacions d'aquests
class BKDTree
{
public:
    // Constructor, crea un arbre buit de dimensió dim i una tupla arrel
    BKDTree(int dim);

    // Destructor
    ~BKDTree();

    // Esborra tot l'arbre en O(n), on n és la quantitat de nodes
    void clear();

    // Calcula el cost de cerca de la tupla més propera a tuple
    int cost_nearest_neighbor(NTuple &tuple);

    // Imprimeix l'arbre en inordre
    void print_tree();

    // Nodes de l'arbre
    class KDTreeNode
    {
    public:
        KDTreeNode(int d, const NTuple &t) : tuple(t)
        {
            discriminant = d;
            left = nullptr;
            right = nullptr;
        }
        int cost_nearest_neighbor(NTuple &tuple, const NTuple *&best_guess);
        int cost_nearest_neighbor_brute_force(NTuple &query,
                                              const NTuple *&best_guess);

        int discriminant;
        KDTreeNode *left;
        KDTreeNode *right;
        NTuple tuple;
    };

    // Dimensions de l'arbre
    int dim;

    // Arrel de l'arbre
    KDTreeNode *root;

private:
    static void clear(KDTreeNode *node);
    void print_tree(KDTreeNode *node, int level);
    int test_cost_nearest_neighbor(NTuple &tuple);
};
#endif