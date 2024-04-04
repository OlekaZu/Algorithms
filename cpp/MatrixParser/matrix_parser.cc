#include "matrix_parser.h"

namespace s21 {

MatrixParser::MatrixParser(const std::string& filename) {
  std::fstream s_file;
  s_file.open(filename, std::ios::in);
  if (!s_file.is_open()) throw std::out_of_range("File doesn't exist.");
  if (s_file.peek() == EOF) throw std::out_of_range("File is empty.");
  std::string line;
  int row_index = 0;
  while (std::getline(s_file, line)) {
    if (!matrix_) {
      if (!MatrixInitialize(line))
        throw std::out_of_range(
            "File has the wrong format of the matrix size.");
    } else {
      FillTheRow(ParseLine(line), row_index);
      ++row_index;
    }
  }
  s_file.close();
  if (row_index != size_row_)
    throw std::out_of_range("File has the wrong format of the matrix.");
}

MyMatrix MatrixParser::GetMatrix() const { return *matrix_; }

bool MatrixParser::IsMatrixSquare() const {
  return size_row_ == size_col_ && matrix_;
}

bool MatrixParser::MatrixInitialize(const std::string& line) {
  auto first_line = ParseLine(line);
  if (first_line.empty() || first_line.size() > 2) return false;
  size_row_ = static_cast<int>(first_line[0]);
  size_col_ = static_cast<int>(first_line[1]);
  if (size_row_ <= 0 || size_col_ <= 0) return false;
  matrix_ = std::make_unique<MyMatrix>(size_row_, size_col_);
  return true;
}

std::vector<double> MatrixParser::ParseLine(const std::string& line) {
  std::vector<double> row_values;
  std::istringstream ss_line;
  ss_line.str(line);
  for (std::string subline; std::getline(ss_line, subline, ' ');)
    row_values.push_back(std::stod(subline));
  return row_values;
}

void MatrixParser::FillTheRow(const std::vector<double>& row_values,
                              int row_index) {
  if (static_cast<int>(row_values.size()) != size_col_)
    throw std::out_of_range("File has the wrong format of the matrix.");
  for (int i = 0; i < size_col_; ++i)
    matrix_->operator()(row_index, i) = row_values[i];
}

}  // namespace s21
