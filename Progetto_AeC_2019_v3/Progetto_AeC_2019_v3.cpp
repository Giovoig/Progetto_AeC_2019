#include "pch.h"
#include "ExprParser.h"
#include <string>
#include <vector>
#include <iostream>

int main()
{
	vector<string> str_expr_v;
	
	str_expr_v.push_back("x = (A and (B or C)) or not D");

	ExprParser expr_parser(str_expr_v);

	return 0;
}
