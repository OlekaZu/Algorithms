#include "ant_colony.h"

namespace s21 {

AntColony::AntColony(std::shared_ptr<MyMatrix> matrix, int tours_number) {
  number_ = matrix->get_cols();
  distances_ = *matrix.get();
  pheromones_ = MyMatrix(number_, number_);
  max_tour_numbers_ = tours_number;
  best_path_distance_ = INFINITY;
}

void AntColony::StartSearch() {
  Research();
  if (use_threads_) {
    StartSearchUsingThreads();
  } else {
    StartSearchWithoutThreads();
  }
}

std::vector<int> AntColony::GetBestPath() const { return best_path_; }

double AntColony::GetBestDistance() const { return best_path_distance_; }

void AntColony::StartSearchWithoutThreads() {
  int count_tours = 0;
  while (count_tours++ < max_tour_numbers_) DoOneTour();
}

void AntColony::StartSearchUsingThreads() {
  auto count_system_threads = std::thread::hardware_concurrency();
  for (int i = 0; i < max_tour_numbers_; ++i) {
    std::vector<std::thread> threads;
    std::size_t i_th = 0;
    for (; i_th < count_system_threads - 1 && i < max_tour_numbers_;
         ++i_th, ++i)
      threads.push_back(std::thread([this] { DoOneTour(); }));
    --i;
    for (auto& element : threads) element.join();
  }
}

void AntColony::DoOneTour() {
  auto thread_colony = CreateColony();
  for (int i = 1; i <= number_; ++i)
    thread_colony.push_back(std::make_unique<Ant>(i));
  int count_vertices = 0;
  while (count_vertices <= number_) {
    for (auto& each_ant : thread_colony) {
      int cur_vertex = each_ant->GetLocation();
      if (each_ant->CanMove()) {
        auto next_vertex_pair =
            each_ant->ChooseNextVertex(distances_, pheromones_);
        mtx_.lock();
        UpdatePheromones(cur_vertex, next_vertex_pair);
        mtx_.unlock();
      }
    }
    ++count_vertices;
  }
  FindBestPath(thread_colony);
  EvaporationPheromones();
}

void AntColony::Research() {
  for (int i = 0; i < pheromones_.get_rows(); ++i) {
    for (int k = 0; k < pheromones_.get_cols(); ++k) {
      if (distances_(i, k)) pheromones_(i, k) = 1.0 / number_;
    }
  }
}

std::vector<std::unique_ptr<AntColony::Ant>> AntColony::CreateColony() const {
  std::vector<std::unique_ptr<Ant>> colony;
  for (int i = 1; i <= number_; ++i) colony.push_back(std::make_unique<Ant>(i));
  return colony;
}

void AntColony::UpdatePheromones(
    int cur_vertex, const std::pair<int, double>& next_vertex_pair) {
  if (next_vertex_pair.first) {
    int from_index = cur_vertex - 1;
    int to_index = next_vertex_pair.first - 1;
    double cur_pheromone_value = pheromones_(from_index, to_index);
    pheromones_(from_index, to_index) =
        kPheromonesDying * next_vertex_pair.second +
        (1 - kPheromonesDying) * cur_pheromone_value;
  }
}

void AntColony::FindBestPath(
    const std::vector<std::unique_ptr<AntColony::Ant>>& colony) {
  for (auto& each_ant : colony) {
    auto temp = CalculateDistance(each_ant->GetTabu());
    mtx_.lock();
    if (temp < best_path_distance_) {
      best_path_distance_ = temp;
      best_path_ = each_ant->GetTabu();
    }
    mtx_.unlock();
  }
}

double AntColony::CalculateDistance(const std::vector<int>& path) {
  double result = 0.0;
  if (static_cast<int>(path.size()) == number_ + 1) {
    for (size_t i = 0; i < path.size() - 1; i++)
      result += distances_(path[i] - 1, path[i + 1] - 1);
  } else {
    result = INFINITY;
  }
  return result;
}

void AntColony::EvaporationPheromones() {
  pheromones_.MulNumber(kPheromonesEvaporation);
}

AntColony::Ant::Ant(int location) : start_location_(location), can_move_(true) {
  current_location_ = start_location_;
  tabu_.push_back(current_location_);
}

std::pair<int, double> AntColony::Ant::ChooseNextVertex(
    MyMatrix& distances, const MyMatrix& pheromones) {
  MyMatrix ant_pheromones = pheromones;
  int vertex_chosen_index = 0;
  // pair (number of possible vertex (begins with 1), its value of pheromones)
  std::vector<std::pair<int, double>> possible_vertices;
  // накопленные вероятности рандомного прохода
  std::vector<double> vertices_probability_markers;
  FormChoiceProbabilityVector(distances, ant_pheromones, possible_vertices,
                              vertices_probability_markers);
  if (possible_vertices.empty()) {
    can_move_ = false;
    possible_vertices.push_back(std::make_pair(0, 0.0));
  } else {
    // generate random value for comparing
    std::random_device ant_rd;
    std::uniform_real_distribution<double> uniform_dist(
        0.0, *std::max_element(vertices_probability_markers.begin(),
                               vertices_probability_markers.end()));
    double random_var = uniform_dist(ant_rd);
    // choice the first match
    auto vertex_chosen_iterator =
        find_if(vertices_probability_markers.begin(),
                vertices_probability_markers.end(),
                [random_var](const double& element) -> bool {
                  return random_var <= element;
                });
    vertex_chosen_index = std::distance(vertices_probability_markers.begin(),
                                        vertex_chosen_iterator);
    current_location_ = possible_vertices.at(vertex_chosen_index).first;
    tabu_.push_back(current_location_);
  }
  return possible_vertices.at(vertex_chosen_index);
}

void AntColony::Ant::FormChoiceProbabilityVector(
    MyMatrix& distances, MyMatrix& pheromones,
    std::vector<std::pair<int, double>>& vertices,
    std::vector<double>& probabilities) {
  double max_probability = 0.0;
  for (int i = 0; i < distances.get_rows(); ++i) {
    if (distances(current_location_ - 1, i) &&
        (none_of(tabu_.begin(), tabu_.end(),
                 [i](const int& element) { return element == (i + 1); }) ||
         (i == start_location_ - 1 &&
          static_cast<int>(tabu_.size()) == distances.get_rows()))) {
      double pheromone_value = CalculatePheromonesValue(pheromones, i);
      vertices.push_back(std::make_pair(i + 1, pheromone_value));
      double cur_probability =
          pow(pheromones(current_location_ - 1, i), AntColony::kAlpha) *
          pow(1.0 / distances(current_location_ - 1, i), AntColony::kBeta);
      max_probability += cur_probability;
      probabilities.push_back(max_probability);
    }
  }
}

int AntColony::Ant::GetLocation() const { return current_location_; }

std::vector<int> AntColony::Ant::GetTabu() const { return tabu_; }

void AntColony::Ant::ClearTabu() {
  tabu_.erase(begin(tabu_) + 1, end(tabu_));
  tabu_.shrink_to_fit();
}

bool AntColony::Ant::CanMove() const { return can_move_; }

double AntColony::Ant::CalculatePheromonesValue(MyMatrix& pheromones,
                                                int vertex) {
  double result = 0;
  for (int i = 0; i < pheromones.get_rows(); ++i) {
    if (pheromones(vertex, i) > result) result = pheromones(vertex, i);
  }
  return result;
}

}  // namespace s21
