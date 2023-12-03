#ifndef CPP3_SMARTCALC_CALCULATOR_H
#define CPP3_SMARTCALC_CALCULATOR_H

#include <cmath>
#include "parser.h"

namespace s21 {

	class Calculator {
	public:
		Calculator(std::string& expression, double x);
		std::string Execute();

	private:
		std::list<Node> rpn_;
		std::list<Node> stack_;
		std::list<Node> tokens_;

		double result_;
		Parser parser_;

		int CreateRPN();
		double GetResult();
		double CalculateAction(double first_operand, double second_operand, Actions action);
		int Calculate();
	};
}  // namespace s21

#endif  // CPP3_SMARTCALC_CALCULATOR_H
