#ifndef __MCC_LEXER__
#define __MCC_LEXER__

#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include "general.h"

enum TokenType
{
	// kieu du lieu
	TOK_CHAR_T, TOK_BOOL_T, TOK_SHORT_T, TOK_INT_T, TOK_LONG_T, TOK_LLONG_T,
	TOK_FLOAT_T, TOK_DOUBLE_T, TOK_LDOUBLE_T,
	TOK_STRING_T,
	// dinh danh va 1 so chuc nang co ban/Operators...
	TOK_ID, 
	TOK_EQUAL_O, TOK_PLUS_O, TOK_MIN_O, TOK_STAR_O, TOK_SLASH_O, TOK_PERCENT_O,
	TOK_EQEQ_O, TOK_NEQ_O, TOK_SMALL_O, TOK_LARGE_O, TOK_SEQ_O, TOK_LEQ_O, TOK_NOT_O,
	TOK_LBRACE_O, TOK_RBRACE_O, // operator<{}>
	TOK_2DOLLAR_O, // operator<$$> 
	TOK_COMMA_O,
	//Keyword/Operators...
	TOK_IF_K, TOK_ELIF_K, TOK_ELSE_K,
	TOK_FUNCTION_K,
	TOK_FOR_K, TOK_WHILE_K,

	TOK_SEMICOL, TOK_NUM_INT, TOK_NUM_FLOAT, TOK_STRLIT,
	TOK_LNGOAC_O, TOK_PNGOAC_O,
	TOK_EOF, TOK_UWHAT
};
struct TokenStruct {
	enum TokenType toktype;
	class std::basic_string<char> value;
};
class MCCLexer
{
private:
	class std::basic_string<char> code;
	size_t pos = 0;
public:
	MCCLexer(const std::string& s): code(s) {}
	class std::vector<TokenStruct> lexer(void) noexcept
	{
		std::vector<TokenStruct> token;
		while (pos < code.size())
		{
			char ch = code[pos];
			if (std::isspace(ch)) {
				pos++;
				continue;
			}
			// lexer...
			// cac tu keyword -> ID dinh danh
			if (std::isalpha(ch) || ch == '_')
			{
				std::string word = "";
				while (pos < code.size() &&
					(std::isalnum(code[pos]) || 
					 code[pos] == '_')) {word += code[pos++];}
				// kieu du lieu (?_?)
				if (word == "char") {
					token.push_back({
						TOK_CHAR_T, word
					});
				}
				else if (word == "bool") {
					token.push_back({
						TOK_BOOL_T, word
					});
				}
				else if (word == "short") {
					token.push_back({
						TOK_SHORT_T, word
					});
				}
				else if (word == "int") {
					token.push_back({
						TOK_INT_T, word
					});
				}
				else if (word == "long") {
					token.push_back({
						TOK_LONG_T, word
					});
				}
				else if (word == "llong") {
					token.push_back({
						TOK_LLONG_T, word
					});
				}
				else if (word == "float") {
					token.push_back({
						TOK_FLOAT_T, word
					});
				}
				else if (word == "double") {
					token.push_back({
						TOK_DOUBLE_T, word
					});
				}
				else if (word == "ldouble") {
					token.push_back({
						TOK_LDOUBLE_T, word
					});
				}
				else if (word == "string_t") {
					token.push_back({
						TOK_STRING_T, word
					});
				}
				else if (word == "if") {
					token.push_back({
						TOK_IF_K, word
					});
				}
				else if (word == "elif") {
					token.push_back({
						TOK_ELIF_K, word
					});
				}
				else if (word == "else") {
					token.push_back({
						TOK_ELSE_K, word
					});
				}
				else if (word == "for") {
					token.push_back({
						TOK_FOR_K, word
					});
				}
				else if (word == "while") {
					token.push_back({
						TOK_WHILE_K, word
					});
				}
				else if (word == "function") {
					token.push_back({
						TOK_FUNCTION_K, word
					});
				}
				else {
					token.push_back({
						TOK_ID, word
					});
				}
			}
			// so (nguyen va thuc (x thuoc Z/R))
			else if (std::isdigit(ch) || ch == '.')
			{
				std::string so = "";
				bool R = 0;
				while (pos < code.size() &&
					(std::isdigit(code[pos]) || 
					 code[pos] == '.')) { 
					if (code[pos] == '.') {
						if (R) {
							errprint(1, "invalid suffix for float number");
							return {{TOK_EOF, ""}};
						}
						R = 1;

					}
					so += code[pos++];
				}
				token.push_back({
					(R) ? TOK_NUM_FLOAT : TOK_NUM_INT,
					so
				});
			}
			// stting literal
			else if (ch == '"') 
			{
				// VD: "Hello Everyone ^^ XD :)" 
				pos++;
				std::string chuoi;
				while (pos < code.size() && code[pos] != '"') {
					chuoi += code[pos++];
				}
				if (pos >= code.size()) {
					errprint(0, "excepted '\"' at string");
					return {{TOK_EOF, ""}};
				} // thieu '"'
				token.push_back({TOK_STRLIT, chuoi});
				pos++;
			}
			// symbol...
			else
			{
				// operator
				if (ch == '=') 
				{
					bool is_eq = (pos + 1 < code.size() && code[pos + 1] == '=');
					token.push_back({
						(is_eq) ? TOK_EQEQ_O : TOK_EQUAL_O, (is_eq) ? "==" : "="
					});
					if (is_eq) { pos++; }
				}
				else if (ch == '+') {
					token.push_back({
						TOK_PLUS_O, "+"
					});
				}
				else if (ch == '-') {
					token.push_back({
						TOK_MIN_O, "-"
					});
				}
				else if (ch == '*') {
					token.push_back({
						TOK_STAR_O, "*"
					});
				}
				else if (ch == '/') {
					token.push_back({
						TOK_SLASH_O, "/"
					});
				}
				else if (ch == '%') {
					token.push_back({
						TOK_PERCENT_O, "%"
					});
				}
				else if (ch == '!') {
					bool is_eq = (pos + 1 < code.size() && code[pos + 1] == '=');
					token.push_back({
						(is_eq) ? TOK_NEQ_O : TOK_NOT_O, (is_eq) ? "!=" :"!"
					});
					if (is_eq) { pos++; }
				}
				else if (ch == '(') {
					token.push_back({
						TOK_LNGOAC_O, "("
					});
				}
				else if (ch == ')') {
					token.push_back({
						TOK_PNGOAC_O, ")"
					});
				}
				else if (ch == '<') {
					bool is_eq = (pos + 1 < code.size() && code[pos + 1] == '=');
					token.push_back({
						(is_eq) ? TOK_SEQ_O :TOK_SMALL_O, (is_eq) ? "<=" :"<"
					});
					if (is_eq) { pos++; }
				}
				else if (ch == '>') {
					bool is_eq = (pos + 1 < code.size() && code[pos + 1] == '=');
					token.push_back({
						(is_eq) ? TOK_LEQ_O : TOK_LARGE_O, (is_eq) ? ">=" :">"
					});
					if (is_eq) { pos++; }
				}
				else if (ch == '$')
				{
					if (pos + 1 < code.size() && code[pos + 1] == '$') {
						token.push_back({
							TOK_2DOLLAR_O, "$$"
						});
						pos++;
					}
					else {
						errprint(1, "excepted '$' after '$' (request 2 dollar, symbol: `$$`)");
						return {{TOK_EOF, ""}};
					}
				}
				else if (ch == ',') {
					token.push_back({
						TOK_COMMA_O, ","
					});
				}
				else if (ch == '{') {
					token.push_back({
						TOK_LBRACE_O, "{"
					});
				}
				else if (ch == '}') {
					token.push_back({
						TOK_RBRACE_O, "}"
					});
				}
				else if (ch == ';') {
					token.push_back({
						TOK_SEMICOL, ";"
					});
				}
				else { 
					errprint(1, "unexcepted char: ", ch);
					return {{TOK_EOF, ""}};
				};
				pos++;
			}
		}
		token.push_back({TOK_EOF, ""});
		return token;
	}
};

#endif
