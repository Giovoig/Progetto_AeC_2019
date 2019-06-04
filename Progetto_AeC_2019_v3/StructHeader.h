#ifndef STRUCT_HEADER_H  //begin header guard
#define STRUCT_HEADER_H

#include <string>
#include <vector>
using namespace std;

enum Type
{
	VARIABLE, FLIPFLOP, AND, OR, NOT, LEFT_BR, RIGHT_BR, SUCCESS, ERROR
};

struct Token
{
	Type type;
	string id = "";
	int val = -2;
};

struct ParsExpr
{
	Token output;
	vector<Token> expr;
};

struct PostExpr
{
	Token output;
	vector<Token> expr;
};

#endif