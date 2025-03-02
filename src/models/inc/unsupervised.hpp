#ifndef UNSUPERVISED
#define UNSUPERVISED

#include <data.hpp>

std::vector<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> KMeans(int k, DataTable& input);

#endif /* UNSUPERVISED */
