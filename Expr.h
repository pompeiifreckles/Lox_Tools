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


template<class T> class Binary;
template<class T> class Grouping;
template<class T> class Literal;
template<class T> class Unary;

template<class T>
class Visitor {
public:
	virtual ~Visitor() = default;
	virtual T visitBinaryExpr(std::shared_ptr<Binary<T>> expr) = 0;
	virtual T visitGroupingExpr(std::shared_ptr<Grouping<T>> expr) = 0;
	virtual T visitLiteralExpr(std::shared_ptr<Literal<T>> expr) = 0;
	virtual T visitUnaryExpr(std::shared_ptr<Unary<T>> expr) = 0;
};

template<class T>
class Expr {
public:
	virtual ~Expr() = default;
	virtual T accept(std::shared_ptr<Visitor<T>> visitor) = 0;
};

template<class T>
class Binary : public Expr<T>, public std::enable_shared_from_this<Binary<T>> {
public:
	Binary(std::shared_ptr<Expr<T>> left, std::shared_ptr<Token> op, std::shared_ptr<Expr<T>> right) {
		this->left = left;
		this->op = op;
		this->right = right;
	}

	T accept(std::shared_ptr<Visitor<T>> visitor) override {
		return visitor->visitBinaryExpr( this->shared_from_this() );
	}

	std::shared_ptr<Expr<T>> left;
	std::shared_ptr<Token> op;
	std::shared_ptr<Expr<T>> right;
};

template<class T>
class Grouping : public Expr<T>, public std::enable_shared_from_this<Grouping<T>> {
public:
	Grouping(std::shared_ptr<Expr<T>> expression) {
		this->expression = expression;
	}

	T accept(std::shared_ptr<Visitor<T>> visitor) override {
		return visitor->visitGroupingExpr( this->shared_from_this() );
	}

	std::shared_ptr<Expr<T>> expression;
};

template<class T>
class Literal : public Expr<T>, public std::enable_shared_from_this<Literal<T>> {
public:
	Literal(std::shared_ptr<any> value) {
		this->value = value;
	}

	T accept(std::shared_ptr<Visitor<T>> visitor) override {
		return visitor->visitLiteralExpr( this->shared_from_this() );
	}

	std::shared_ptr<any> value;
};

template<class T>
class Unary : public Expr<T>, public std::enable_shared_from_this<Unary<T>> {
public:
	Unary(std::shared_ptr<Token> op, std::shared_ptr<Expr<T>> right) {
		this->op = op;
		this->right = right;
	}

	T accept(std::shared_ptr<Visitor<T>> visitor) override {
		return visitor->visitUnaryExpr( this->shared_from_this() );
	}

	std::shared_ptr<Token> op;
	std::shared_ptr<Expr<T>> right;
};

#endif
