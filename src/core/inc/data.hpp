#ifndef DATA
#define DATA

#include <string>

#include <Eigen/Core>

class DataTable {
public:
	using DynamicMatrix = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;

	// Data Information
	inline int Rows() { return table.rows(); }
	inline int Cols() { return table.cols(); }
	inline int Size() { return table.size(); }
	void Info();

	// Accessors
	auto& operator[](int index);
	inline auto GetRow(int index) { return table.row(index); }
	inline auto GetCol(int index) { return table.col(index); }
	inline DynamicMatrix& Data() { return table; }

	void AddFeature(const std::string featureName, std::vector<double>& values);

	DataTable() {}

private:
	Eigen::RowVector<std::string, Eigen::Dynamic> header;
	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> table;
};

#endif /* DATA */
