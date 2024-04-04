#ifndef SRC_ANTALGORITHM_ANT_COLONY_H_
#define SRC_ANTALGORITHM_ANT_COLONY_H_

#include <algorithm>
#include <cmath>
#include <memory>
#include <mutex>
#include <random>
#include <thread>

#include "my_matrix.h"

namespace s21 {

// number of ants for one tour = number of graph vertices
class AntColony {
 public:
  AntColony() = delete;
  explicit AntColony(std::shared_ptr<MyMatrix> matrix, int tours_number);
  AntColony(const AntColony& other) = delete;
  AntColony& operator=(const AntColony& other) = delete;
  ~AntColony() = default;

  void StartSearch();
  std::vector<int> GetBestPath() const;
  double GetBestDistance() const;

  bool use_threads_ = false;

 private:
  // pheromones part of probability of random choice of the next vertex
  constexpr static const double kAlpha = 1.0;
  // distances part of probability of random choice of the next vertex
  constexpr static const double kBeta = 1.0;
  constexpr static const double kPheromonesDying = 0.8;
  constexpr static const double kPheromonesEvaporation = 0.2;
  class Ant;

  void StartSearchWithoutThreads();
  void StartSearchUsingThreads();
  void DoOneTour();
  void Research();
  std::vector<std::unique_ptr<AntColony::Ant>> CreateColony() const;
  void UpdatePheromones(int cur_vertex,
                        const std::pair<int, double>& next_vertex_pair);
  void FindBestPath(const std::vector<std::unique_ptr<AntColony::Ant>>& colony);
  double CalculateDistance(const std::vector<int>& path);
  void EvaporationPheromones();

  std::random_device rd_;
  int number_;
  MyMatrix distances_;
  MyMatrix pheromones_;
  int max_tour_numbers_;
  std::vector<int> best_path_;
  double best_path_distance_;
  std::mutex mtx_;
};

class AntColony::Ant {
 public:
  Ant() = delete;
  explicit Ant(int location);
  Ant(const Ant&) = default;
  Ant& operator=(const Ant&) = default;
  Ant(Ant&&) = default;
  Ant& operator=(Ant&&) = default;
  ~Ant() = default;

  std::pair<int, double> ChooseNextVertex(MyMatrix& distances,
                                          const MyMatrix& pheromones);
  int GetLocation() const;
  std::vector<int> GetTabu() const;
  void ClearTabu();
  bool CanMove() const;

 private:
  void FormChoiceProbabilityVector(
      MyMatrix& distances, MyMatrix& pheromones,
      std::vector<std::pair<int, double>>& vertices,
      std::vector<double>& probabilities);
  double CalculatePheromonesValue(MyMatrix& pheromones, int vertex);

  // location number begins with 1 !!!
  int start_location_, current_location_;
  std::vector<int> tabu_;
  // if the ant is at a dead-end
  bool can_move_;
};

}  // namespace s21

#endif  // SRC_ANTALGORITHM_ANT_COLONY_H_
