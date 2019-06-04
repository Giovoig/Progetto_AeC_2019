#ifndef EXPRPARSER_H
#define EXPRPARSER_H

#include <string>
#include <vector>
#include <list>
#include "Node.h"
#include "StructHeader.h"
using namespace std;

class ExprParser
{
public:

	//Constructor & destructor

	ExprParser(const vector<string> &str_expr_v);

	~ExprParser();

private:

	//Data

	vector<Tree> _tree_v;

	//Member functions

	ParsExpr stringToPars(const string &str_expr); //Richiamata solo in stringVToParsedV

	Token getToken(stringstream &ss); //Richiamata solo in stringToParsed

	Token checkWord(const string &s);

	vector<ParsExpr> stringVToParsV(const vector<string> &str_expr_v);

	PostExpr parsToPost(const ParsExpr &pars_expr); //Richiamata solo in parsedVToPostfixV

	vector<PostExpr> parsVToPostV(const vector<ParsExpr> &pars_expr_v);

	Tree postToTree(const PostExpr &post_expr); //Richiamata solo in postVToTreeV

	vector<Tree> postVToTreeV(const vector<PostExpr> &post_expr_v);
};

#endif