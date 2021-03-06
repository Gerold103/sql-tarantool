
#include "Expr.h"
#include <stdio.h>
#include <string.h>

namespace hsql {

char* substr(const char* source, int from, int to) {
	if (source == NULL) {
		char *res = new char[1];
		res[0] = '\0';
		return res;
	}
	int len = to-from;
	char* copy = new char[len+1];
	strncpy(copy, source+from, len);
	copy[len] = '\0';
	return copy;
}

std::string ExprTypeToString(ExprType tp)
{
	switch(tp)
	{
		case ExprType::kExprLiteralFloat:
			return "kExprLiteralFloat";
		case ExprType::kExprLiteralString:
			return "kExprLiteralString";
		case ExprType::kExprLiteralInt:
			return "kExprLiteralInt";
		case ExprType::kExprStar:
			return "kExprStar";
		case ExprType::kExprPlaceholder:
			return "kExprPlaceholder";
		case ExprType::kExprColumnRef:
			return "kExprColumnRef";
		case ExprType::kExprFunctionRef:
			return "kExprFunctionRef";
		case ExprType::kExprOperator:
			return "kExprOperator";
		default:
			return "#undef#";
	}
}


std::string Expr::OperatorTypeToString(OperatorType tp)
{
	switch(tp)
	{
		case OperatorType::SIMPLE_OP:
			return "SIMPLE_OP";
		case OperatorType::IF:
			return "IF";
		case OperatorType::NOT_EQUALS:
			return "NOT_EQUALS";
		case OperatorType::LESS_EQ:
			return "LESS_EQ";
		case OperatorType::GREATER_EQ:
			return "GREATER_EQ";
		case OperatorType::LIKE:
			return "LIKE";
		case OperatorType::NOT_LIKE:
			return "NOT_LIKE";
		case OperatorType::AND:
			return "AND";
		case OperatorType::OR:
			return "OR";
		case OperatorType::NOT:
			return "NOT";
		case OperatorType::UMINUS:
			return "UMINUS";
		case OperatorType::ISNULL:
			return "ISNULL";
		default:
			return "#undef#";
	}
}

Expr* Expr::makeOpUnary(OperatorType op, Expr* expr) {
	Expr* e = new Expr(kExprOperator);
	e->op_type = op;
	e->expr = expr;
	e->expr2 = NULL;
	return e;
}



Expr* Expr::makeOpBinary(Expr* expr1, OperatorType op, Expr* expr2) {
	Expr* e = new Expr(kExprOperator);
	e->op_type = op;
	e->op_char = 0;
	e->expr = expr1;
	e->expr2 = expr2;
	return e;
}

Expr* Expr::makeOpBinary(Expr* expr1, char op, Expr* expr2) {
	Expr* e = new Expr(kExprOperator);
	e->op_type = SIMPLE_OP;
	e->op_char = op;
	e->expr = expr1;
	e->expr2 = expr2;
	return e;
}

Expr* Expr::makeOpTernary(Expr* expr1, Expr* expr2, Expr* expr3, OperatorType tp) {
	Expr* e = new Expr(kExprOperator);
	e->op_type = tp;
	e->op_char = 0;
	e->expr = expr1;
	e->expr2 = expr2;
	e->expr3 = expr3;
	return e;
}

Expr* Expr::makeLiteral(int64_t val) {
	Expr* e = new Expr(kExprLiteralInt);
	e->ival = val;
	return e;
}

Expr* Expr::makeLiteral(double value) {
	Expr* e = new Expr(kExprLiteralFloat);
	e->fval = value;
	return e;
}

Expr* Expr::makeLiteral(std::string string) {
	Expr* e = new Expr(kExprLiteralString);
	e->name = string;
	return e;
}


Expr* Expr::makeColumnRef(const char *name) {
	Expr* e = new Expr(kExprColumnRef);
	e->SetName(name);
	return e;
}

Expr* Expr::makeColumnRef(const char *table, const char *name) {
	Expr* e = new Expr(kExprColumnRef);
	e->SetName(name);
	e->SetTable(table);
	return e;
}

Expr* Expr::makeColumnRef(const char *database, const char *table, const char *name)
{
	Expr *e = new Expr(kExprColumnRef);
	e->SetDatabase(database);
	e->SetName(name);
	e->SetTable(table);
	return e;
}

Expr* Expr::makeFunctionRef(const char * func_name, Expr* expr, bool distinct) {
	Expr* e = new Expr(kExprFunctionRef);
	e->SetName(func_name);
	e->expr = expr;
	e->distinct = distinct;
	return e;
}

Expr* Expr::makePlaceholder(int id) {
	Expr* e = new Expr(kExprPlaceholder);
	e->ival = id;
	return e;
}

Expr::~Expr() {
	delete expr;
	delete expr2;
	delete expr3;
}

std::ostream &operator<<(std::ostream &stream, const Expr &ob)
{
	stream << "Expr: " << std::endl;
	stream << "expr_type: " << ExprTypeToString(ob.type) << std::endl;
	stream << "op_type: " << Expr::OperatorTypeToString(ob.op_type) << std::endl;
	stream << "op_char: " << ob.op_char << "; distinct: " << ob.distinct << std::endl;
	if (ob.expr != NULL) stream << "expr: " << *(ob.expr) << std::endl;
	if (ob.expr2 != NULL) stream << "expr2: " << *(ob.expr2) << std::endl;
	if (ob.expr3 != NULL) stream << "expr3: " << *(ob.expr3) << std::endl;
	if (!ob.name.empty()) stream << "name: " << ob.name << std::endl;
	if (!ob.table.empty()) stream << "table: " << ob.table << std::endl;
	if (!ob.database.empty()) stream << "database: " << ob.database << std::endl;
	if (!ob.alias.empty()) stream << "alias: " << ob.alias << std::endl;
	stream << "fval: " << ob.fval << "; ival: " << ob.ival << "; ival2: " << ob.ival2 << std::endl;
	return stream;
} 

} // namespace hsql