#ifndef CPP3_SMARTCALC_PARSER_PARSER_H
#define CPP3_SMARTCALC_PARSER_PARSER_H


#include <string>
#include <iostream>
#include <list>
#include "Model/actions.h"
#include "Model/priority.h"
#include "Model/node.h"

namespace s21 {
	class Parser
	{
	public:
		Parser(const std::string expresion, double x_value) : expresion_(expresion), x_value_(x_value) {}		

		std::pair<std::list<Node>, int> GetResult();


	private:
		const std::string expresion_;
		std::list<Node> tokens_;
		double x_value_;
		size_t index_ = 0;


		int Parse();
		int ParseAction(int action_is_active, int operand);
		void CheckAction(int action, int operand);
		int CreateNumber();
	};
}

#endif  // CPP3_SMARTCALC_PARSER_PARSER_H
