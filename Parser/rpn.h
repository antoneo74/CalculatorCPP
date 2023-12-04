#ifndef CPP3_SMARTCALC_PARSER_RPN_H
#define CPP3_SMARTCALC_PARSER_RPN_H

#include "parser.h"

namespace s21 {
	class Notation {
	public:
		std::pair<std::list<Node>, bool> CreateRPN(std::list<Node> tokens);

	private:
		std::list<Node> rpn_;
		std::list<Node> temporary_stack_;
		std::list<Node> tokens_;

		bool ClosingBracketItem();
		void DefaultItem();
		std::pair<std::list<Node>, bool> Clear();
	};
}


#endif  // CPP3_SMARTCALC_PARSER_RPN_H

