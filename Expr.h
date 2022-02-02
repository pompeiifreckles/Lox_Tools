#ifndef CRAFTING_INTERPRETER_EXPR_H
#define CRAFTING_INTERPRETER_EXPR_H

#include <vector>
#include <memory>
#include <any>

#include "Token.h"

using std::any;

enum class Expression {
	Expr,
	Binary,
	Grouping,
	Literal,
	Unary
};


class Binary;
class Grouping;
class Literal;
class Unary;

class Visitor {
public:
	static any visitBinaryExpr(std::shared_ptr<Binary> expr);
	static any visitGroupingExpr(std::shared_ptr<Grouping> expr);
	static any visitLiteralExpr(std::shared_ptr<Literal> expr);
	static any visitUnaryExpr(std::shared_ptr<Unary> expr);
};

class Expr {
public:
	virtual any accept() = 0;
};

class Binary : public Expr, public std::enable_shared_from_this<Binary> {
public:
	Binary(std::shared_ptr<Expr> left, std::shared_ptr<Token> op, std::shared_ptr<Expr> right) {
		this->left = left;
		this->op = op;
		this->right = right;
	}

	any accept() override {
		return Visitor::visitBinaryExpr( shared_from_this() );
	}

	std::shared_ptr<Expr> left;
	std::shared_ptr<Token> op;
	std::shared_ptr<Expr> right;
};

class Grouping : public Expr, public std::enable_shared_from_this<Grouping> {
public:
	Grouping(std::shared_ptr<Expr> expression) {
		this->expression = expression;
	}

	any accept() override {
		return Visitor::visitGroupingExpr( shared_from_this() );
	}

	std::shared_ptr<Expr> expression;
};

class Literal : public Expr, public std::enable_shared_from_this<Literal> {
public:
	Literal(std::shared_ptr<any> value) {
		this->value = value;
	}

	any accept() override {
		return Visitor::visitLiteralExpr( shared_from_this() );
	}

	std::shared_ptr<any> value;
};

class Unary : public Expr, public std::enable_shared_from_this<Unary> {
public:
	Unary(std::shared_ptr<Token> op, std::shared_ptr<Expr> right) {
		this->op = op;
		this->right = right;
	}

	any accept() override {
		return Visitor::visitUnaryExpr( shared_from_this() );
	}

	std::shared_ptr<Token> op;
	std::shared_ptr<Expr> right;
};

#endif
