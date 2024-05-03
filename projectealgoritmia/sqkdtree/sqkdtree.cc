#include "sqkdtree.h"
#include "../ntuple.h"
#include <algorithm>
#include <iostream>

SqKDTree::SqKDTree(int dim) : bkd_tree(dim) {};

SqKDTree::~SqKDTree() = default;

void SqKDTree::clear()
{
    bkd_tree.clear();
}

void SqKDTree::insert(const NTuple &tuple)
{
    // La bounding box está representada en cada dimensió per un parell de doubles tals que {Límit inferior, Límit superior}
    std::vector<std::pair<double, double>> bb(bkd_tree.dim, {0, 1});

    if (bkd_tree.root == nullptr)
    {
        bkd_tree.root = new BKDTree::KDTreeNode(0, tuple);
    }
    else
    {
        BKDTree::KDTreeNode *node = bkd_tree.root;
        while (true)
        {
            if (tuple[node->discriminant] < node->tuple[node->discriminant])
            {
                bb[node->discriminant].second = node->tuple[node->discriminant];
                if (node->left == nullptr)
                {
                    node->left = new BKDTree::KDTreeNode(return_biggest_bb_dim(bb), tuple);
                    break;
                }
                else
                {
                    node = node->left;
                }
            }
            else
            {
                bb[node->discriminant].first = node->tuple[node->discriminant];
                if (node->right == nullptr)
                {
                    node->right = new BKDTree::KDTreeNode(return_biggest_bb_dim(bb), tuple);
                    break;
                }
                else
                {
                    node = node->right;
                }
            }
        }
    }
}

int SqKDTree::cost_nearest_neighbor(NTuple &query)
{
    return bkd_tree.cost_nearest_neighbor(query);
}

void SqKDTree::print_tree()
{
    return bkd_tree.print_tree();
}

//Funció per a trobar el costat de la bb més gran
int SqKDTree::return_biggest_bb_dim(const std::vector<std::pair<double, double>> &bb)
{
    int d = 0;
    double difference = 0;
    for (int i = 0; i < bb.size(); ++i)
    {
        double aux_diff = bb[i].second - bb[i].first;
        if (aux_diff > difference)
        {
            difference = aux_diff;
            d = i;
        }
    }
    return d;
}