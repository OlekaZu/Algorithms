#include "solve_tsp.h"

namespace s21 {

SolveTsp::SolveTsp() : tours_number_(0) { result_.distance = 0.0; }

void SolveTsp::ReadMatrixFromFile(const std::string& filename) {
  if (matrix_) matrix_.reset();
  parser_ = std::make_unique<MatrixParser>(filename);
  // only square matrix is needed!
  if (!parser_->IsMatrixSquare())
    throw std::out_of_range("File has the wrong format of the matrix.");
  matrix_ = std::make_shared<MyMatrix>(parser_->GetMatrix());
}

void SolveTsp::SetToursNumber(int number) {
  if (number <= 0) throw std::out_of_range("Wrong repeat number.");
  tours_number_ = number;
}

void SolveTsp::SetMatrix(int size, const std::vector<double>& weights) {
  if (matrix_) matrix_.reset();
  matrix_ = std::make_unique<MyMatrix>(size, size, weights.data());
}

double SolveTsp::SolveTradeSalesmanProblem() {
  auto start_test = std::chrono::high_resolution_clock::now();
  if (ants_colony_) ants_colony_.reset();
  ants_colony_ = std::make_unique<AntColony>(matrix_, tours_number_);
  ants_colony_->use_threads_ = use_threads_;
  ants_colony_->StartSearch();
  result_.distance = ants_colony_->GetBestDistance();
  result_.vertices = ants_colony_->GetBestPath();
  auto finish_test = std::chrono::high_resolution_clock::now();
  auto test_time_ = finish_test - start_test;
  return std::chrono::duration_cast<my_seconds>(test_time_).count();
}

SolveTsp::TsmResult SolveTsp::GetResult() const { return result_; }

}  // namespace s21
