#include <iostream>
#include <memory>

#include "Expr.h"

using namespace std;

class AstPrinter : public enable_shared_from_this<AstPrinter> {
	any print(shared_ptr<Expr> expr) {
		return expr->accept();
	}
};
