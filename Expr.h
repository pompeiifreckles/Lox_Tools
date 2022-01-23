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

template <typename T>
class Visitor;

class Expr {
public:
	template <typename T>
	virtual void accept(std::shared_ptr<Visitor<T>> visitor);
};

class Binary : public Expr {
public:
	Binary(std::shared_ptr<Expr> left, std::shared_ptr<Token> op, std::shared_ptr<Expr> right) {
		this->left = left;
		this->op = op;
		this->right = right;
	}
	std::shared_ptr<Expr> left;
	std::shared_ptr<Token> op;
	std::shared_ptr<Expr> right;

	Expression getType() {
		return Expression::Binary;
	}
};

class Grouping : public Expr {
public:
	Grouping(std::shared_ptr<Expr> expression) {
		this->expression = expression;
	}
	std::shared_ptr<Expr> expression;

	Expression getType() {
		return Expression::Grouping;
	}
};

class Literal : public Expr {
public:
	Literal(std::shared_ptr<any> value) {
		this->value = value;
	}
	std::shared_ptr<any> value;

	Expression getType() {
		return Expression::Literal;
	}
};

class Unary : public Expr {
public:
	Unary(std::shared_ptr<Token> op, std::shared_ptr<Expr> right) {
		this->op = op;
		this->right = right;
	}
	std::shared_ptr<Token> op;
	std::shared_ptr<Expr> right;

	Expression getType() {
		return Expression::Unary;
	}
};

template <typename T>
class Visitor {
	T visitBinaryExpr(Binary expr);
	T visitGroupingExpr(Grouping expr);
	T visitLiteralExpr(Literal expr);
	T visitUnaryExpr(Unary expr);
};

#endif
