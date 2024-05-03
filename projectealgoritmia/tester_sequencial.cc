#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include "ikdtree.h"
#include "stkdtree/stkdtree.h"
#include "rkdtree/rkdtree.h"
#include "sqkdtree/sqkdtree.h"

const int SEED = 42;
const int N = 2000000;
const int DIM = 2;
const int Q = 100;

int main()
{
    // Preguntem quin tipus d'arbre volem provar
    std::cout << "Quin tipus d'arbre vols provar?" << std::endl;
    std::cout << "1. Standard KDTree" << std::endl;
    std::cout << "2. Random KDTree" << std::endl;
    std::cout << "3. Squarish KDTree" << std::endl;
    int tipus_arbre = 1;
    std::cin >> tipus_arbre;
    if (tipus_arbre < 1 or tipus_arbre > 3)
    {
        std::cerr << "Opció invàlida: L'input ha de formar de ser '1', '2' o '3'" << std::endl;
        return EXIT_FAILURE;
    }

    // Inicialitzem el generador de nombres aleatoris amb una seed qualsevol
    std::mt19937 gen;
    gen.seed(SEED);

    // Creem un vector de NTuples aleatòries
    std::vector<NTuple> v = std::vector<NTuple>(0);
    std::cout << "Generating " << N << " random tuples..." << std::endl;
    for (int i = 0; i < N; ++i)
    {
        v.push_back(NTuple(DIM, gen));
        if (i * 10 / N != (i - 1) * 10 / N)
        {
            std::cout << i * 100 / N << "%..." << std::endl;
        }
    }

    // Creem l'arbre i inserim les tuples
    std::unique_ptr<IKDTree> tree;
    switch (tipus_arbre)
    {
    case 1:
        tree = std::make_unique<StKDTree>(DIM);
        break;
    case 2:
        tree = std::make_unique<RKDTree>(DIM, gen);
        break;
    case 3:
        tree = std::make_unique<SqKDTree>(DIM);
        break;
    }
    std::cout << "Inserting " << N << " tuples..." << std::endl;
    for (int i = 0; i < N; ++i)
    {
        tree->insert(v[i]);
        if (i * 10 / N != (i - 1) * 10 / N)
        {
            std::cout << i * 100 / N << "%..." << std::endl;
        }
    }

    // Creem un vector de costos de mida Q i calculem el cost de cerca per a les primeres Q tuples
    std::vector<int> costs = std::vector<int>(Q);
    std::cout << "Calculating cost of nearest neighbor for " << Q << " tuples..." << std::endl;
    for (int i = 0; i < Q; ++i)
    {
        costs[i] = tree->cost_nearest_neighbor(v[i]);
        if (i * 10 / Q != (i - 1) * 10 / Q)
        {
            std::cout << i * 100 / Q << "%..." << std::endl;
        }
    }

    // Imprimim el promig dels costos
    std::cout << "Average cost: " << std::accumulate(costs.begin(), costs.end(), 0.0) / Q << std::endl;
    return EXIT_SUCCESS;
}