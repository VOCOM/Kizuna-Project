#ifndef UNSUPERVISED
#define UNSUPERVISED

#include <kizuna/data.hpp>

class Clusters {
public:
	using DynamicMatrix = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;

public:
	auto& operator[](int index) { return clusters[index]; }

	Clusters(int k = 0, int rows = 0, int cols = 0) {
		for (int i = 0; i < k; i++) {
			clusters.push_back(DynamicMatrix());
			clusters[i].setZero(rows, cols);
		}
	}

private:
	std::vector<DynamicMatrix> clusters;
};

Clusters KMeans(int k, DataTable& input);

#endif /* UNSUPERVISED */
