#ifndef DATA
#define DATA

#include <string>

#include <Eigen/Core>

class DataTable {
public:
	using DynamicRow    = Eigen::RowVector<std::string, Eigen::Dynamic>;
	using DynamicMatrix = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;

	// Data Information
	int Rows() { return table.rows(); }
	int Cols() { return table.cols(); }
	int Size() { return table.size(); }
	int Count() { return table.rowwise().count().count(); }
	DynamicRow& Header() { return header; }
	void Info();
	void ShortInfo(int count = 5);

	// Accessors
	auto& operator[](int index);
	auto GetRow(int index) { return table.row(index); }
	auto GetCol(int index) { return table.col(index); }
	DynamicMatrix& Data() { return table; }

	void AddFeature(const std::string featureName);
	void AddElements(std::vector<double>& values);

	DataTable() {}

private:
	DynamicRow header;
	DynamicMatrix table;
};

#endif /* DATA */
