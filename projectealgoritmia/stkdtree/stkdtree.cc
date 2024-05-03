#include "stkdtree.h"
#include "../ntuple.h"
#include "../bkdtree.h"
#include <algorithm>
#include <iostream>

StKDTree::StKDTree(int dim) : bkd_tree(dim){};

StKDTree::~StKDTree() = default;

void StKDTree::clear()
{
    bkd_tree.clear();
}

void StKDTree::insert(const NTuple &tuple)
{
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
                if (node->left == nullptr)
                {
                    node->left = new BKDTree::KDTreeNode((node->discriminant + 1) % bkd_tree.dim, tuple);
                    break;
                }
                else
                {
                    node = node->left;
                }
            }
            else
            {
                if (node->right == nullptr)
                {
                    node->right = new BKDTree::KDTreeNode((node->discriminant + 1) % bkd_tree.dim, tuple);
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

int StKDTree::cost_nearest_neighbor(NTuple &query)
{
    return bkd_tree.cost_nearest_neighbor(query);
}

void StKDTree::print_tree()
{
    return bkd_tree.print_tree();
}