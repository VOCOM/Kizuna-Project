#include <iostream>
#include <iomanip>

#include <core.hpp>
#include <timer.hpp>

#include <unsupervised.hpp>

int main(int argc, char** argv) {
	if (Core_Bond::Init() == false) {
		std::cout << "Failed to initialize OpenCL\n";
		return 1;
	}

	Timer timer;
	DataTable data;

	int c = 10000;
	std::vector<double> feature1, feature2, feature3, feature4;
	for (int i = 0; i < c; i++) {
		feature1.push_back(std::rand() % 99 + 0.1);
		feature2.push_back(std::rand() % 99 + 0.1);
		feature3.push_back(std::rand() % 99 + 0.1);
		feature4.push_back(std::rand() % 99 + 0.1);
	}

	data.AddFeature("Feature 1", feature1);
	data.AddFeature("Feature 2", feature2);
	data.AddFeature("Feature 3", feature3);
	data.AddFeature("Feature 4", feature4);
	// data.Info();
	
	std::cout << "\nElement Count: " << c << "\n";

	timer.Start();
	auto clusters = KMeans(4, data);
	timer.Stop();
		
	std::cout << "GPU KMeans: " << timer.DurationS() << "s\n\n";
}