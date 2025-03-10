#ifndef RESULTS
#define RESULTS

#include <string>

#include <Eigen/Core>

class Clusters {
public:
	using RowMatrix = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;

public:
	int Count() const { return clusters.size(); }

	auto& operator[](int index) { return clusters[index]; }

	Clusters(int k = 0, int rows = 0, int cols = 0) {
		for (int i = 0; i < k; i++) {
			clusters.push_back(RowMatrix());
			clusters[i].setZero(rows, cols);
		}
	}

private:
	std::vector<RowMatrix> clusters;
};

struct Results {
	enum Type {
		Unsupervised,
		Supervised,
		NeuralNet
	};

	// Unsupervised
	Clusters clusters;
	// Supervised
	// Neural Network

	Type type = Unsupervised;
	std::string Type() {
		switch (type) {
		case Supervised:
			return "Supervised";
		case NeuralNet:
			return "NeuralNet";
		default:
			return "Unsupervised";
		}
	}

	bool empty = true;
	bool Empty() { return empty; }
};

#endif /* RESULTS */
