#ifndef DATA
#define DATA

#include <string>

#include <Eigen/Core>

class DataTable {
public:
	using Header = Eigen::Matrix<std::string, 1, Eigen::Dynamic>;
	using Index  = Eigen::Matrix<double, Eigen::Dynamic, 1>;
	using Data   = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;

public: // Table Information
	int Rows() const { return data.rows(); }
	int Cols() const { return data.cols(); }
	int Size() const { return data.size(); }
	int Count() const { return data.rowwise().count().count(); }

	bool ContainsHeaders() const { return header.cols(); }
	bool ContainsLabels() const { return label.cols(); }

	Header GetHeader() const { return header; }
	Header GetLabel() const { return label; }
	Index GetIndex() const { return index; }
	Data GetData() const { return data; }

public: // Data Information
	void Info(int count = -1);

public: // Accessors
	auto Get(int row, int col) { return data(row, col); }
	auto GetRow(int index) { return data.row(index); }
	auto GetCol(int index) { return data.col(index); }

public: // Table Operations
	void InsertFeature(const std::string featureName);
	void InsertEntry(std::vector<double>& values);
	void LoadCSV(std::string filepath);
	void DropData() { data.setZero(0, 0); }

	DataTable() {}

private:
	Header header;
	Header label;
	Index index;
	Data data;
};

#endif /* DATA */
