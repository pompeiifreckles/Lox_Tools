#include <iostream>
#include <memory>
#include <initializer_list>
#include <any>

#include "Expr.h"

using namespace std;

class AstPrinter : public Visitor<string>, public enable_shared_from_this<AstPrinter> {
	template<class T>
	string parenthesize(string name, initializer_list<T> exprs) {

	}

	public:
	string print(shared_ptr<Expr<string>> expr) {
		return expr->accept( shared_from_this() );
	}

	string visitBinaryExpr(shared_ptr<Binary<string>> expr) {
		return parenthesize(expr->op->lexeme, { expr->left, expr->right });
	}

	string visitGroupingExpr(shared_ptr<Grouping<string>> expr) {
		return parenthesize("group", { expr->expression });
	}

	string visitLiteralExpr(shared_ptr<Literal<string>> expr) {
		if (expr->value == nullptr) return "nil";
		if (expr->value->type() == typeid(double)) {
			return any_cast<string>(expr->value);
		}
		return any_cast<string>(expr->value);
	}

	string visitUnaryExpr(shared_ptr<Unary<string>> expr) {
		return parenthesize(expr->op->lexeme, { expr->right });
	}
	
};
