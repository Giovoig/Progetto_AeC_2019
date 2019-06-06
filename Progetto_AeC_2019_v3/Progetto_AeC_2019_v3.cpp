#include "ExprParser.h"
#include <string>
#include <vector>
#include <iostream>

int main()
{
	vector<string> str_expr_v;

	str_expr_v.push_back("x = FF1 and A");
	str_expr_v.push_back("FF1 = B");

	vector<Token> input_vars;
	
	Token token;
	token.type = VARIABLE;

	token.id = "A";
	token.val = 1;
	input_vars.push_back(token);

	token.id = "B";
	token.val = 1;
	input_vars.push_back(token);

	/*token.id = "C";
	token.val = 1;
	input_vars.push_back(token);*/

	ExprParser expr_parser(str_expr_v, input_vars);

	return 0;
}
