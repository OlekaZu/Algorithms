#ifndef SRC_LIBS_MY_MATRIX_H_
#define SRC_LIBS_MY_MATRIX_H_

#include <cmath>
#include <iomanip>
#include <iostream>

namespace s21 {

class MyMatrix {
 public:
  MyMatrix();
  MyMatrix(int rows, int cols);
  MyMatrix(int rows, int cols, const double data_massive[]);
  MyMatrix(const MyMatrix& other);
  MyMatrix& operator=(const MyMatrix& other);
  MyMatrix(MyMatrix&& other);
  MyMatrix& operator=(MyMatrix&& other);
  ~MyMatrix();

  // General methods
  bool EqualMatrix(const MyMatrix& other);
  void SumMatrix(const MyMatrix& other);
  void SubMatrix(const MyMatrix& other);
  void MulNumber(double num);
  void MulMatrix(const MyMatrix& other);
  MyMatrix Transpose();
  double Determinant();
  MyMatrix CalcComplements();
  MyMatrix InverseMatrix();
  void OutputMatrix();
  void Clear();

  // Operators reloading
  MyMatrix operator+(const MyMatrix& other);
  MyMatrix operator-(const MyMatrix& other);
  MyMatrix operator*(const MyMatrix& other);
  friend MyMatrix operator*(double num, const MyMatrix& matrix);
  friend MyMatrix operator*(const MyMatrix& matrix, const double num);
  bool operator==(const MyMatrix& other);
  MyMatrix& operator+=(const MyMatrix& other);
  MyMatrix& operator-=(const MyMatrix& other);
  MyMatrix& operator*=(const MyMatrix& other);
  MyMatrix& operator*=(double num);
  double& operator()(int i, int j);

  // Accessors and Mutators
  int get_rows() const;
  int get_cols() const;
  double get_value(int i, int j) const;
  void set_rows(const int value_rows);
  void set_cols(const int value_cols);

 protected:
  void MemoryAllocation();
  void MemoryFree();
  MyMatrix CreateMinorMatrix(int size, int rows_index, int cols_index);

 private:
  int rows_, cols_;
  double** matrix_;
};

}  // namespace s21

#endif  // SRC_LIBS_MY_MATRIX_H_
