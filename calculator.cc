#include "calculator.h"
using namespace s21;

Calculator::Calculator(std::string& expression, double x) : parser_(expression, x) {
	result_ = 0;
}

std::string Calculator::Execute() {
	int error = parser_.GetResult().second;
	if (!error) {
		tokens_ = parser_.GetResult().first;
		error = CreateRPN();
		if (!error) {
			error = Calculate();
		}
	}
	std::string result = error ? "Error" : std::to_string(GetResult());
	return result;
}


int Calculator::CreateRPN() {
	int error = 0;
	while (!tokens_.empty() && !error) {
		if (tokens_.begin()->action == Actions::kNumber) {
			rpn_.push_back(tokens_.front());
		}
		else if (tokens_.begin()->action == Actions::kSin || tokens_.begin()->action == Actions::kCos ||
			tokens_.begin()->action == Actions::kTan || tokens_.begin()->action == Actions::kAsin ||
			tokens_.begin()->action == Actions::kAcos || tokens_.begin()->action == Actions::kAtan ||
			tokens_.begin()->action == Actions::kLog || tokens_.begin()->action == Actions::kLog10 ||
			tokens_.begin()->action == Actions::kSqrt || tokens_.begin()->action == Actions::kOpeningBracket) {
			stack_.push_front(tokens_.front());
		}
		else if (tokens_.begin()->action == Actions::kClosingBracket) {
			while (!stack_.empty() && stack_.begin()->action != Actions::kOpeningBracket) {
				rpn_.push_back(stack_.front());
				stack_.pop_front();
			}
			if (stack_.empty())
				error = 1;
			else
				stack_.pop_front();
		}
		else {
			while (!stack_.empty() &&
				tokens_.begin()->priority <= stack_.begin()->priority) {
				rpn_.push_back(stack_.front());
				stack_.pop_front();
			}
			stack_.push_front(tokens_.front());
		}
		tokens_.pop_front();
	}
	if (!stack_.empty() && !error) {
		while (!stack_.empty()) {
			if (stack_.begin()->action == Actions::kOpeningBracket) {
				error = 1;
				break;
			}
			rpn_.push_back(stack_.front());
			stack_.pop_front();
		}
	}
	if (error) {
		tokens_.clear();
		rpn_.clear();
		stack_.clear();
	}
	return error;
}

double Calculator::CalculateAction(double first_operand, double second_operand,
	Actions action) {
	double result = 0;
	switch (action) {
	case Actions::kAdd:
		result = first_operand + second_operand;
		break;
	case Actions::kSub:
		result = first_operand - second_operand;
		break;
	case Actions::kMul:
		result = first_operand * second_operand;
		break;
	case Actions::kDiv:
		result = first_operand / second_operand;
		break;
	case Actions::kPow:
		result = pow(first_operand, second_operand);
		break;
	case Actions::kFmod:
		result = fmod(first_operand, second_operand);
		break;
	case Actions::kSin:
		result = sin(first_operand);
		break;
	case Actions::kCos:
		result = cos(first_operand);
		break;
	case Actions::kTan:
		result = tan(first_operand);
		break;
	case Actions::kAsin:
		result = asin(first_operand);
		break;
	case Actions::kAcos:
		result = acos(first_operand);
		break;
	case Actions::kAtan:
		result = atan(first_operand);
		break;
	case Actions::kLog:
		result = log(first_operand);
		break;
	case Actions::kLog10:
		result = log10(first_operand);
		break;
	case Actions::kSqrt:
		result = sqrt(first_operand);
		break;
	}
	return result;
}

int Calculator::Calculate() {
	int error = 0;
	if (!rpn_.empty()) {
		while (!rpn_.empty() && !error) {
			if (rpn_.begin()->action == Actions::kNumber) {
				stack_.push_front(rpn_.front());
			}
			else {
				if (stack_.empty()) {
					error = 1;
				}
				else {
					Actions action = rpn_.begin()->action;
					int binary = rpn_.begin()->binary;

					if ((action == Actions::kAdd || action == Actions::kSub) && !binary) {
						stack_.begin()->value = (action == Actions::kSub ? -stack_.begin()->value
							: stack_.begin()->value);
					}
					else {
						double first_operand = 0;
						double second_operand = 0;
						if (binary) {
							second_operand = stack_.begin()->value;
							stack_.pop_front();
							if (!stack_.empty()) {
								first_operand = stack_.begin()->value;
								stack_.pop_front();
							}
							else {
								error = 1;
							}
						}
						else {
							first_operand = stack_.begin()->value;
							stack_.pop_front();
						}
						double res = CalculateAction(first_operand, second_operand, action);
						stack_.push_front(Node{ Actions::kNumber, Priority::kLowest, false, res });
					}
				}
			}
			rpn_.pop_front();
		}
		if (!error) {
			result_ = stack_.begin()->value;
			stack_.pop_front();
		}
		if (error) rpn_.clear();
	}
	return error;
}

double Calculator::GetResult() { return result_; }
