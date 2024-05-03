#include "rkdtree.h"
#include <algorithm>
#include <iostream>
#include <random>

RKDTree::RKDTree(int dim, std::mt19937 &gen) : bkd_tree(dim), gen(gen){};

RKDTree::~RKDTree()  = default;

void RKDTree::clear()
{
    bkd_tree.clear();
}

void RKDTree::insert(const NTuple &tuple)
{
    std::uniform_int_distribution<int> disc(0, bkd_tree.dim - 1);
    if (bkd_tree.root == nullptr)
    {
        bkd_tree.root = new BKDTree::KDTreeNode(disc(gen), tuple);
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
                    node->left = new BKDTree::KDTreeNode(disc(gen), tuple);
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
                    node->right = new BKDTree::KDTreeNode(disc(gen), tuple);
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

int RKDTree::cost_nearest_neighbor(NTuple &query)
{
    return bkd_tree.cost_nearest_neighbor(query);
}

void RKDTree::print_tree()
{
    return bkd_tree.print_tree();
}