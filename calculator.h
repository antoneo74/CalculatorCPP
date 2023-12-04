#ifndef CPP3_SMARTCALC_CALCULATOR_H
#define CPP3_SMARTCALC_CALCULATOR_H

#include <cmath>
#include "Parser/parser.h"
#include "Parser/rpn.h"

namespace s21 {

	class Calculator {
	public:
		std::string Execute(std::string& expression, double x);

	private:
		std::list<Node> rpn_;
		std::list<Node> stack_;
		std::list<Node> tokens_;

		double result_ = 0;
		Parser parser_;
		Notation notation_;

		double CalculateAction(double first_operand, double second_operand, Actions action);
		double GetResult();
		bool Calculate();
		bool CalculateItem(Node item);
		void Clear();
		std::pair<double, bool> GetValueFromStack();
	};
}  // namespace s21

#endif  // CPP3_SMARTCALC_CALCULATOR_H
