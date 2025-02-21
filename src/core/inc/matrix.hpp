#ifndef MATRIX
#define MATRIX

#include <vector>
#include <algorithm>
#include <iomanip>

#include <core.hpp>

template<typename T>
class Matrix {
public:
    inline int  Row() const { return _row; }
    inline int Col()const { return _col; }
    inline int Size()const { return _data.size(); }

    T& operator[](int idx);

    Matrix<T> operator+(Matrix& rhs);
    Matrix<T> operator-(Matrix& rhs);
    Matrix<T> operator*(Matrix& rhs);

    inline T* begin() { return &(_data[0]); }
    inline T* end() { return &(_data[Size() - 1]); }

    void Print() const;

    inline Matrix() {}
    inline Matrix(int row, int col) :_row(row), _col(col), _data(row* col) {}

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
    if ((_row != rhs.Row()) || (_col != rhs.Col())) throw new std::exception();

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
    if ((_row != rhs.Row()) || (_col != rhs.Col())) throw new std::exception();

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
    if (_col != rhs.Row()) throw new std::exception();

    int size = _row * rhs.Col();
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
    kernel.setArg(1, rhs.Col());
    kernel.setArg(2, _col);
    kernel.setArg(3, buffer1);
    kernel.setArg(4, buffer2);
    kernel.setArg(5, buffer3);

    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(std::min(Core_Bond::MAX_THREADS(), size)));
    queue.finish();

    Matrix<T> output(_row, rhs.Col());
    queue.enqueueReadBuffer(buffer3, CL_TRUE, 0, sizeof(int) * size, output.begin());
    return output;
}

template<typename T>
inline void Matrix<T>::Print() const {
    for (int i = 0; i < _row;i++) {
        std::cout << "[ ";
        for (int j = 0;j < _col;j++)
            std::cout << std::setw(5) << _data[i * _row + j] << " ";
        std::cout << "]\n";
    }
}


#endif /* MATRIX */
