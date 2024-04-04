#include "ant_interface.h"

int main() {
  setlocale(LC_ALL, "ru");
  s21::AntInterface::GetInstance();
  while (s21::AntInterface::GetInstance().GetNType() != 9) {
    try {
      s21::AntInterface::GetInstance().Menu();
      s21::AntInterface::GetInstance().Start();
    } catch (std::exception &error) {
      std::cout << error.what() << std::endl;
    }
  }
  return 0;
}
