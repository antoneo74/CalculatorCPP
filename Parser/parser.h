#ifndef CPP3_SMARTCALC_PARSER_PARSER_H
#define CPP3_SMARTCALC_PARSER_PARSER_H


#include <string>
#include <iostream>
#include <list>
#include "../Model/actions.h"
#include "../Model/priority.h"
#include "../Model/node.h"

namespace s21 {
	class Parser {
	public:
		std::pair<std::list<Node>, int> GetResult(const std::string expresion, double x_value);

	private:
		void Initialization(const std::string expresion, double x_value);
		std::string expresion_;
		std::list<Node> tokens_;
		double x_value_ = 0;
		size_t index_ = 0;

		bool Parse();
		bool ParseAction(int action_is_active, int operand);
		bool CreateNumber();
		void CheckAction(int action, int operand);
	};
}

#endif  // CPP3_SMARTCALC_PARSER_PARSER_H
