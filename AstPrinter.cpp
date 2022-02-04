#include <iostream>
#include <memory>
#include <initializer_list>
#include <any>

#include "Expr.h"

using namespace std;

class AstPrinter : public Visitor<string>, public enable_shared_from_this<AstPrinter> {
	string parenthesize(string name, initializer_list<shared_ptr<Expr<string>>> exprs) {
		string s;
		s += "(" + name;
		for (auto expr : exprs) {
			s += " ";
			s += expr->accept( this->shared_from_this() );
		}
		s += ")";

		return s;
	}

	public:
	string print(shared_ptr<Expr<string>> expr) {
		return expr->accept( this->shared_from_this() );
	}

	string visitBinaryExpr(shared_ptr<Binary<string>> expr) override {
		return parenthesize(expr->op->lexeme, { expr->left, expr->right });
	}

	string visitGroupingExpr(shared_ptr<Grouping<string>> expr) override {
		return parenthesize("group", { expr->expression });
	}

	string visitLiteralExpr(shared_ptr<Literal<string>> expr) override {
		if (expr->value->type() == typeid(nullptr) || expr->value->type() == typeid(NULL)) return "nil";
		if (expr->value->type() == typeid(int)) {
			return to_string(any_cast<int>(*expr->value));
		} else if (expr->value->type() == typeid(double)) {
			return to_string(any_cast<double>(*expr->value));
		} else if (expr->value->type() == typeid(const char *)) {
			return any_cast<const char *>(*expr->value);
		} else if (expr->value->type() == typeid(string)) {
			return any_cast<string>(*expr->value);
		}
		return expr->value->type().name();
	}

	string visitUnaryExpr(shared_ptr<Unary<string>> expr) override {
		return parenthesize(expr->op->lexeme, { expr->right });
	}
	
};

int main() {
	shared_ptr<Expr<string>> expression = make_shared<Binary<string>>(
			make_shared<Unary<string>>(
				make_shared<Token>(TokenType::MINUS, "-", nullptr, 1),
				make_shared<Literal<string>>(make_shared<any>(123))
				),
			make_shared<Token>(TokenType::STAR, "*", nullptr, 1),
			make_shared<Grouping<string>>( make_shared<Literal<string>>(make_shared<any>(45.67)) )
			);

	cout << make_shared<AstPrinter>()->print(expression) << endl;
}
