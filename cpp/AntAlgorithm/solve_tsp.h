#ifndef SRC_ANTALGORITHM_SOLVE_TSP_H_
#define SRC_ANTALGORITHM_SOLVE_TSP_H_

#include <chrono>

#include "ant_colony.h"
#include "matrix_parser.h"

namespace s21 {

class SolveTsp {
 public:
  struct TsmResult {
    std::vector<int> vertices;
    double distance;
  };
  using my_seconds = std::chrono::duration<float>;

  explicit SolveTsp();
  SolveTsp(const SolveTsp& other) = delete;
  SolveTsp& operator=(const SolveTsp& other) = delete;
  ~SolveTsp() = default;

  void ReadMatrixFromFile(const std::string& filename);
  void SetToursNumber(int number);
  void SetMatrix(int size, const std::vector<double>& weights);
  // возвращает время выполнения алгоритма в секундах
  double SolveTradeSalesmanProblem();
  TsmResult GetResult() const;

  bool use_threads_ = false;

 private:
  std::unique_ptr<MatrixParser> parser_;
  std::shared_ptr<MyMatrix> matrix_;
  int tours_number_;
  std::unique_ptr<AntColony> ants_colony_;
  TsmResult result_;
};

}  // namespace s21

#endif  // SRC_ANTALGORITHM_SOLVE_TSP_H_
