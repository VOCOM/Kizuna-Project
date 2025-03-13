#include <data.hpp>

#include <fstream>
#include <iomanip>
#include <iostream>

#include <utility/utils.hpp>

using namespace Eigen;

void DataTable::InsertFeature(const std::string featureName) {
	int newCols = Cols() + 1;
	header.conservativeResize(NoChange, newCols);
	data.conservativeResize(NoChange, newCols);
	header(newCols - 1) = featureName;
}
void DataTable::InsertEntry(std::vector<double>& values) {
	int count   = values.size();
	int newRows = Rows() + 1;
	index.conservativeResize(newRows + 1, NoChange);
	data.conservativeResize(newRows, NoChange);

	auto row = GetRow(newRows - 1);
	for (int col = 0; col < Cols(); col++) {
		if (col > count) continue;
		row(col) = values[col];
	}
	index(newRows) = newRows;
}

void DataTable::LoadCSV(std::string filepath, bool hasHeaders) {
	std::ifstream fs(filepath);
	if (!fs.is_open()) {
		std::cout << "Failed to read file: " << filepath << "\n";
		return;
	}

	std::string line;
	while (!fs.eof()) {
		fs >> line;
		auto stringValues = Split(line, ',');
		int count         = stringValues.size();
		if (count == 0) continue;

		if (hasHeaders) {
			for (int i = 0; i < count; i++)
				InsertFeature(stringValues[i]);
			hasHeaders = false;
		} else {
			std::vector<double> values(count);
			for (int i = 0; i < count; i++)
				values[i] = std::stod(stringValues[i]);
			InsertEntry(values);
		}
	}
}

void DataTable::Info(int count) {
	int rows = Rows(), cols = Cols();
	std::vector<int> colWidths(cols + 1);

	// Column Width
	for (int row = 0; row < rows; row++) {
		if (row == 0) {
			for (int col = 0; col < cols; col++)
				colWidths[col + 1] = header(col).size();
		} else {
			for (int col = 0, w = 0; col < cols + 1; col++) {
				w = Digits(col == 0 ? index(row) : data(row, col - 1));
				if (w > colWidths[col]) colWidths[col] = w;
			}
		}
	}

	// Header
	std::cout << std::setw(colWidths[0]) << "";
	for (int col = 0; col < cols; col++) {
		std::cout << " ";
		std::cout << std::setw(colWidths[col + 1]) << header(col);
		std::cout << "";
	}
	std::cout << '\n';

	// Entries
	if (count < 0) count = rows;
	bool filler = count < rows;
	for (int row = 0; row < rows; row++) {
		if (row >= count && row != rows - 1) continue;
		if ((row == rows - 1) && filler) {
			int width = 0;
			for (auto w : colWidths) width += w + 1;
			std::cout << std::setw(width) << std::setfill('.') << "\n";
		}
		for (int col = 0; col < cols + 1; col++) {
			if (col != 0) std::cout << " ";
			std::cout << std::setfill(' ')
								<< std::setw(colWidths[col])
								<< (col == 0 ? index(row + 1) : data(row, col - 1));
		}
		std::cout << '\n';
	}

	// Entry filler
	// if (rows > count) {
	// 	int width = 0;
	// 	for (auto w : colWidths) width += w + 1;
	// 	std::cout << std::setw(width) << std::setfill('.') << "\n";

	// 	for (int col = 0; col < cols + 1; col++) {
	// 		if (col != 0) std::cout << " ";
	// 		std::cout << std::setfill(' ')
	// 							<< std::setw(colWidths[col])
	// 							<< (col == 0 ? index(rows) : data(rows - 1, col - 1));
	// 	}
	// }
	// std::cout << "\n";
}
