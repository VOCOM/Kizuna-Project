#ifndef MATRIX
#define MATRIX

#include <vector>
#include <algorithm>
#include <iomanip>

#include <core.hpp>

template<typename T>
class Matrix {
public:
    inline int Rows() const { return _row; }
    inline int Cols() const { return _col; }
    inline int Size() const { return _data.size(); }
    inline bool IsSquare() const { return _row == _col; }
    inline bool IsOrthoganal() const { return static_cast<int>(Determinant()) == 1; }

    // Matrix Properties
    inline double Determinant() const;

    Matrix<T> operator=(const Matrix<T>& rhs);

    // Access Operators
    T& operator[](int idx);

    // Arithmetic Operators
    Matrix<T> operator+(Matrix& rhs);
    Matrix<T> operator-(Matrix& rhs);
    Matrix<T> operator*(Matrix& rhs);

    // Matrix Operations
    Matrix<T> Inverse() const;
    Matrix<T> Transpose() const;

    // Iterators
    inline T* begin() { return &(_data[0]); }
    inline T* end() { return &(_data[Size() - 1]); }
    inline T* cbegin() const { return begin(); }

    void Print(int w = 3, int p = 1) const;

    // Constructors
    inline Matrix(int row = 1, int col = 1, T val = 0) :_row(row), _col(col), _data(row* col) {
        if (val != 0) std::fill(_data.begin(), _data.end(), val);
    }
    static Matrix<T> I(int sz);

private:
    Matrix<T> CoFactor() const;

    int _row, _col;
    std::vector<T> _data;
};

template<typename T>
inline T& Matrix<T>::operator[](int idx) {
    if (idx >= _row * _col) throw new std::exception("Index out of range!");
    return _data[idx];
}

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

template<typename T>
inline Matrix<T> Matrix<T>::Inverse() const {
    if (IsSquare() == false) throw new std::exception("Cannot inverse non-square matrix!");
    if (Determinant() == 0) throw new std::exception("Matrix is non-invertible!");

    // Use simple formula
    if (_row == 2) {
        Matrix<T> inv(2, 2);
        inv[0] = _data[3];
        inv[1] = -_data[1];
        inv[2] = -_data[2];
        inv[3] = _data[0];
        return (1 / Determinant()) * inv;
    }

    Matrix<T> inv;

    return inv;
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
inline Matrix<T> Matrix<T>::I(int sz) {
    Matrix<T> Identity(sz, sz);

    for (int i = 0;i < sz;i++)
        for (int j = 0;j < sz;j++)
            if (i == j) Identity[i * sz + j] = 1;

    return Identity;
}

template<typename T>
inline double Matrix<T>::Determinant() const {
    if (IsSquare() == false) throw new std::exception("Non-square matrix has no determinant!");

    if (_row == 2) return _data[0] * _data[3] - _data[1] * _data[2];
    if (_row == 3) {
        auto cofactor = CoFactor();

    }

    return T();
}

template<typename T>
inline Matrix<T> Matrix<T>::CoFactor() const {
    if (_row != _col) throw new std::exception("No cofactor for non-square matrix!");

    std::vector<T> tmp;
    // [x,x,x]
    // [x,a,b]
    // [x,c,d]
    tmp.push_back(_data[4]);
    tmp.push_back(_data[5]);
    tmp.push_back(_data[7]);
    tmp.push_back(_data[8]);

    // [x,x,x]
    // [a,x,b]
    // [c,x,d]
    tmp.push_back(_data[3]);
    tmp.push_back(_data[5]);
    tmp.push_back(_data[6]);
    tmp.push_back(_data[8]);

    // [x,x,x]
    // [a,b,x]
    // [c,d,x]
    tmp.push_back(_data[3]);
    tmp.push_back(_data[4]);
    tmp.push_back(_data[6]);
    tmp.push_back(_data[7]);

    tmp.push_back(_data[4]);
    tmp.push_back(_data[5]);
    tmp.push_back(_data[7]);
    tmp.push_back(_data[8]);

    tmp.push_back(_data[4]);
    tmp.push_back(_data[5]);
    tmp.push_back(_data[7]);
    tmp.push_back(_data[8]);

    tmp.push_back(_data[4]);
    tmp.push_back(_data[5]);
    tmp.push_back(_data[7]);
    tmp.push_back(_data[8]);

    tmp.push_back(_data[4]);
    tmp.push_back(_data[5]);
    tmp.push_back(_data[7]);
    tmp.push_back(_data[8]);

    tmp.push_back(_data[4]);
    tmp.push_back(_data[5]);
    tmp.push_back(_data[7]);
    tmp.push_back(_data[8]);

    tmp.push_back(_data[4]);
    tmp.push_back(_data[5]);
    tmp.push_back(_data[7]);
    tmp.push_back(_data[8]);

    Matrix<T> cofactor(3, 3);
    return cofactor;
}
template<typename T>
inline Matrix<T> Matrix<T>::operator=(const Matrix<T>& rhs) {
    _data = std::move(rhs._data);
    return *this;
}

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
inline Matrix<T>& operator*(T lhs, Matrix<T>& rhs) {
    int size = rhs.Size();
    auto queue = Core_Bond::Queue();
    auto buffer1 = Core_Bond::Buffer(0);

    queue.enqueueWriteBuffer(buffer1, CL_TRUE, 0, sizeof(T) * rhs.Size(), rhs.begin());

    auto kernel = Core_Bond::Mul();
    kernel.setArg(0, buffer1);
    kernel.setArg(1, lhs);

    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(rhs.Size()));
    queue.finish();

    queue.enqueueReadBuffer(buffer1, CL_TRUE, 0, sizeof(T) * size, rhs.begin());
    return rhs;
}

#endif /* MATRIX */
