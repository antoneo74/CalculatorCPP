#ifndef CPP3_SMARTCALC_CALCULATOR_H
#define CPP3_SMARTCALC_CALCULATOR_H

#include <cmath>
#include <iostream>
#include <list>
#include <string>
#include "actions.h"
#include "priority.h"
#include "node.h"

namespace s21 {

class Calculator {
 public:
  Calculator(std::string& expression, double x);
  std::string Execute();

 private:
  std::list<Node> tokens_;
  std::list<Node> rpn_;
  std::list<Node> stack_;

  std::string str_;
  double result_;
  double x_value_;
  size_t index_;

  int Parse();
  int CreateNumber();
  int ParseAction(int action, int operand);
  int CreateRPN();
  double GetResult();
  double CalculateAction(double first_operand, double second_operand, Actions action);
  int Calculate();
  void CheckAction(int action, int operand);
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_CALCULATOR_H
