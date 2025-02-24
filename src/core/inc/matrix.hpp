#ifndef MATRIX
#define MATRIX

#include <vector>
#include <algorithm>
#include <iomanip>

#include <core.hpp>

// Class Template
template<typename T>
class Matrix {
public:
    // Matrix Properties
    inline int Rows() const { return _row; }
    inline int Cols() const { return _col; }
    inline int Size() const { return _data.size(); }
    inline bool IsSquare() const { return _row == _col; }
    inline bool IsInvertible() const { return static_cast<int>(Determinant()) != 0; }
    inline bool IsOrthoganal() const { return std::abs(static_cast<int>(Determinant())) == 1; }

    // Matrix Properties
    double Determinant() const;

    // Move Operator
    Matrix<T> operator=(const Matrix<T>& rhs);

    // Access Operators
    T& operator[](int idx);

    // Scalar Arithmetic Operators
    Matrix<T> operator+(T& rhs) { return *this + Matrix<T>(_row, _col, rhs); }

    // Matrix Arithmetic Operators
    Matrix<T> operator+(Matrix& rhs);
    Matrix<T> operator-(Matrix& rhs);
    Matrix<T> operator*(Matrix& rhs);

    // Matrix Operations
    Matrix<T> Inverse() const;
    Matrix<T> Transpose() const;
    Matrix<T> Cofactor() const;

    // Iterators
    inline T* begin() { return &(_data[0]); }
    inline T* end() { return &(_data[Size() - 1]); }
    inline T* cbegin() const { return begin(); }

    // Display
    void Print(int w = 3, int p = 1) const;

    Matrix<T> SubMatrix(int rowStart, int rowEnd, int colStart, int colEnd) const;

    // Constructors
    inline Matrix(int row = 1, int col = 1, T val = 0) : _row(row), _col(col), _data(row* col) {
        if (val != 0) std::fill(_data.begin(), _data.end(), val);
    }
    static Matrix<T> I(int sz);

private:
    int _row, _col;
    std::vector<T> _data;
};

// Matrix Properties
template<typename T>
double Matrix<T>::Determinant() const {
    if (IsSquare() == false) throw new std::exception("Non-square matrix has no determinant!");
    if (_row > 3) throw new std::exception("No determinant implementation for matrix of rank 4 and above!");

    if (_row == 2) return _data[0] * _data[3] - _data[1] * _data[2];

    Matrix<T> cofactor = Cofactor();
    double det = 0;
    for (int i = 0; i < _col; i++)
        det += _data[i] * cofactor[i];
    return det;
}

// Move Operator
template<typename T>
inline Matrix<T> Matrix<T>::operator=(const Matrix<T>& rhs) {
    _row = rhs.Rows();
    _col = rhs.Cols();
    _data.resize(rhs.Size());
    _data = std::move(rhs._data);
    return *this;
}

// Access Operators
template<typename T>
inline T& Matrix<T>::operator[](int idx) {
    if (idx >= Size()) throw new std::exception("Index out of range!");
    return _data[idx];
}

// Matrix Arithmetic Operators
template<typename T>
inline Matrix<T> Matrix<T>::operator+(Matrix& rhs) {
    if ((_row != rhs.Rows()) || (_col != rhs.Cols())) throw new std::exception("Matrix shape mismatch!");

    int size = _row * _col;
    auto queue = Core_Bond::Queue();
    auto buffer1 = Core_Bond::Buffer(0);
    auto buffer2 = Core_Bond::Buffer(1);
    auto buffer3 = Core_Bond::Buffer(2);

    queue.enqueueWriteBuffer(buffer1, CL_TRUE, 0, sizeof(T) * size, begin());
    queue.enqueueWriteBuffer(buffer2, CL_TRUE, 0, sizeof(T) * size, rhs.begin());

    auto kernel = Core_Bond::Add();
    kernel.setArg(0, buffer1);
    kernel.setArg(1, buffer2);
    kernel.setArg(2, buffer3);
    kernel.setArg(3, size);

    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(size));
    queue.finish();

    Matrix<T> output(_row, _col);
    queue.enqueueReadBuffer(buffer3, CL_TRUE, 0, sizeof(T) * size, output.begin());
    return output;
}
template<typename T>
inline Matrix<T> Matrix<T>::operator-(Matrix& rhs) {
    if ((_row != rhs.Rows()) || (_col != rhs.Cols())) throw new std::exception("Matrix shape mismatch!");

    int size = _row * _col;
    auto queue = Core_Bond::Queue();
    auto buffer1 = Core_Bond::Buffer(0);
    auto buffer2 = Core_Bond::Buffer(1);
    auto buffer3 = Core_Bond::Buffer(2);

    queue.enqueueWriteBuffer(buffer1, CL_TRUE, 0, sizeof(T) * size, begin());
    queue.enqueueWriteBuffer(buffer2, CL_TRUE, 0, sizeof(T) * size, rhs.begin());

    auto kernel = Core_Bond::Sub();
    kernel.setArg(0, buffer1);
    kernel.setArg(1, buffer2);
    kernel.setArg(2, buffer3);
    kernel.setArg(3, size);

    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(size));
    queue.finish();

    Matrix<T> output(_row, _col);
    queue.enqueueReadBuffer(buffer3, CL_TRUE, 0, sizeof(T) * size, output.begin());
    return output;
}
template<typename T>
inline Matrix<T> Matrix<T>::operator*(Matrix& rhs) {
    if (_col != rhs.Rows()) throw new std::exception("Common dimension mismatch!");

    int size = _row * rhs.Cols();
    auto queue = Core_Bond::Queue();
    auto buffer1 = Core_Bond::Buffer(0);
    auto buffer2 = Core_Bond::Buffer(1);
    auto buffer3 = Core_Bond::Buffer(2);

    queue.enqueueWriteBuffer(buffer1, CL_TRUE, 0, sizeof(T) * Size(), begin());
    queue.enqueueWriteBuffer(buffer2, CL_TRUE, 0, sizeof(T) * rhs.Size(), rhs.begin());

    // [MxN] = [MxK] * [KxN]
    // M1, M2, M3, K
    auto kernel = Core_Bond::GeMM();
    kernel.setArg(0, buffer1);
    kernel.setArg(1, buffer2);
    kernel.setArg(2, buffer3);
    kernel.setArg(3, _col);

    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(_row, rhs.Cols()));
    queue.finish();

    Matrix<T> output(_row, rhs.Cols());
    queue.enqueueReadBuffer(buffer3, CL_TRUE, 0, sizeof(T) * size, output.begin());
    return output;
}

// Matrix Operations
template<typename T>
inline Matrix<T> Matrix<T>::Inverse() const {
    if (_row == 1) return _data[0];
    if (IsSquare() == false) throw new std::exception("Cannot inverse non-square matrix!");
    if (_row > 3) throw new std::exception("No inverse implementation for matrix of dimension 4 and above!");

    double determinant = Determinant();
    if (IsInvertible() == false) throw new std::exception("Matrix is non-invertible!");

    // Use simple formula
    if (_row == 2) {
        Matrix<T> inv(2, 2);
        inv[0] = _data[3];
        inv[1] = -_data[1];
        inv[2] = -_data[2];
        inv[3] = _data[0];
        return (1 / determinant) * inv;
    }

    auto adjoint = Cofactor().Transpose();
    return (1 / determinant) * adjoint;
}
template<typename T>
inline Matrix<T> Matrix<T>::Transpose() const {
    int size = Size();
    auto queue = Core_Bond::Queue();
    auto buffer1 = Core_Bond::Buffer(0);
    auto buffer2 = Core_Bond::Buffer(1);

    queue.enqueueWriteBuffer(buffer1, CL_TRUE, 0, sizeof(T) * size, &(_data[0]));

    auto kernel = Core_Bond::Transpose();
    kernel.setArg(0, buffer1);
    kernel.setArg(1, buffer2);

    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(_row, _col));
    queue.finish();

    Matrix<T> output(_col, _row);
    queue.enqueueReadBuffer(buffer2, CL_TRUE, 0, sizeof(T) * size, output.begin());

    return output;
}
template<typename T>
inline Matrix<T> Matrix<T>::Cofactor() const {
    if (IsSquare() == false) throw new std::exception("No cofactor for non-square matrix!");

    int size = Size();
    auto queue = Core_Bond::Queue();
    auto buffer1 = Core_Bond::Buffer(0);
    auto buffer2 = Core_Bond::Buffer(1);

    std::vector<T> buf(size * 4);
    for (int i = 0; i < _row; i++)
        for (int j = 0; j < _col; j++) {
            int idx = (i * _col + j) * 4;
            buf[idx + 0] = _data[(i == 0 ? 3 : 0) + (j == 0 ? 1 : 0)];
            buf[idx + 1] = _data[(i == 0 ? 3 : 0) + (j != 2 ? 2 : 1)];
            buf[idx + 2] = _data[(i != 2 ? 3 : 0) + (j == 0 ? 4 : 3)];
            buf[idx + 3] = _data[(i != 2 ? 3 : 0) + (j != 2 ? 5 : 4)];
        }

    queue.enqueueWriteBuffer(buffer1, CL_TRUE, 0, sizeof(T) * buf.size(), &(buf[0]));

    auto kernel = Core_Bond::Cofactor();
    kernel.setArg(0, buffer1);
    kernel.setArg(1, buffer2);

    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(size));
    queue.finish();

    Matrix<T> cofactor(_row, _col);
    queue.enqueueReadBuffer(buffer2, CL_TRUE, 0, sizeof(T) * size, cofactor.begin());
    return cofactor;
}

// Display
template<typename T>
inline void Matrix<T>::Print(int w, int p) const {
    std::cout << std::fixed << std::setprecision(p);
    for (int i = 0; i < _row; i++) {
        std::cout << "[ ";
        for (int j = 0; j < _col; j++)
            std::cout << std::setw(w) << _data[i * _col + j] << " ";
        std::cout << "]\n";
    }
    std::cout << "Shape " << _row << "x" << _col << '\n';
}

template<typename T>
inline Matrix<T> Matrix<T>::SubMatrix(int rowStart, int rowEnd, int colStart, int colEnd) const {
    int newRows = rowEnd - rowStart + 1;
    int newCols = colEnd - colStart + 1;
    if ((newRows > _row) || (newCols > _col)) throw new std::exception("Sub matrix dimensions out of range!");

    Matrix<T> sub(newRows, newCols);
    for (int i = 0; i < newRows; i++)
        for (int j = 0; j < newCols; j++)
            sub[i * newCols + j] = _data[(rowStart + i) * _col + colStart + j];
    return sub;
}

// Constructors
template<typename T>
inline Matrix<T> Matrix<T>::I(int sz) {
    Matrix<T> Identity(sz, sz);

    for (int i = 0;i < sz;i++)
        for (int j = 0;j < sz;j++)
            if (i == j) Identity[i * sz + j] = 1.0;

    return Identity;
}

// Integer Operators
template<typename T>
inline Matrix<T>& operator*(double lhs, Matrix<T>& rhs) {
    int size = rhs.Size();
    auto queue = Core_Bond::Queue();
    auto buffer1 = Core_Bond::Buffer(0);

    queue.enqueueWriteBuffer(buffer1, CL_TRUE, 0, sizeof(T) * size, rhs.begin());

    auto kernel = Core_Bond::Mul();
    kernel.setArg(0, buffer1);
    kernel.setArg(1, lhs);

    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(size));
    queue.finish();

    queue.enqueueReadBuffer(buffer1, CL_TRUE, 0, sizeof(T) * size, rhs.begin());
    return rhs;
}

#endif /* MATRIX */