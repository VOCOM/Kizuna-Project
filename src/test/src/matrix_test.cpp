#include "matrix_test.hpp"

#include <timer.hpp>
#include <matrix.hpp>

#include <iostream>

namespace Matrix_Test {
  void Addition() {
    Timer timer;
    Matrix A(3, 3, 1.0), B(3, 3, 2.0);

    timer.Start();
    auto C = A + B;
    timer.Stop();

    std::cout << "Addition Test\n";
    std::cout << "-------------";
    std::cout << "\nA:\n"; A.Print();
    std::cout << "\nB:\n"; B.Print();
    std::cout << "\nC:\n"; C.Print();
    std::cout << "\nGPU: " << timer.DurationUS() << "us\n";
    std::cout << "-------------\n\n";
  }
  void Subtraction() {
    Timer timer;
    Matrix A(3, 3, 1.0), B(3, 3, 2.0);

    timer.Start();
    auto C = A - B;
    timer.Stop();

    std::cout << "Subtraction Test\n";
    std::cout << "----------------";
    std::cout << "\nA:\n"; A.Print();
    std::cout << "\nB:\n"; B.Print();
    std::cout << "\nC:\n"; C.Print();
    std::cout << "\nGPU: " << timer.DurationUS() << "us\n";
    std::cout << "----------------\n\n";
  }
  void Multiplication() {
    Timer timer;
    Matrix A(3, 5, 2.0), B(5, 2, 3.0);

    timer.Start();
    auto C = A * B;
    timer.Stop();

    std::cout << "Multiplication Test\n";
    std::cout << "-------------------";
    std::cout << "\nA:\n"; A.Print();
    std::cout << "\nB:\n"; B.Print();
    std::cout << "\nC:\n"; C.Print();
    std::cout << "\nGPU: " << timer.DurationUS() << "us\n";
    std::cout << "-------------------\n\n";
  }
  void Inverse() {
    Timer timer;
    Matrix<double> A(3, 3);
    for (int i = 0; i < A.Size(); i++) A[i] = std::rand() % 10;

    timer.Start();
    auto B = A.Inverse();
    timer.Stop();

    std::cout << "Inverse Test\n";
    std::cout << "------------";
    std::cout << "\nA:\n"; A.Print();
    std::cout << "\nB:\n"; B.Print(4);
    std::cout << "\nAB:\n"; (A * B).Print(4);
    std::cout << "\nGPU: " << timer.DurationUS() << "us\n";
    std::cout << "------------\n\n";
  }
  void Transpose() {
    Timer timer;
    Matrix<double> A(2, 3);
    for (int i = 0; i < A.Size(); i++) A[i] = std::rand() % 10;

    timer.Start();
    auto B = A.Transpose();
    timer.Stop();

    std::cout << "Transpose Test\n";
    std::cout << "--------------";
    std::cout << "\nA:\n"; A.Print();
    std::cout << "\nB:\n"; B.Print();
    std::cout << "\nGPU: " << timer.DurationUS() << "us\n";
    std::cout << "--------------\n\n";
  }
  void Cofactor() {
    Timer timer;
    Matrix<double> A(3, 3);
    for (int i = 0; i < A.Size(); i++) A[i] = std::rand() % 10;

    timer.Start();
    auto B = A.Cofactor();
    timer.Stop();

    std::cout << "Cofactor Test\n";
    std::cout << "-------------";
    std::cout << "\nA:\n"; A.Print();
    std::cout << "\nB:\n"; B.Print(5);
    std::cout << "\nGPU: " << timer.DurationUS() << "us\n";
    std::cout << "-------------\n\n";
  }
  void Submatrix() {
    Timer timer;
    Matrix<double> A(5, 5);
    for (int i = 0; i < A.Size(); i++) A[i] = std::rand() % 10;

    timer.Start();
    auto B = A.SubMatrix(1, 3, 1, 3);
    timer.Stop();

    std::cout << "Submatrix Test\n";
    std::cout << "--------------";
    std::cout << "\nA:\n"; A.Print();
    std::cout << "\nB:\n"; B.Print(3);
    std::cout << "\nGPU: " << timer.DurationUS() << "us\n";
    std::cout << "--------------\n\n";
  }
  void Determinant() {
    Timer timer;
    Matrix<double> A(3, 3);
    for (int i = 0; i < A.Size(); i++) A[i] = std::rand() % 10;

    timer.Start();
    auto Det = A.Determinant();
    timer.Stop();

    std::cout << "Determinant Test\n";
    std::cout << "----------------";
    std::cout << "\nA:\n"; A.Print();
    std::cout << "\nDeterminant: " << Det << '\n';
    std::cout << "\nGPU: " << timer.DurationUS() << "us\n";
    std::cout << "----------------\n\n";
  }
}
