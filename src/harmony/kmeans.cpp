#include <unsupervised.hpp>

// C++ Headers
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

// Modules
#include <harmony.hpp>

#if DEBUG
#include <utility/timer.hpp>
#endif

using namespace Eigen;

std::vector<bool> locks;

void LoadCluster(const int start, const int end, const int N, const int dim, cl::Buffer index, cl::Buffer points, std::shared_ptr<Clusters> clusters) {
	auto queue = Harmony::Queue();
	for (int i = start, c = 0; i < end; i++) {
		// Find cluster
		queue.enqueueReadBuffer(index, CL_TRUE, sizeof(int) * i, sizeof(int), &c);

		// Load point
		RowVector<double, Dynamic> point(dim);
		for (int n = 0; n < dim; n++)
			queue.enqueueReadBuffer(points, CL_TRUE, sizeof(double) * (n * N + i), sizeof(double), point.data() + n);

		// Append to cluster
		while (locks[c]) std::this_thread::yield();
		locks[c]          = true;
		auto& cluster     = (*clusters)[c];
		int rows          = cluster.rowwise().count().count();
		cluster.row(rows) = point;
		locks[c]          = false;
	}
}

void KMeans::Info(int count) {
	std::cout << "Kmeans " << k << " clusters\n";
	data.Info(count);
}

void KMeans::Train(int maxThreads) {
	// Check from within Harmony
	// if (input.Size() * k > Harmony::BufferInfo().second)
	// 	throw new std::exception("Insufficient Buffer Space!");
	lock = true;

	int ret;
	const int maxEntries      = data.Rows();
	const int dimensions      = data.Cols();
	const DataTable::Data pts = data.GetData().transpose();
	std::vector<int> clusterIndexes(maxEntries);

	Matrix<double, Dynamic, Dynamic> centroids(k, dimensions);
	Matrix<double, Dynamic, Dynamic> newCentroids(k, dimensions);

	// Initialize Clusters & Centroids
	for (int i = 0; i < k; i++)
		centroids.row(i) = data.GetRow(i);

	// Get Master Queue
	auto queue   = Harmony::Queue();
	auto buffer1 = Harmony::Buffer(0); // Points        [double]
	auto buffer2 = Harmony::Buffer(1); // Centroids     [double]
	auto buffer3 = Harmony::Buffer(2); // Cluster Index [int]

	// Fill Buffer 1 with points [double]
	ret = queue.enqueueWriteBuffer(buffer1, CL_TRUE, 0, sizeof(double) * pts.count(), pts.data());
	if (ret != CL_SUCCESS) std::cout << "Error Filling Buffer 1. Code " << ret << "\n";

#if DEBUG
	int n                 = 0;
	long long computeTime = 0;
	long long bufferTime  = 0;
	Timer timer;
	timer.Start();
#endif

	// Compute clusters
	bool converged = false;
	while (!converged) {
		auto buffer4 = Harmony::Buffer(3); // New Centroids [double]

		// Fill Buffer 2 with centroids [double]
		ret = queue.enqueueWriteBuffer(buffer2, CL_TRUE, 0, sizeof(double) * centroids.count(), centroids.data());
		if (ret != CL_SUCCESS) std::cout << "Error Filling Buffer 2. Code " << ret << "\n";

		// Select Euclidean Distance Kernel
		// (Points, Centroids, Index, Dimensions, K)
		auto kernel = Harmony::Distance();
		kernel.setArg(0, buffer1);
		kernel.setArg(1, buffer2);
		kernel.setArg(2, buffer3);
		kernel.setArg(3, dimensions);
		kernel.setArg(4, k);

		// Execute Kernel
		ret = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(maxEntries));
		if (ret != CL_SUCCESS) std::cout << "Error executing Distance. Code " << ret << "\n";
		queue.finish();

		// Select Centroid Kernel
		// (Points, Dimensions, Index, New Centroids)
		kernel = Harmony::Centroid();
		kernel.setArg(0, buffer1);
		kernel.setArg(1, buffer3);
		kernel.setArg(2, buffer4);
		kernel.setArg(3, dimensions);
		kernel.setArg(4, maxEntries);

		// Execute Kernel
		ret = queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(k));
		if (ret != CL_SUCCESS) std::cout << "Error executing Centroid. Code " << ret << "\n";
		queue.finish();

		// Read New Centroids
		for (int c = 0; c < k; c++) {
			auto bytes = sizeof(double) * dimensions;
			RowVector<double, Dynamic> r(dimensions);
			queue.enqueueReadBuffer(buffer4, CL_TRUE, bytes * c, bytes, r.data());
			newCentroids.row(c) = r;
		}

		// Determine Convergence
		converged = centroids.isApprox(newCentroids);
		centroids = newCentroids;

#ifdef DEBUG
		n++;
#endif
	}

#ifdef DEBUG
	timer.Stop();
	computeTime = timer.DurationMS();
	timer.Start();
#endif

	// Load Clusters [Multi-CPU]
	clusters = Clusters(k, maxEntries, dimensions);
	locks.resize(k);

	int step = std::ceil(maxEntries / static_cast<double>(maxThreads));
	std::vector<std::thread> threadpool(maxThreads);

	for (int i = 0; i < maxThreads; i++) {
		int start     = i * step;
		int end       = std::min(start + step, maxEntries);
		threadpool[i] = std::thread(LoadCluster, start, end, maxEntries, dimensions, buffer3, buffer1, std::make_shared<Clusters>(clusters));
	}
	for (auto& thread : threadpool) thread.join();

#ifdef DEBUG
	timer.Stop();
	bufferTime = timer.DurationMS();
	std::cout << "KMeans\n";
	std::cout << "Iterations " << n << "\n";
	std::cout << "Compute Time " << computeTime << "ms\n";
	std::cout << "Buffering Time " << bufferTime << "ms\n";
#endif

	lock = false;
}
void KMeans::Test(int maxThreads) {
}

Results KMeans::Result() {
	Results r;
	r.clusters = clusters;
	return r;
}
