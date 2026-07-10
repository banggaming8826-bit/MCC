#ifndef __MCC_AST__
#define __MCC_AST__

#include <string>
#include "general.h"
#include "lexer.h"
#include <iostream>

enum DataType
{
	// nguyen
	DT_CHAR, DT_BOOL, DT_SHORT, DT_INT, DT_LONG, DT_LLONG,
	// thuc
	DT_FLOAT, DT_DOUBLE, DT_LDOUBLE,
	DT_UWHAT
};
struct Barrel_Node;
// Node goc
struct Node {
	virtual ~Node(void) = default;
	enum DataType type_enum;
};
// Khai bao bien
struct VarDecl_Node   : public Node
{
	std::string type, name;
	Node* khoitao;
	VarDecl_Node(const std::string& kieu, const std::string& ten, 
		Node* init = NULL): type(kieu), name(ten), khoitao(init) {}
};
struct BinOp2_Node    : public Node
{
	Node* trai, *phai;
	TokenStruct op2;
	BinOp2_Node(Node* l, TokenStruct t, Node* r): trai(l), op2(t), phai(r) {}
};
struct VarRef_Node    : public Node {
	std::string var;
	VarRef_Node(const std::string& v): var(v) {}
};
struct IntNum_Node    : public Node {
	std::string num;
	IntNum_Node(const std::string& x): num(x) {}
};
struct FloatNum_Node  : public Node {
	std::string num;
	FloatNum_Node(const std::string& x): num(x) {}
};
struct Assign_Node    : public Node {
	std::string name;
	Node* expr;
	Assign_Node(const std::string& ten, Node* init = NULL): name(ten), expr(init) {}
};
struct Condition_Node : public Node 
{
	Node* dk;
	Barrel_Node* lenh;
	Node* elst;
	Condition_Node(Node* d, Barrel_Node* l, Node* e = NULL):
		dk(d), lenh(l), elst(e) {}
};
struct ForLoop_Node   : public Node
{
	Node* khoitao;
	Node* dk;
	Node* update;
	Barrel_Node* lenh;

	ForLoop_Node(Node* init, Node* con, Node* cn, Barrel_Node* cmd):
		khoitao(init), dk(con), update(cn), lenh(cmd) {}
};
struct Function_Node  : public Node
{
	std::string rettype;
	std::string name;
	std::vector<VarDecl_Node*> args;
	Barrel_Node* lenh;
	Function_Node(const std::string& dt, const std::string& n, const std::vector<VarDecl_Node*>& vect, Barrel_Node* b):
		rettype(dt), name(n), args(vect), lenh(b) {}
};
struct FReturn_Node   : public Node 
{
	Function_Node* func;
	Node* value;
	FReturn_Node(Node* v, Function_Node* f): func(f), value(v) {}
};
struct WhileLoop_Node : public Node
{
	Node* dk;
	Barrel_Node* lenh;
	WhileLoop_Node (Node* d, Barrel_Node* l): dk(d), lenh(l) {}
};
// chung chung 9
struct Barrel_Node : public Node {
	std::vector<Node*> st;
};
#endif
