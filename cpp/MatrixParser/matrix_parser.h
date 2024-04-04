#ifndef SRC_MATRIXPARSER_MATRIXPARSER_H_
#define SRC_MATRIXPARSER_MATRIXPARSER_H_

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "my_matrix.h"

namespace s21 {

class MatrixParser {
 public:
  explicit MatrixParser(const std::string& filename);
  MatrixParser(const MatrixParser& other) = delete;
  MatrixParser(MatrixParser&& other) = delete;
  MatrixParser& operator=(const MatrixParser& other) = delete;
  MatrixParser operator=(MatrixParser&& other) = delete;
  ~MatrixParser() {}

  MyMatrix GetMatrix() const;
  bool IsMatrixSquare() const;

 private:
  bool MatrixInitialize(const std::string& line);
  std::vector<double> ParseLine(const std::string& line);
  void FillTheRow(const std::vector<double>& row_values, int row_index);

  std::unique_ptr<MyMatrix> matrix_;
  int size_row_ = 0;
  int size_col_ = 0;
};

}  // namespace s21

#endif  // SRC_MATRIXPARSER_MATRIXPARSER_H_
