#include <iostream>
#include <random>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include "ntuple.h"
#include "stkdtree/stkdtree.h"
#include "rkdtree/rkdtree.h"
#include "sqkdtree/sqkdtree.h"

const int SEED = 42;
#define N_DIM 5
#define SIZES 4

// Diferents k a provar
const int k[N_DIM] = {2, 3, 4, 5, 6};
// Diferents n a provar
const int n[SIZES] = {100, 1000, 10000, 100000}; //, 1000000, 10000000};
// Proves per arbre
const int Q = 50;
// Arbres per cada mida diferent
const int N = 50;
// Un per "thread"
std::vector<std::mt19937> gens;
// Per fer locks
std::mutex mutex;
std::function<void(int, int, int, int, std::vector<int> &, std::vector<bool> &)> task_function;

void init_random()
{
    gens = std::vector<std::mt19937>(std::thread::hardware_concurrency());
    for (int i = 0; i < gens.size(); ++i)
        gens[i].seed(SEED + i);
}

/*
    Fa els càlculs per un arbre k-dimensional estàndard, tenint en compte l'estructura paral·lela del programa
*/
void task_consulta(int tree_type, int dim, int size, int index, int pool_index, std::vector<int> &costs, std::vector<bool> &finished)
{
    std::unique_ptr<IKDTree> tree;
    std::mt19937 &gen = gens[pool_index];
    switch (tree_type)
    {
    case 0:
        tree = std::make_unique<StKDTree>(dim);
        break;
    case 1:
        tree = std::make_unique<RKDTree>(dim, gen);
        break;
    case 2:
        tree = std::make_unique<SqKDTree>(dim);
        break;
    default:
        tree = std::make_unique<StKDTree>(dim);
        break;
    }

    for (int i = 0; i < size; ++i)
        tree->insert(NTuple(dim, gen));

    for (int i = 0; i < Q; ++i)
    {
        NTuple tuple(dim, gen);
        costs[index + i] = tree->cost_nearest_neighbor(tuple);
    }

    mutex.lock();
    finished[pool_index] = true;
    mutex.unlock();
}

void usage()
{
    std::cerr << "Usage: ./tests [mode] [super]" << std::endl;
    std::cerr << "Available modes are:" << std::endl
              << std::endl;

    std::cerr << "std"
              << "       -     standard KDTree" << std::endl;
    std::cerr << "rand"
              << "      -     random KDTree (RKDTree)" << std::endl;
    std::cerr << "squarish"
              << "  -     squarish KDTree (SKDTree)" << std::endl;
    std::cerr << std::endl;
    std::cerr << "If super is used, the program will print the cost for each query - beware as this is a lot of data" << std::endl;
    std::cerr << "If it is not, it will print the mean cost and standard deviation for each tree size and dimension" << std::endl;

    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{

    init_random();
    int tree_type = 0;
    bool supertest = false;

    if (argc < 2 or argc > 3)
    {
        usage();
    }

    {
        std::string arg1 = argv[1];
        if (arg1 == "std")
            tree_type = 0;
        else if (arg1 == "rand")
            tree_type = 1;
        else if (arg1 == "squarish")
            tree_type = 2;
        else
        {
            usage();
        }
        if (argc > 2)
        {
            std::string arg2 = argv[2];
            if (arg2 == "super")
                supertest = true;
            else
                usage();
        }
    }

    if (supertest)
    {
        // Escriu els noms de les variables de cada columna
        std::cout << "k n cost" << std::endl;
    }

    for (int i = 0; i < N_DIM; ++i) // Itera sobre les dimensions
    {
        if (not supertest)
            std::cout << std::endl
                      << std::endl
                      << "k: " << k[i];
        for (int j = 0; j < SIZES; ++j) // Itera sobre les diferents mides d'arbre
        {
            int k_act = k[i];
            int n_act = n[j];
            if (not supertest)
                std::cout << std::endl
                          << "n: " << n_act << std::endl;

            std::vector<int> costs(N * Q);
            // Limitador de threads rudimentari
            std::vector<bool> finished(std::thread::hardware_concurrency(), true);

            for (int c = 0; c < N; ++c) // Itera sobre la quantitat d'arbres de mateixa mida
            {
                // Busca thread lliure i assigna tasca
                bool found = false;
                while (not found)
                {
                    for (int l = 0; l < finished.size(); ++l)
                    {
                        if (finished[l])
                        {
                            // Crea thread
                            mutex.lock();
                            finished[l] = false;
                            mutex.unlock();
                            std::thread t(task_consulta, tree_type, k_act, n_act, c * Q, l, std::ref(costs), std::ref(finished));
                            t.detach();
                            found = true;
                            break;
                        }
                    }
                }
            }

            // Espera a que acabin tots els threads
            bool threads_working = true;
            while (threads_working)
            {
                int threads_finished = 0;

                mutex.lock();
                for (bool b : finished)
                    if (b)
                        threads_finished++;
                if (threads_finished == finished.size())
                    threads_working = false;
                mutex.unlock();
            }

            if (supertest)
            {
                // Escriu els resultats pero colummnes amb el format k1 n1 cost1
                //                                                   k2 n2 cost2
                for (int d : costs)
                    std::cout << k_act << ' ' << n_act << ' ' << d << std::endl;
            }
            else
            {
                // Calcula la mitjana
                double average = 0;
                for (int d : costs)
                    average += d;
                average /= costs.size();

                // Calcula la variància
                double variance = 0;
                for (int d : costs)
                {
                    double difference = d - average;
                    variance += difference * difference;
                }
                variance /= costs.size();

                // Output dels costos per aquestes k i n
                std::cout << "avg: " << average << "\t"
                          << "std dev: " << sqrtf64(variance) << "\t";
            }
            if (supertest)
                std::cout << std::endl;
        }
    }
    if (not supertest)
        std::cout << std::endl;

    return EXIT_SUCCESS;
}
