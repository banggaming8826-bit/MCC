#ifndef __MCC_PARSER__
#define __MCC_PARSER__

#include <iostream>
#include "lexer.h"
#include "ast.h"
#include "general.h"
#include <string>

//Xay dung class "OOP"
class MCCParser
{
private:
	std::vector<TokenStruct> tok;
	size_t ht = 0;
	// method token's (bool = char, _Bool = bool):
	inline TokenStruct& gettok (void) noexcept { return tok[ht]; }	 // peek
	inline TokenStruct& dop_tok(void) noexcept { return tok[ht++]; } // an com
	inline bool chdop_tok(TokenType type) noexcept 
	{
		return (gettok().toktype == type) ? [&](void) -> bool {
				dop_tok();
				return 1;
			}() : 0;
	} // match
	inline bool is_type(TokenType tok) noexcept
	{
		switch(tok) 
		{
			// So nguyen (x thuoc Z)
			case TOK_CHAR_T:
			case TOK_BOOL_T:
			case TOK_SHORT_T:
			case TOK_INT_T:
			case TOK_LONG_T:
			case TOK_LLONG_T:
			// So thuc (x thuoc R)
			case TOK_FLOAT_T:
			case TOK_DOUBLE_T:
			case TOK_LDOUBLE_T:
			// Chuoi ki tu (string_t)
			case TOK_STRING_T:
				return 1;
				break;
			default:
				return 0;
				break;
		};
	}	
	enum DataType get_edtype(const std::string& s) noexcept
	{
		if (s.empty()) 					{ return DT_UWHAT;	}
		else if (s == "char") 				{ return DT_CHAR;  	}
		else if (s == "bool") 				{ return DT_BOOL;  	}
		else if (s == "short") 				{ return DT_SHORT; 	}
		else if (s == "int") 				{ return DT_INT;   	}
		else if (s == "long") 				{ return DT_LONG;  	}
		else if (s == "llong") 				{ return DT_LLONG; 	}
		else if (s == "float") 				{ return DT_FLOAT; 	}
		else if (s == "double") 			{ return DT_DOUBLE; 	}
		else if (s == "ldouble") 			{ return DT_LDOUBLE; 	}
		else 						{ return DT_UWHAT; 	}
	}
public:
	explicit MCCParser(const std::vector<TokenStruct>& vt): tok(vt) {}
	~MCCParser(void) = default;
	// bieu thuc
	// truong hop moi
	inline Node* parse_expression(void) noexcept;
	inline Node* parse_op_compare(void) noexcept;
	inline Node* parse_statement(void) noexcept;
	inline Node* parse_if(void) noexcept;
	inline Node* parse_while(void) noexcept;
	inline Node* parse_for(void) noexcept;
	inline Node* parse_function(void) noexcept;
	inline Barrel_Node* parse_block(void) noexcept;
	inline Node* parse_op_term(void) noexcept;
	inline Node* parse_op_fact(void) noexcept;

	// truong hop cu
	inline size_t tokvect_size(TokenType tok = TOK_ID) noexcept 
	{
		size_t sz = 0;
		for (const auto& v : this->tok) {
			if (v.toktype == tok) { sz++; }
		}
		return sz;
	}
	inline Node* parse_vardecl(bool semi = true) noexcept
	{
		// cu phap mau: "type name = gia tri / bieu thuc / bien khac (COPY)"
		if (!is_type(this->gettok().toktype)) {
			errprint(1, 
				"unknown type / excepted type in decl.");
			return NULL;
		}
		std::string type = dop_tok().value;
		if (gettok().toktype != TOK_ID) {
			errprint(1,"excepted identifier");
			return NULL;
		}
		std::string name = dop_tok().value;
		Node* gtkt = NULL;
		if (chdop_tok(TOK_EQUAL_O)) {
			gtkt = parse_op_compare();
		}
		if (!this->chdop_tok(TOK_SEMICOL) && semi) {
			errprint(1, "excepted \';\'");
			return NULL;
		}
		auto* ptr = new VarDecl_Node(type, name, gtkt);
		ptr->type_enum = this->get_edtype(type);
		return ptr;
	}
	inline Node* parse_assign(bool semi = true) noexcept
	{
		if (gettok().toktype != TOK_ID) {
			errprint(1, "excepted identifier");
			return NULL;
		}
		std::string name = dop_tok().value;
		if (!chdop_tok(TOK_EQUAL_O)) {
			errprint(1, "excepted operator'=' in assign expression");
			return NULL;
		}
		Node* bieu_thuc = parse_op_compare();
		if (!this->chdop_tok(TOK_SEMICOL) && semi) {
			errprint(1, "excpeted ';'");
			return NULL;
		}
		auto* ptr = new Assign_Node(name, bieu_thuc);
		return ptr;
	}
	// ham chinh (nam/nu 9)
	inline Barrel_Node* parse(void) noexcept 
	{
		auto* ptr = new Barrel_Node;
		while (this->gettok().toktype != TOK_EOF) {
			ptr->st.push_back(this->parse_statement());
		}
		return ptr;
	}
};
inline Node* MCCParser::parse_expression(void) noexcept 
{
	// a + b, a - b / (bieu thuc) + (bieu thuc), (bieu thuc) - (bieu thuc)
	Node* trai = MCCParser::parse_op_term(); // 
	while ( MCCParser::gettok().toktype == TOK_PLUS_O ||
		MCCParser::gettok().toktype == TOK_MIN_O
	) {
		TokenStruct o = MCCParser::dop_tok();
		Node* phai = MCCParser::parse_op_term();
		trai = new BinOp2_Node(trai, o, phai); 
	}
	return trai; // (bt...)
}
inline Node* MCCParser::parse_op_compare(void) noexcept
{
	Node* lat = MCCParser::parse_expression();
	while (
		gettok().toktype == TOK_EQEQ_O  || gettok().toktype == TOK_NEQ_O   ||
		gettok().toktype == TOK_SMALL_O || gettok().toktype == TOK_LARGE_O ||
		gettok().toktype == TOK_SEQ_O	|| gettok().toktype == TOK_LEQ_O	
	) {
		TokenStruct op = MCCParser::dop_tok();
		Node* lap = MCCParser::parse_expression();
		auto* ptr = new BinOp2_Node(lat, op, lap);
		ptr->type_enum = DT_BOOL;
		lat = ptr;
	}
	return lat;
}
inline Node* MCCParser::parse_op_term(void) noexcept 
{
	// a * b, a / b / (bieu thuc) * (bieu thuc), (bieu thuc) / (bieu thuc)
	// (cho ca operator`%')
	Node* trai = MCCParser::parse_op_fact(); // 
	while ( MCCParser::gettok().toktype == TOK_STAR_O ||
		MCCParser::gettok().toktype == TOK_SLASH_O ||
		MCCParser::gettok().toktype == TOK_PERCENT_O
	) {
		TokenStruct o = MCCParser::dop_tok();
		Node* phai = MCCParser::parse_op_fact();
		trai = new BinOp2_Node(trai, o, phai); 
	}
	return trai; // (bt...)
}
inline Node* MCCParser::parse_op_fact(void) noexcept 
{
	TokenStruct a = MCCParser::gettok();
	switch(a.toktype)	
	{
		case TOK_NUM_INT:
			return new IntNum_Node(MCCParser::dop_tok().value); 
			break;
		case TOK_NUM_FLOAT:
			return new FloatNum_Node(MCCParser::dop_tok().value);
			break;
		case TOK_ID:
			return new VarRef_Node(MCCParser::dop_tok().value);
			break;
		case TOK_LNGOAC_O:
			MCCParser::dop_tok();
			Node* tien = MCCParser::parse_op_compare();
			if (MCCParser::dop_tok().toktype != TOK_PNGOAC_O) {
				errprint(1,
					"excepted ')' at expression");
				return NULL;
			}
			return tien;
			break;
	}
	errprint(1, "excepted number");
	return NULL;
}
inline Barrel_Node* MCCParser::parse_block(void) noexcept
{
	if (!(this->chdop_tok(TOK_LBRACE_O))) {
		errprint(1, "excepted '{' in block statement");
		return NULL;
	}
	auto* ptr = new Barrel_Node;
	while (this->gettok().toktype != TOK_RBRACE_O) { ptr->st.push_back(this->parse_statement()); }
	return [&](void) -> Barrel_Node* {
		this->dop_tok();
		return ptr;
	}();
}
inline Node* MCCParser::parse_statement(void) noexcept
{
	// Cac statement chi co the la . . . if node/function node/. . .
	if (this->is_type(this->gettok().toktype)) { return this->parse_vardecl(); }
	switch(this->gettok().toktype)
	{
		case TOK_IF_K:
			return this->parse_if();
			break;
		case TOK_WHILE_K:
			return this->parse_while();
			break;
		case TOK_FOR_K:
			return this->parse_for();
			break;
		case TOK_FUNCTION_K:
			return this->parse_function();
			break;
		case TOK_LBRACE_O:
			return this->parse_block();
			break;
		case TOK_ID:
			return (this->tok[this->ht + 1].toktype == TOK_EQUAL_O &&
				ht + 1 < tok.size())				? this->parse_assign() 
										: this->parse_expression();
			break;
		default:
			return this->parse_expression();
			break;
	}
}
inline Node* MCCParser::parse_if(void) noexcept
{
	// if (dk...) { ... } / elif (dk...) { ... } / else { ... }
	this->dop_tok();
	if (!(this->chdop_tok(TOK_LNGOAC_O))) {
		errprint(1, "excepted `(' at if/elif");
		return NULL;
	}
	Node* dk = this->parse_op_compare();
	if (!(this->chdop_tok(TOK_PNGOAC_O))) {
		errprint(1, "excepted `)' at if/elif");
		return NULL;
	}
	Barrel_Node* then = this->parse_block();
	Node* conlai	  = NULL;
	if (this->chdop_tok(TOK_ELIF_K)) {
		this->gettok().toktype = TOK_IF_K;
		conlai = this->parse_if();
	}
	else if (this->chdop_tok(TOK_ELSE_K)) {
		conlai = this->parse_block();
	}
	return new Condition_Node(dk, then, conlai);
}
inline Node* MCCParser::parse_while(void) noexcept
{
	// while (dk...)
	this->dop_tok();
	if (!this->chdop_tok(TOK_LNGOAC_O)) {
		errprint(1, "excepted '('");
		return NULL;
	}
	Node* dk = this->parse_op_compare();
	if (!this->chdop_tok(TOK_PNGOAC_O)) {
		errprint(1, "excepted ')'");
		return NULL;
	}
	return new WhileLoop_Node(dk, this->parse_block());
}
inline Node* MCCParser::parse_for(void) noexcept
{
	// khi NULL == nullptr...
	// for (init $$ dk... $$ update) { ... }
	this->dop_tok();
	if (!this->chdop_tok(TOK_LNGOAC_O)) {
		errprint(1, "excepted '('");
		return NULL;
	}
	Node* khoitao = NULL;
	if (this->is_type(this->gettok().toktype)) { khoitao = this->parse_vardecl(0); }
	else { khoitao = this->parse_assign(0); }

	if (!this->chdop_tok(TOK_2DOLLAR_O)) {
		errprint(1, "excepted '$$' to init expression (for loop)");
		return NULL;
	}
	Node* dk = this->parse_op_compare();
	if (!this->chdop_tok(TOK_2DOLLAR_O)) {
		errprint(1, "excepted '$$' to condition expression (for loop)");
		return NULL;
	}
	Node* update = this->parse_assign(0);
	if (!this->chdop_tok(TOK_PNGOAC_O)) {
		errprint(1, "excepted ')'");
		return NULL;
	}
	return new ForLoop_Node(khoitao, dk, update, this->parse_block());
}
inline Node* MCCParser::parse_function(void) noexcept 
{
	// function<Type> name(args...) <thuoc tinh hanh vi (chua them)> { ... }
	this->dop_tok();
	if (!this->chdop_tok(TOK_SMALL_O)) {
		errprint(1, "excepted '<' at function declaration");
		return NULL;
	}
	if (!this->is_type(this->gettok().toktype)) {
		errprint(1, "excepted return type at function declaration");
		return NULL;
	}
	const auto& kieutv = this->dop_tok().value;
	if (!this->chdop_tok(TOK_LARGE_O)) {
		errprint(1, "excepted '>' at function declaration");
		return NULL;
	}
	if (this->gettok().toktype != TOK_ID) {
		errprint(1, "excepted function name at function declaration");
		return NULL;
	}
	std::string name = this->dop_tok().value;
	if (!this->chdop_tok(TOK_LNGOAC_O)) {
		errprint(1, "excepted '(' at function declaration");
		return NULL;
	}
	std::vector<VarDecl_Node*> args;
	while (this->gettok().toktype != TOK_PNGOAC_O) 
	{
		auto* ptr = dynamic_cast<VarDecl_Node*>(this->parse_vardecl(0));
		if ( !ptr) {
			return NULL;
		}
		args.push_back(ptr);
		if (this->gettok().toktype == TOK_COMMA_O) { this->dop_tok(); }
		else { break; }
	}
	if (!this->chdop_tok(TOK_PNGOAC_O)) {
		errprint(1, "excepted ')' at function declaration");
		return NULL;
	}
	return new Function_Node(kieutv, name, args, this->parse_block());
}

#endif
