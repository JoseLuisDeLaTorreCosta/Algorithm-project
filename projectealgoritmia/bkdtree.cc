#include "bkdtree.h"
#include "ntuple.h"
#include <algorithm>
#include <iostream>

BKDTree::BKDTree(int dim)
{
    this->dim = dim;
    this->root = nullptr;
}

BKDTree::~BKDTree() { clear(); }

void BKDTree::clear() {
    clear(root);
    root = nullptr;
}

int BKDTree::cost_nearest_neighbor(NTuple &query)
{
    const NTuple *best_guess = &this->root->tuple;
    return this->root->cost_nearest_neighbor(query, best_guess);
}

int BKDTree::test_cost_nearest_neighbor(NTuple &query)
{
    const NTuple *best_guess = &this->root->tuple;
    int cost = this->root->cost_nearest_neighbor(query, best_guess);
    const NTuple *best_guess2 = &this->root->tuple;
    int cost2 = this->root->cost_nearest_neighbor_brute_force(query, best_guess2);
    if (best_guess != best_guess2)
    {
        std::cout << "ERROR" << std::endl;
    }
    return cost;
}

int BKDTree::KDTreeNode::cost_nearest_neighbor(NTuple &query,
                                               const NTuple *&best_guess)
{
    int cost = 1;
    if (query.distance(tuple) < query.distance(*best_guess))
    {
        best_guess = &tuple;
    }
    if (tuple[discriminant] > query[discriminant])
    {
        if (left != nullptr)
        {
            cost += left->cost_nearest_neighbor(query, best_guess);
        }
        if (right != nullptr and tuple[discriminant] - query[discriminant] <
                                     query.distance(*best_guess))
        {
            cost += right->cost_nearest_neighbor(query, best_guess);
        }
    }
    else
    {
        if (right != nullptr)
        {
            cost += right->cost_nearest_neighbor(query, best_guess);
        }
        if (left != nullptr and query[discriminant] - tuple[discriminant] <
                                    query.distance(*best_guess))
        {
            cost += left->cost_nearest_neighbor(query, best_guess);
        }
    }
    return cost;
}

void BKDTree::print_tree()
{
    std::cout << "Dimensions: " << dim << std::endl;
    print_tree(root, 0);
}

int BKDTree::KDTreeNode::cost_nearest_neighbor_brute_force(
    NTuple &query, const NTuple *&best_guess)
{
    int cost = 1;
    if (&tuple != &query and
        (best_guess == nullptr or
         query.distance(tuple) < query.distance(*best_guess)))
    {
        best_guess = &tuple;
    }
    if (left != nullptr)
    {
        cost += left->cost_nearest_neighbor(query, best_guess);
    }
    if (right != nullptr)
    {
        cost += right->cost_nearest_neighbor(query, best_guess);
    }
    return cost;
}

// static
void BKDTree::clear(KDTreeNode *node)
{
    if (node != nullptr)
    {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

void BKDTree::print_tree(KDTreeNode *node, int level)
{
    if (node == nullptr)
        return;
    print_tree(node->left, level + 1);

    std::cout << "Nivell: " << level << " "
              << "Clau: ";
    for (int i = 0; i < this->dim; ++i)
    {
        std::cout << node->tuple[i] << " ";
    }
    std::cout << "Discriminant: " << node->discriminant << std::endl;
    print_tree(node->right, level + 1);
}