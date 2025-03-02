#ifndef DATA
#define DATA

#include <string>

#include <Eigen/Core>

class DataTable {
public:
    int Rows() { return table.rows(); }
    int Cols() { return table.cols(); }
    int Size() { return table.size(); }
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>& Data() { return table; }

    auto GetRow(int i) { return table.row(i); }
    auto GetCol(int i) { return table.col(i); }

    void AddFeature(const std::string featureName, std::vector<double>& values);

    void Info();

    DataTable() {}

private:
    Eigen::RowVector<std::string, Eigen::Dynamic> header;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> table;

};

#endif /* DATA */
