#include "calculator.h"

s21::Calculator::Calculator(std::string& expression, double x)
	: str_(expression), x_value_(x) {
	result_ = 0;
	index_ = 0;
}

int s21::Calculator::Start() {
	int error = Parse();
	if (!error) {
		error = CreateRPN();
		if (!error) {
			error = Calculate();
		}
	}
	return error;
}

int s21::Calculator::Parse() {
	int error = 0;
	int action = 1;
	int operand = 0;

	for (; index_ < str_.size() && !error; ) {
		if (str_[index_] == 'X') {
			CheckAction(action, operand);
			digital_.push_back(Node{ Actions::kNumber, Priority::kLowest, false, x_value_ });
			action = 0;
			operand = 1;
			++index_;
		}
		else if (str_[index_] == '.' || std::isdigit(str_[index_])) {
			CheckAction(action, operand);
			error = CreateNumber();
			if (error) break;
			operand = 1;
			action = 0;
		}
		else if (str_[index_] == ')') {
			ParseAction(action, operand);
			operand = 1;
			action = 0;
		}
		else if (str_[index_] == ' ') {
			++index_;
		}
		else {
			ParseAction(action, operand);
			action = 1;
			operand = 0;
		}
	}
	if (error) digital_.clear();
	return error;
}

void s21::Calculator::CheckAction(int action, int operand) {
	if (operand && !action) {
		digital_.push_back(Node{ Actions::kMul, Priority::kMedium, true, 0 });
	}
}

int s21::Calculator::CreateNumber() {
	try {
		size_t end = index_;
		std::string str = str_.substr(index_);
		double number = std::stod(str, &end);
		digital_.push_back(Node{ Actions::kNumber, Priority::kLowest, false, number });
		index_ = index_ + end;
		return 0;
	}
	catch (const std::exception&) {
		return 1;
	}
}

void s21::Calculator::ParseAction(int action_is_active, int operand) {
	if (str_.substr(index_, 3) == "sin") {
		CheckAction(action_is_active, operand);
		digital_.push_back(Node{ Actions::kSin, Priority::kHigh, false, 0 });
		index_ += 3;
	}
	else if (str_.substr(index_, 3) == "cos") {
		CheckAction(action_is_active, operand);
		digital_.push_back(Node{ Actions::kCos, Priority::kHigh, false, 0 });
		index_ += 3;
	}
	else if (str_.substr(index_, 3) == "tan") {
		CheckAction(action_is_active, operand);
		digital_.push_back(Node{ Actions::kTan, Priority::kHigh, false, 0 });
		index_ += 3;
	}
	else if (str_.substr(index_, 3) == "mod") {
		digital_.push_back(Node{ Actions::kFmod, Priority::kMedium, true, 0 });
		index_ += 3;
	}
	else if (str_.substr(index_, 2) == "ln") {
		CheckAction(action_is_active, operand);
		digital_.push_back(Node{ Actions::kLog, Priority::kHigh, false, 0 });
		index_ += 2;
	}
	else if (str_.substr(index_, 3) == "log") {
		CheckAction(action_is_active, operand);
		digital_.push_back(Node{ Actions::kLog10, Priority::kHigh, false, 0 });
		index_ += 3;
	}
	else if (str_.substr(index_, 4) == "asin") {
		CheckAction(action_is_active, operand);
		digital_.push_back(Node{ Actions::kAsin, Priority::kHigh, false, 0 });
		index_ += 4;
	}
	else if (str_.substr(index_, 4) == "acos") {
		CheckAction(action_is_active, operand);
		digital_.push_back(Node{ Actions::kAcos, Priority::kHigh, false, 0 });
		index_ += 4;
	}
	else if (str_.substr(index_, 4) == "atan") {
		CheckAction(action_is_active, operand);
		digital_.push_back(Node{ Actions::kAtan, Priority::kHigh, false, 0 });
		index_ += 4;
	}
	else if (str_.substr(index_, 4) == "sqrt") {
		CheckAction(action_is_active, operand);
		digital_.push_back(Node{ Actions::kSqrt, Priority::kHigh, false, 0 });
		index_ += 4;
	}
	else if (str_[index_] == '(') {
		CheckAction(action_is_active, operand);
		digital_.push_back(Node{ Actions::kOpeningBracket, Priority::kLowest, false, 0 });
		++index_;
	}
	else if (str_[index_] == ')') {
		digital_.push_back(Node{ Actions::kClosingBracket, Priority::kLowest, false, 0 });
		++index_;
	}
	else if (str_[index_] == '+' || str_[index_] == '-') {
		Actions action = str_[index_] == '-' ? Actions::kSub : Actions::kAdd;
		if (action_is_active) {
			digital_.push_back(Node{ action, Priority::kHighest, false, 0 });
		}
		else {
			digital_.push_back(Node{ action, Priority::kLow, true, 0 });
		}
		++index_;
	}
	else if (str_[index_] == '*' || str_[index_] == '/') {
		Actions action = str_[index_] == '*' ? Actions::kMul : Actions::kDiv;
		digital_.push_back(Node{ action, Priority::kMedium, true, 0 });
		++index_;
	}
	else if (str_[index_] == '^') {
		digital_.push_back(Node{ Actions::kPow, Priority::kHigh, true, 0 });
		++index_;
	}
}

int s21::Calculator::CreateRPN() {
	int error = 0;
	while (!digital_.empty() && !error) {
		if (digital_.begin()->action == Actions::kNumber) {
			rpn_.push_back(digital_.front());
		}
		else if (digital_.begin()->action == Actions::kSin || digital_.begin()->action == Actions::kCos ||
			digital_.begin()->action == Actions::kTan || digital_.begin()->action == Actions::kAsin ||
			digital_.begin()->action == Actions::kAcos || digital_.begin()->action == Actions::kAtan ||
			digital_.begin()->action == Actions::kLog || digital_.begin()->action == Actions::kLog10 ||
			digital_.begin()->action == Actions::kSqrt || digital_.begin()->action == Actions::kOpeningBracket) {
			stack_.push_front(digital_.front());
		}
		else if (digital_.begin()->action == Actions::kClosingBracket) {
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
				digital_.begin()->priority <= stack_.begin()->priority) {
				rpn_.push_back(stack_.front());
				stack_.pop_front();
			}
			stack_.push_front(digital_.front());
		}
		digital_.pop_front();
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
		digital_.clear();
		rpn_.clear();
		stack_.clear();
	}
	return error;
}

double s21::Calculator::CalculateAction(double first_operand, double second_operand,
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

int s21::Calculator::Calculate() {
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

					if ((action == '+' || action == '-') && !binary) {
						stack_.begin()->value = (action == '-' ? -stack_.begin()->value
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

double s21::Calculator::GetResult() { return result_; }
