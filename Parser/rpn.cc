#include "rpn.h"
using namespace s21;

std::pair<std::list<Node>, bool> Notation::CreateRPN(std::list<Node> tokens) {
	bool error = 0;
	tokens_ = tokens;

	while (!tokens_.empty() && !error) {
		auto token = tokens_.begin()->action;

		switch (token) {
		case kNumber:
			rpn_.push_back(tokens_.front());
			break;
		case kSin:
		case kCos:
		case kTan:
		case kAsin:
		case kAcos:
		case kAtan:
		case kLog:
		case kLog10:
		case kSqrt:
		case kOpeningBracket:
			temporary_stack_.push_front(tokens_.front());
			break;
		case s21::kClosingBracket:
			error = ClosingBracketItem();
			if (error) return Clear();
			break;
		default:
			DefaultItem();
			break;
		}
		tokens_.pop_front();
	}

	while (!temporary_stack_.empty()) {
		if (temporary_stack_.begin()->action == Actions::kOpeningBracket) {
			return Clear();
		}
		rpn_.push_back(temporary_stack_.front());
		temporary_stack_.pop_front();
	}

	auto res = std::make_pair(rpn_, error);
	return res;
}

bool Notation::ClosingBracketItem() {
	while (!temporary_stack_.empty() && temporary_stack_.begin()->action != kOpeningBracket) {
		rpn_.push_back(temporary_stack_.front());
		temporary_stack_.pop_front();
	}
	if (temporary_stack_.empty())
		return 1;
	else
		temporary_stack_.pop_front();
	return 0;
}

void Notation::DefaultItem() {
	while (!temporary_stack_.empty() &&
		tokens_.begin()->priority <= temporary_stack_.begin()->priority) {
		rpn_.push_back(temporary_stack_.front());
		temporary_stack_.pop_front();
	}
	temporary_stack_.push_front(tokens_.front());
}

std::pair<std::list<Node>, bool> Notation::Clear() {
	tokens_.clear();
	rpn_.clear();
	temporary_stack_.clear();
	return std::make_pair(rpn_, true);
}
