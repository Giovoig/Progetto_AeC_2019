#include "pch.h"
#include "ExprParser.h"
#include "StructHeader.h"
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <iostream>
using namespace std;


//Constructor & destructor


ExprParser::ExprParser(const vector<string> &str_expr_v, const vector<Token> &input_vars)
{
	_tree_v = flipFlopLink( postVToTreeV( parsVToPostV( stringVToParsV(str_expr_v) ) ) );
	int result = _tree_v[0].tree_root->evaluate(input_vars);
	cout << result;
}


ExprParser::~ExprParser()
{
}


//Private member functions


ParsExpr ExprParser::stringToPars(const string & str_expr)
{
	ParsExpr parsed_expr;
	Token token;
	stringstream ss(str_expr);

	token = getToken(ss);
	parsed_expr.output = token;
	ss.get(); //Rimuove dallo stream lo spazio
	ss.get(); //Rimuove dallo stream '='

	token = getToken(ss);
	while (token.type != SUCCESS && token.type != ERROR)
	{
		parsed_expr.expr.push_back(token);
		token = getToken(ss);
	}

	return parsed_expr;
}


Token ExprParser::getToken(stringstream & ss)
{
	char c;
	string s;
	int state = 0;
	Token token;

	ss.get(c);
	while (!ss.eof())
	{
		switch (state)
		{
		case 0:
			if (isalpha(c)) { s = toupper(c); state = 1; }
			else if (isspace(c)) { state = 0; }
			else if (c == '(') { token.type = LEFT_BR; return token; }
			else if (c == ')') { token.type = RIGHT_BR; return token; }
			else { token.type = ERROR; return token; }
			break;

		case 1:
			if (isalpha(c) || isdigit(c)) { s += toupper(c); }
			else if (isspace(c) || c == ')') { ss.putback(c); return checkWord(s); }
			//else if (isdigit(c)) { s += c; state = 2; }
			else { token.type = ERROR; return token; }
			break;

		/*case 2:
			if (isspace(c) || c == ')')
			{
				ss.putback(c);
				if (s[0] == 'F' && s[1] == 'F')
				{
					token.type = FLIPFLOP;
					token.id = s;
					return token;
				}
				else
				{
					token.type = ERROR;
					return token;
				}
			}
			else
			{
				token.type = ERROR;
				return token;
			}

			break;*/
		}
		ss.get(c);
	}

	if (s != "")
	{
		return checkWord(s);
	}
	else
	{
		token.type = SUCCESS;
		return token;
	}
}


Token ExprParser::checkWord(const string & s)
{
	Token token;

	if (s == "AND") { token.type = AND; return token; }
	else if (s == "OR") { token.type = OR; return token; }
	else if (s == "NOT") { token.type = NOT; return token; }
	else if (s[0] == 'F' && s[1] == 'F')
	{
		token.type = FLIPFLOP;
		token.id = s;
		return token;
	}
	else { token.type = VARIABLE; token.id = s; return token; }
	/*else if ((int)s.length() == 1) { full_token.token = variable_id; full_token.id = s; return full_token; }
	else { full_token.token = illegal_token; return full_token; }*/

	return token;
}


vector<ParsExpr> ExprParser::stringVToParsV(const vector<string>& str_expr_v)
{
	ParsExpr pars_expr;
	vector<ParsExpr> pars_expr_v;

	for (int i = 0; i < (int)str_expr_v.size(); i++)
	{
		pars_expr = stringToPars(str_expr_v[i]);
		pars_expr_v.push_back(pars_expr);
	}

	return pars_expr_v;
}


PostExpr ExprParser::parsToPost(const ParsExpr & pars_expr)
{
	stack <Token, vector <Token>> op_stack;

	PostExpr postfix_expr;

	postfix_expr.output = pars_expr.output;

	for (int i = 0; i < (int)pars_expr.expr.size(); i++)
	{
		switch (pars_expr.expr[i].type)
		{
		case AND:
		case OR:
		case NOT:
		case LEFT_BR:

			op_stack.push(pars_expr.expr[i]);

			break;

		case VARIABLE:
		case FLIPFLOP:

			postfix_expr.expr.push_back(pars_expr.expr[i]);

			while (!op_stack.empty() && op_stack.top().type != LEFT_BR)
			{
				postfix_expr.expr.push_back(op_stack.top());
				op_stack.pop();
			}

			break;

		case RIGHT_BR:

			op_stack.pop();

			while (!op_stack.empty() && op_stack.top().type != LEFT_BR)
			{
				postfix_expr.expr.push_back(op_stack.top());
				op_stack.pop();
			}

			break;
		}
	}

	while (!op_stack.empty())
	{
		postfix_expr.expr.push_back(op_stack.top());
		op_stack.pop();
	}

	return postfix_expr;
}


vector<PostExpr> ExprParser::parsVToPostV(const vector<ParsExpr>& pars_expr_v)
{
	PostExpr post_expr;
	vector<PostExpr> post_expr_v;

	for (int i = 0; i < (int)pars_expr_v.size(); i++)
	{
		post_expr = parsToPost(pars_expr_v[i]);
		post_expr_v.push_back(post_expr);
	}

	return post_expr_v;
}


Tree ExprParser::postToTree(const PostExpr & post_expr)
{
	stack<Node*> node_stack;
	Node* left = nullptr;
	Node* right = nullptr;
	Tree tree;

	switch (post_expr.output.type)
	{
	case VARIABLE:
		tree.output = new Variable(post_expr.output.id);
		break;

	case FLIPFLOP:
		tree.output = new FlipFlop(post_expr.output.id);
		break;
	}

	for (int i = 0; i < (int)post_expr.expr.size(); i++)
	{
		switch (post_expr.expr[i].type)
		{
		case VARIABLE:
			node_stack.push(new Variable(post_expr.expr[i].id));
			break;

		case FLIPFLOP:
			node_stack.push(new FlipFlop(post_expr.expr[i].id));
			break;

		case NOT:
			left = node_stack.top();
			node_stack.pop();

			node_stack.push(new Operator(left, nullptr, post_expr.expr[i].type));
			break;

		default: //case AND || OR
			left = node_stack.top();
			node_stack.pop();
			right = node_stack.top();
			node_stack.pop();

			node_stack.push(new Operator(left, right, post_expr.expr[i].type));
			break;
		}
	}

	tree.tree_root = node_stack.top();

	return tree;
}


vector<Tree> ExprParser::postVToTreeV(const vector<PostExpr>& post_expr_v)
{
	vector<Tree> tree_v;

	for (int i = 0; i < (int)post_expr_v.size(); i++)
	{
		tree_v.push_back(postToTree(post_expr_v[i]));
	}

	return tree_v;
}


vector<Tree> ExprParser::flipFlopLink(vector<Tree> vector_tree)
{
	/*
		Dato un vettore di alberi disgiunti, ricerca in ogni albero che ha in output una variabile tutti i FF e li connette con il corrispettivo albero (presente nel vettore di partenza).
		Es.
		x = FF1 AND FF2
		FF1 = a OR b
		FF2 = c AND d
		=> x = FF1->(a OR b) AND FF2->(c AND d)
	*/
	
	vector<Tree> output_var_trees;
	Tree curr_tree;
	Node* found;

	for (int i = 0; i < (int)vector_tree.size(); i++)
	{
		if (vector_tree[i].output->getType() == FLIPFLOP)
		{
			/*
				Dapprima, connetto tutti i FF nelle espressioni FF = ... ai corrispettivi alberi
				Es.
				FF2 = FF1 AND a
				FF1 = b OR c
				=> FF2 = FF1->(b OR c) AND a
			*/
			
			curr_tree = vector_tree[i];

			for (int j = 0; j < (int)vector_tree.size(); j++)
			{
				if (vector_tree[j].output->getType() == FLIPFLOP)
				{
					//vector_tree[j] è l'albero che ha come output il FF da cui prendere l'albero associato

					found = curr_tree.tree_root->find(vector_tree[j].output->getId(), FLIPFLOP); //Restituisce un puntatore al FF trovato nell'albero corrente (output = FF)

					if (found != nullptr)
					{
						found->setInput(vector_tree[j].tree_root);
					}
				}
			}
		}
	}

	for (int i = 0; i < (int)vector_tree.size(); i++)
	{
		/*
			Adesso, connetto tutti i FF nelle espressioni var = ... ai corrispettivi alberi
			Es.
			x = FF1 AND FF2
			FF1 = a OR b
			FF2 = c AND d
			=> x = FF1->(a OR b) AND FF2->(c AND d)
		*/
		
		if (vector_tree[i].output->getType() == VARIABLE)
		{
			curr_tree = vector_tree[i];
			
			for (int j = 0; j < (int)vector_tree.size(); j++)
			{
				if (vector_tree[j].output->getType() == FLIPFLOP)
				{
					//vector_tree[j] è l'albero che ha come output un flip flop
					
					found = curr_tree.tree_root->find(vector_tree[j].output->getId(), FLIPFLOP); //Restituisce un puntatore al flip flop trovato nell'albero corrente (output = variable)

					if (found != nullptr)
					{
						found->setInput(vector_tree[j].tree_root);
					}
				}
			}

			output_var_trees.push_back(curr_tree);
		}
	}

	return output_var_trees;
}
