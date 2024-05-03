#include "ntuple.h"
using index_t = std::vector<int>::size_type;

NTuple::NTuple()
{
    data = std::vector<double>(0);
}

NTuple::NTuple(int dim, std::mt19937 &gen)
{
    data = std::vector<double>(dim);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (int i = 0; i < dim; ++i)
    {
        data[i] = dist(gen);
    }
}

const double &NTuple::operator[](int index) const
{
    return data.at(index);
}

double NTuple::distance(const NTuple &other) const
{
    double sum = 0.0;
    for (int i = 0; i < data.size(); ++i)
    {
        sum += (data[i] - other[i]) * (data[i] - other[i]);
    }
    return std::sqrt(sum);
}