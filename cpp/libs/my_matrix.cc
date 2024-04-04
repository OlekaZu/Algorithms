#include "my_matrix.h"

namespace s21 {

/* ---------- Constructors & Destructor ---------- */
MyMatrix::MyMatrix() : rows_(0), cols_(0), matrix_(nullptr) {}

MyMatrix::MyMatrix(int rows, int cols) : rows_(rows), cols_(cols) {
  if (rows_ <= 0 || cols_ <= 0)
    throw std::out_of_range("Rows or Columns cannot be less 0 or 0.");
  MemoryAllocation();
}

MyMatrix::MyMatrix(int rows, int cols, const double data_massive[])
    : rows_(rows), cols_(cols) {
  if (rows_ <= 0 || cols_ <= 0)
    throw std::out_of_range("Rows or Columns cannot be less 0 or 0.");
  if (data_massive == nullptr)
    throw std::out_of_range("Empty massive for fillment.");
  MemoryAllocation();
  for (int i = 0; i < rows_; i++) {
    for (int k = 0; k < cols_; k++) matrix_[i][k] = data_massive[i * cols_ + k];
  }
}

MyMatrix::MyMatrix(const MyMatrix& other) : MyMatrix() { *this = other; }

MyMatrix& MyMatrix::operator=(const MyMatrix& other) {
  if (this == &other)
    throw std::out_of_range("Coping the same matrix is forbidden!");
  MemoryFree();
  rows_ = other.rows_;
  cols_ = other.cols_;
  MemoryAllocation();
  for (int i = 0; i < rows_; i++) {
    for (int k = 0; k < cols_; k++) matrix_[i][k] = other.matrix_[i][k];
  }
  return *this;
}

MyMatrix::MyMatrix(MyMatrix&& other) : MyMatrix() { *this = std::move(other); }

MyMatrix& MyMatrix::operator=(MyMatrix&& other) {
  if (this == &other)
    throw std::out_of_range("Moving the same matrix is forbidden!");
  std::swap(rows_, other.rows_);
  std::swap(cols_, other.cols_);
  std::swap(matrix_, other.matrix_);
  other.MemoryFree();
  return *this;
}

MyMatrix::~MyMatrix() { MemoryFree(); }

/* ---------- General Methods ---------- */
bool MyMatrix::EqualMatrix(const MyMatrix& other) {
  bool Result = true;
  if (rows_ == other.rows_ && cols_ == other.cols_) {
    double tolerance = 1e-7;
    for (int i = 0; i < rows_; i++) {
      for (int k = 0; k < cols_; k++) {
        if (fabs(matrix_[i][k] - other.matrix_[i][k]) >= tolerance)
          Result = false;
      }
    }
  } else {
    Result = false;
  }
  return Result;
}

void MyMatrix::SumMatrix(const MyMatrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw std::out_of_range("Matrixes with different size cannot be summed.");
  for (int i = 0; i < rows_; i++) {
    for (int k = 0; k < cols_; k++) matrix_[i][k] += other.matrix_[i][k];
  }
}

void MyMatrix::SubMatrix(const MyMatrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw std::out_of_range(
        "Matrixes with different size cannot be subtracted.");
  for (int i = 0; i < rows_; i++) {
    for (int k = 0; k < cols_; k++) matrix_[i][k] -= other.matrix_[i][k];
  }
}

void MyMatrix::MulNumber(double num) {
  for (int i = 0; i < rows_; i++) {
    for (int k = 0; k < cols_; k++) matrix_[i][k] *= num;
  }
}

void MyMatrix::MulMatrix(const MyMatrix& other) {
  if (cols_ != other.rows_)
    throw std::out_of_range(
        "Matrixes cannot be multed: matrix_1 cols != matrix_2 rows.");
  MyMatrix Result(rows_, other.cols_);
  for (int i = 0; i < rows_; i++) {
    for (int k = 0; k < other.cols_; k++) {
      double sum_matrix_elements = 0.0;
      for (int j = 0; j < cols_; j++)
        sum_matrix_elements += matrix_[i][j] * other.matrix_[j][k];
      Result.matrix_[i][k] = sum_matrix_elements;
    }
  }
  *this = std::move(Result);
}

MyMatrix MyMatrix::Transpose() {
  MyMatrix Result(cols_, rows_);
  for (int i = 0; i < Result.rows_; i++) {
    for (int k = 0; k < Result.cols_; k++) Result.matrix_[i][k] = matrix_[k][i];
  }
  return Result;
}

double MyMatrix::Determinant() {
  if (rows_ != cols_) {
    throw std::out_of_range(
        "Determinant cannot be found: matrix isn't square.");
  } else if (rows_ == 0) {
    throw std::out_of_range("Determinant cannot be found: matrix is empty.");
  }
  double Result;
  if (rows_ == 1) {
    Result = matrix_[0][0];
  } else {
    double sum_of_koef = 0.0;
    for (int k = 0; k < cols_; k++) {
      int Tmp_matrix_size = cols_ - 1;
      MyMatrix Tmp_matrix = CreateMinorMatrix(Tmp_matrix_size, 0, k);
      sum_of_koef += matrix_[0][k] * pow(-1, k) * Tmp_matrix.Determinant();
    }
    Result = sum_of_koef;
  }
  return Result;
}

MyMatrix MyMatrix::CreateMinorMatrix(int size, int rows_index, int cols_index) {
  MyMatrix Result(size, size);
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (j >= cols_index && i >= rows_index) {
        Result.matrix_[i][j] = matrix_[i + 1][j + 1];
      } else if (j >= cols_index && i < rows_index) {
        Result.matrix_[i][j] = matrix_[i][j + 1];
      } else if (j < cols_index && i >= rows_index) {
        Result.matrix_[i][j] = matrix_[i + 1][j];
      } else {
        Result.matrix_[i][j] = matrix_[i][j];
      }
    }
  }
  return Result;
}

MyMatrix MyMatrix::CalcComplements() {
  if (rows_ != cols_)
    throw std::out_of_range(
        "Сofactor matrix cannot be found: matrix isn't square.");
  MyMatrix Result(rows_, cols_);
  if (rows_ == 1) {
    Result.matrix_[0][0] = 1;
  } else {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        int Tmp_matrix_size = cols_ - 1;
        MyMatrix Tmp_matrix = CreateMinorMatrix(Tmp_matrix_size, i, j);
        Result.matrix_[i][j] = pow(-1, (i + j)) * Tmp_matrix.Determinant();
      }
    }
  }
  return Result;
}

MyMatrix MyMatrix::InverseMatrix() {
  double det = this->Determinant();
  if (det == 0)
    throw std::out_of_range("Inverse matrix cannot be found: det is nought.");
  MyMatrix Tmp_cofactor_matrix = CalcComplements();
  MyMatrix Tmp_transpose_matrix = Tmp_cofactor_matrix.Transpose();
  Tmp_transpose_matrix.MulNumber(1 / det);
  MyMatrix Result(rows_, cols_);
  for (int i = 0; i < rows_; i++) {
    for (int k = 0; k < cols_; k++)
      Result.matrix_[i][k] = Tmp_transpose_matrix.matrix_[i][k];
  }
  return Result;
}

void MyMatrix::OutputMatrix() {
  if (!matrix_) throw std::out_of_range("Matrix is empty.");
  std::cout << "Matrix output:" << std::endl;
  for (int i = 0; i < rows_; i++) {
    for (int k = 0; k < cols_; k++)
      std::cout << std::fixed << std::setprecision(6) << matrix_[i][k] << " ";
    if (i < (rows_ - 1)) std::cout << std::endl;
  }
  std::cout << std::endl;
}

void MyMatrix::Clear() { MemoryFree(); }

/* ---------- Operators overloading ---------- */
MyMatrix MyMatrix::operator+(const MyMatrix& other) {
  MyMatrix Result = *this;
  Result.SumMatrix(other);
  return Result;
}

MyMatrix MyMatrix::operator-(const MyMatrix& other) {
  MyMatrix Result = *this;
  Result.SubMatrix(other);
  return Result;
}

MyMatrix MyMatrix::operator*(const MyMatrix& other) {
  MyMatrix Result = *this;
  Result.MulMatrix(other);
  return Result;
}

MyMatrix operator*(const double num, const MyMatrix& matrix) {
  MyMatrix Result(matrix.rows_, matrix.cols_);
  for (int i = 0; i < matrix.rows_; i++) {
    for (int k = 0; k < matrix.cols_; k++)
      Result.matrix_[i][k] = num * matrix.matrix_[i][k];
  }
  return Result;
}

MyMatrix operator*(const MyMatrix& matrix, const double num) {
  MyMatrix Result(matrix.rows_, matrix.cols_);
  for (int i = 0; i < matrix.rows_; i++) {
    for (int k = 0; k < matrix.cols_; k++)
      Result.matrix_[i][k] = num * matrix.matrix_[i][k];
  }
  return Result;
}

bool MyMatrix::operator==(const MyMatrix& other) { return EqualMatrix(other); }

MyMatrix& MyMatrix::operator+=(const MyMatrix& other) {
  SumMatrix(other);
  return *this;
}

MyMatrix& MyMatrix::operator-=(const MyMatrix& other) {
  SubMatrix(other);
  return *this;
}

MyMatrix& MyMatrix::operator*=(const MyMatrix& other) {
  MulMatrix(other);
  return *this;
}

MyMatrix& MyMatrix::operator*=(double num) {
  MulNumber(num);
  return *this;
}

double& MyMatrix::operator()(int i, int j) {
  if (i < 0 || j < 0)
    throw std::out_of_range("Wrong value of index! Indexes cannot be less 0.");
  if (i > (rows_ - 1) || j > (cols_ - 1))
    throw std::out_of_range(
        "Wrong value of index! Value exceeds the size of the matrix.");
  return this->matrix_[i][j];
}

/* ---------- Accessor and Mutator Methods ---------- */
int MyMatrix::get_rows() const { return rows_; }

int MyMatrix::get_cols() const { return cols_; }

double MyMatrix::get_value(int i, int j) const {
  if (i < 0 || j < 0)
    throw std::out_of_range("Wrong value of index! Indexes cannot be less 0.");
  if (i > (rows_ - 1) || j > (cols_ - 1))
    throw std::out_of_range(
        "Wrong value of index! Value exceeds the size of the matrix.");
  if (!matrix_) throw std::out_of_range("Matrix is empty.");
  return this->matrix_[i][j];
}

// if new row value is more than old row value, new matrix value is 0
void MyMatrix::set_rows(int value_rows) {
  if (value_rows <= 0)
    throw std::out_of_range("Rows or Columns cannot be less 0 or 0.");
  if (rows_ != value_rows) {
    MyMatrix old_matrix = (*this);
    int save_cols = cols_;
    MemoryFree();
    rows_ = value_rows;
    cols_ = save_cols;
    MemoryAllocation();
    for (int i = 0; i < rows_; i++) {
      for (int k = 0; k < cols_; k++) {
        if (i >= old_matrix.rows_ || k >= old_matrix.cols_) {
          matrix_[i][k] = 0;
        } else {
          matrix_[i][k] = old_matrix.matrix_[i][k];
        }
      }
    }
  }
}

// if new cols value is more than old cols value, new matrix value is 0
void MyMatrix::set_cols(int value_cols) {
  if (value_cols <= 0)
    throw std::out_of_range("Rows or Columns cannot be less 0 or 0.");
  if (cols_ != value_cols) {
    MyMatrix old_matrix = (*this);
    int save_rows = rows_;
    MemoryFree();
    rows_ = save_rows;
    cols_ = value_cols;
    MemoryAllocation();
    for (int i = 0; i < rows_; i++) {
      for (int k = 0; k < cols_; k++) {
        if (i >= old_matrix.rows_ || k >= old_matrix.cols_) {
          matrix_[i][k] = 0;
        } else {
          matrix_[i][k] = old_matrix.matrix_[i][k];
        }
      }
    }
  }
}

/* ---------- Protected Methods ---------- */
void MyMatrix::MemoryAllocation() {
  matrix_ = new double*[rows_];
  for (int i = 0; i < rows_; i++) matrix_[i] = new double[cols_]{0};
}

void MyMatrix::MemoryFree() {
  if (matrix_) {
    for (int i = 0; i < rows_; i++) {
      if (matrix_[i]) {
        delete[] matrix_[i];
        matrix_[i] = nullptr;
      }
    }
    delete[] matrix_;
    matrix_ = nullptr;
  }
  rows_ = 0;
  cols_ = 0;
}

}  // namespace s21
