#include <supervised.hpp>

#include <iostream>

void KNN::Info(int count) {
	std::cout << "KNN " << k << " clusters\n";
	data.Info(count);
}

void KNN::Train(int maxThreads) {
	lock = true;

	lock = false;
}