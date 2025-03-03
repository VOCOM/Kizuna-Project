#include <data.hpp>

#include <iomanip>
#include <iostream>

using namespace Eigen;

int numDigits(int number) {
	int digits = 1;
	while (number > 0) {
		number /= 10;
		digits++;
	}
	return digits;
}

void DataTable::AddFeature(const std::string featureName, std::vector<double>& values) {
	header.conservativeResize(NoChange, header.cols() + 1);
	int cols = header.cols(), rows = values.size();
	header(cols - 1) = featureName;
	table.conservativeResize(rows, cols);
	for (int row = 0; row < rows; row++)
		table(row, cols - 1) = values[row];
}

void DataTable::Info() {
	int rows = table.rows(), cols = table.cols();
	std::vector<int> colWidths(cols);

	std::cout << "Features: " << cols << '\n';
	std::cout << "Entries: " << rows << '\n';

	for (int col = 0; col < cols; col++) {
		colWidths[col] = header(col).size();
		for (int row = 0; row < rows; row++) {
			int w = numDigits(table(row, col));
			if (w > colWidths[col])
				colWidths[col] = w;
		}
	}

	for (int i = 0; i < header.cols(); i++)
		std::cout << '\'' << header(i) << "\' ";

	std::cout << '\n';
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			double val = table(row, col);
			std::cout << std::setw(colWidths[col] + 1) << val << "  ";
		}
		std::cout << '\n';
	}
}

auto& DataTable::operator[](int index) {
	if (index > table.size())
		throw new std::exception("Index out of range!");
	return table(index);
}
