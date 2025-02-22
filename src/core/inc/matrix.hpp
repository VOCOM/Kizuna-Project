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

    // Access Operators
    T& operator[](int idx);

    // Arithmetic Operators
    Matrix<T> operator+(Matrix& rhs);
    Matrix<T> operator-(Matrix& rhs);
    Matrix<T> operator*(Matrix& rhs);

    Matrix<T> Transpose() const;

    // Iterators
    inline T* begin() { return &(_data[0]); }
    inline T* end() { return &(_data[Size() - 1]); }

    inline T* cbegin() const { return begin(); }

    void Print(int w = 3) const;

    // Constructors
    inline Matrix(int row = 1, int col = 1, T val = 0) :_row(row), _col(col), _data(row* col) {
        if (val != 0) std::fill(_data.begin(), _data.end(), val);
    }
    static Matrix<T> I(int sz);

    Matrix<T> operator=(const Matrix<T>& rhs);

private:
    int _row, _col;
    std::vector<T> _data;
};

template<typename T>
inline T& Matrix<T>::operator[](int idx) {
    if (idx >= _row * _col) throw new std::exception();
    return _data[idx];
}

template<typename T>
inline Matrix<T> Matrix<T>::operator+(Matrix& rhs) {
    if ((_row != rhs.Rows()) || (_col != rhs.Cols())) throw new std::exception();

    int size = _row * _col;
    auto queue = Core_Bond::Queue();
    auto buffer1 = Core_Bond::Buffer(0);
    auto buffer2 = Core_Bond::Buffer(1);
    auto buffer3 = Core_Bond::Buffer(2);

    queue.enqueueWriteBuffer(buffer1, CL_TRUE, 0, sizeof(int) * size, begin());
    queue.enqueueWriteBuffer(buffer2, CL_TRUE, 0, sizeof(int) * size, rhs.begin());

    auto kernel = Core_Bond::Add();
    kernel.setArg(0, buffer1);
    kernel.setArg(1, buffer2);
    kernel.setArg(2, buffer3);
    kernel.setArg(3, size);

    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(std::min(Core_Bond::MAX_THREADS(), size)));
    queue.finish();

    Matrix<T> output(_row, _col);
    queue.enqueueReadBuffer(buffer3, CL_TRUE, 0, sizeof(int) * size, output.begin());
    return output;
}

template<typename T>
inline Matrix<T> Matrix<T>::operator-(Matrix& rhs) {
    if ((_row != rhs.Rows()) || (_col != rhs.Cols())) throw new std::exception();

    int size = _row * _col;
    auto queue = Core_Bond::Queue();
    auto buffer1 = Core_Bond::Buffer(0);
    auto buffer2 = Core_Bond::Buffer(1);
    auto buffer3 = Core_Bond::Buffer(2);

    queue.enqueueWriteBuffer(buffer1, CL_TRUE, 0, sizeof(int) * size, begin());
    queue.enqueueWriteBuffer(buffer2, CL_TRUE, 0, sizeof(int) * size, rhs.begin());

    auto kernel = Core_Bond::Sub();
    kernel.setArg(0, buffer1);
    kernel.setArg(1, buffer2);
    kernel.setArg(2, buffer3);
    kernel.setArg(3, size);

    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(std::min(Core_Bond::MAX_THREADS(), size)));
    queue.finish();

    Matrix<T> output(_row, _col);
    queue.enqueueReadBuffer(buffer3, CL_TRUE, 0, sizeof(int) * size, output.begin());
    return output;
}

template<typename T>
inline Matrix<T> Matrix<T>::operator*(Matrix& rhs) {
    if (_col != rhs.Rows()) throw new std::exception();

    int size = _row * rhs.Cols();
    auto queue = Core_Bond::Queue();
    auto buffer1 = Core_Bond::Buffer(0);
    auto buffer2 = Core_Bond::Buffer(1);
    auto buffer3 = Core_Bond::Buffer(2);

    queue.enqueueWriteBuffer(buffer1, CL_TRUE, 0, sizeof(int) * Size(), begin());
    queue.enqueueWriteBuffer(buffer2, CL_TRUE, 0, sizeof(int) * rhs.Size(), rhs.begin());

    // [MxN] = [MxK] * [KxN]
    // M, N, K, M1, M2, M3
    auto kernel = Core_Bond::Mul();
    kernel.setArg(0, _row);
    kernel.setArg(1, rhs.Cols());
    kernel.setArg(2, _col);
    kernel.setArg(3, buffer1);
    kernel.setArg(4, buffer2);
    kernel.setArg(5, buffer3);

    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(_row, _col));
    queue.finish();

    Matrix<T> output(_row, rhs.Cols());
    queue.enqueueReadBuffer(buffer3, CL_TRUE, 0, sizeof(int) * size, output.begin());
    return output;
}

template<typename T>
inline Matrix<T> Matrix<T>::Transpose() const {
    int size = Size();
    auto queue = Core_Bond::Queue();
    auto buffer1 = Core_Bond::Buffer(0);
    auto buffer2 = Core_Bond::Buffer(1);

    queue.enqueueWriteBuffer(buffer1, CL_TRUE, 0, sizeof(int) * size, &(_data[0]));

    auto kernel = Core_Bond::Transpose();
    kernel.setArg(0, buffer1);
    kernel.setArg(1, buffer2);

    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(_row, _col));
    queue.finish();

    Matrix<T> output(_col, _row);
    queue.enqueueReadBuffer(buffer2, CL_TRUE, 0, sizeof(int) * size, output.begin());

    return output;
}

template<typename T>
inline void Matrix<T>::Print(int w) const {
    std::cout << "Shape " << _row << "x" << _col << '\n';
    for (int i = 0; i < _row; i++) {
        std::cout << "[ ";
        for (int j = 0; j < _col; j++)
            std::cout << std::setw(w) << _data[i * _col + j] << " ";
        std::cout << "]\n";
    }
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
inline Matrix<T> Matrix<T>::operator=(const Matrix<T>& rhs) {
    _data = std::move(rhs._data);
    return *this;
}

template<typename T>
inline Matrix<T> operator*(T lhs, Matrix<T>& rhs) {
    Matrix<T> scalar(rhs.Rows(), rhs.Cols());
    std::fill(scalar.begin(), scalar.end() + 1, lhs);
    return scalar * rhs;
}

#endif /* MATRIX */
