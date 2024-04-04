#ifndef SRC_ANTALGORITHM_ANT_INTERFACE_H_
#define SRC_ANTALGORITHM_ANT_INTERFACE_H_

#include <iomanip>

#include "solve_tsp.h"

namespace s21 {

class AntInterface {
 public:
  AntInterface(const AntInterface&) = delete;
  AntInterface& operator=(AntInterface&) = delete;
  static AntInterface& GetInstance() {
    static AntInterface p_instance;
    return p_instance;
  }

  void Menu();
  void Start();
  int GetNType() const;

 private:
  AntInterface();
  ~AntInterface(){};

  void InputMatrixFile();
  void InputMatrixInConsole();
  void OutputResult();
  void OutputAlgorithmResult();
  void InputToursNumber();

  SolveTsp algorithm_;
  int n_type_ = 0;
};

}  // namespace s21

#endif  // SRC_ANTALGORITHM_ANT_INTERFACE_H_
