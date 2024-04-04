#include "ant_interface.h"

namespace s21 {

void AntInterface::Menu() {
  std::cout << std::endl
            << "Please, load the adjacency matrix of a graph. / " << std::endl;
  std::cout << "Пожалуйста, введите матрицу смежности для графа." << std::endl
            << std::setw(67)
            << "|    * * *  * * *  * * *  * * *  * * *  * * *   |" << std::endl;
  std::cout << std::setw(57) << "| Press 1 - for loading from a file. / "
            << std::setw(10) << '|' << std::endl;
  std::cout << std::setw(79) << "| Нажмите 1 - для загрузки из файла."
            << std::setw(13) << '|' << std::endl;
  std::cout << std::setw(65)
            << "| Press 2 - for loading trought the console. / " << std::setw(2)
            << '|' << std::endl;
  std::cout << std::setw(98) << "| Нажмите 2 - для ввода матрицы через консоль."
            << std::setw(3) << '|' << std::endl;
  std::cout << std::setw(42) << "| Press 9 - for quit. / " << std::setw(25)
            << '|' << std::endl;
  std::cout << std::setw(83) << "| Нажмите 9 - для выхода из программы."
            << std::setw(11) << '|' << std::endl
            << std::setw(67)
            << "|    * * *  * * *  * * *  * * *  * * *  * * *   |" << std::endl;
}

void AntInterface::Start() {
  std::cin >> n_type_;
  while (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Something wrong - try again. / Что-то пошло не так - "
                 "попробуй снова."
              << std::endl;
    std::cin >> n_type_;
  }
  if (n_type_ == 1) {
    InputMatrixFile();
  } else if (n_type_ == 2) {
    InputMatrixInConsole();
  } else if (n_type_ == 9) {
    std::cout << "See you later!" << std::endl << "Пока!" << std::endl;
  } else {
    std::cout << "Something wrong - try again. / Что-то пошло не так - "
                 "попробуй снова."
              << std::endl;
  }
}

int AntInterface::GetNType() const { return n_type_; }

void AntInterface::InputMatrixFile() {
  std::cout << "Enter the path to the file / " << std::endl
            << "Введите путь до файла" << std::endl;
  std::string path;
  while (path.empty()) std::cin >> path;
  algorithm_.ReadMatrixFromFile(path);
  InputToursNumber();
  OutputResult();
}

void AntInterface::InputMatrixInConsole() {
  int matrix_size = 0, count = 0;
  double value = 0.0;
  std::vector<double> matrix_values;
  std::cout << "Enter the graph vertices number: / " << std::endl
            << "Введите колчества вершин графа:" << std::endl;
  std::cin >> matrix_size;
  std::cout << "Enter " << matrix_size * matrix_size << " elements values: / "
            << std::endl
            << "Введите " << matrix_size * matrix_size
            << " значений элементов матрицы:" << std::endl;
  while (count < matrix_size * matrix_size) {
    std::cin >> value;
    matrix_values.push_back(value);
    ++count;
  }
  algorithm_.SetMatrix(matrix_size, matrix_values);
  InputToursNumber();
  OutputResult();
}

void AntInterface::OutputResult() {
  algorithm_.use_threads_ = false;
  auto res_1 = algorithm_.SolveTradeSalesmanProblem();
  OutputAlgorithmResult();
  std::cout << std::setiosflags(std::ios::left) << std::setw(84)
            << "Total time (sec) / Итого время (сек): "
            << std::resetiosflags(std::ios::left) << res_1 << std::endl;
  std::cout << std::endl;
  algorithm_.use_threads_ = true;
  auto res_2 = algorithm_.SolveTradeSalesmanProblem();
  OutputAlgorithmResult();
  std::cout << std::setiosflags(std::ios::left) << std::setw(84)
            << "Total time in parallels (sec) / Итого время при "
               "многопоточности (сек): "
            << std::resetiosflags(std::ios::left) << res_2 << std::endl;
}

void AntInterface::OutputAlgorithmResult() {
  std::cout << std::setiosflags(std::ios::left) << std::setw(40)
            << "Distance / Расстояние: ";
  std::cout << algorithm_.GetResult().distance << std::endl;
  std::cout << std::setiosflags(std::ios::left) << std::setw(34)
            << "Path / Путь: ";
  for (auto ele : algorithm_.GetResult().vertices) std::cout << ele << " ";
  std::cout << std::endl;
}

void AntInterface::InputToursNumber() {
  int count = 0;
  std::cout << "Please, input number of algorithm tours. / " << std::endl;
  std::cout << "Пожалуйста, введите количество повторений алгоритма."
            << std::endl;
  std::cin >> count;
  algorithm_.SetToursNumber(count);
}

AntInterface::AntInterface() {
  std::cout << "--- ESTIMATING OF TWO IMPLEMENTATIONS OF THE ANT ALGORITHM FOR "
               "SOLVING TRAVELLING SALESMAN PROBLEM. ---"
            << std::endl;
  std::cout << "--- СРАВНЕНИЕ ДВУХ РЕАЛИЗАЦИИЙ МУРАВЬИНОГО АЛГОРИТМА ДЛЯ "
               "РЕШЕНИЯ ЗАДАЧИ О КОММИВОЯЖЁРЕ --- "
            << std::endl;
  std::cout << std::setw(50) << "* * *" << std::endl;
}

}  // namespace s21
