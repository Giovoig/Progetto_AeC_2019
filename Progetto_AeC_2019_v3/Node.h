#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include "StructHeader.h"
using namespace std;

class Node
{
public:
	Node();
	~Node();

	virtual int evaluate(const vector<Token> &input_vars) = 0;
};


class Operator : public Node
{
private:
	Node* _left = nullptr;
	Node* _right = nullptr;
	Type _op_type;
	int _curr_val = -2;
	int _prev_val = -2;

	int and2(int a, int b);
	int or2(int a, int b);
	int not2(int a);

public:
	Operator(Node* left, Node* right, Type op_type);
	~Operator();

	int evaluate(const vector<Token> &input_vars);
};


class Operand : public Node
{
protected:
	string _id;

public:
	Operand();
	~Operand();

	virtual int evaluate(const vector<Token> &input_vars) = 0;
};


class Variable : public Operand
{
private:
	int _val = -2;

public:
	Variable(string id);
	~Variable();

	int evaluate(const vector<Token> &input_vars);
};


class FlipFlop : public Operand
{
private:
	int _val_mem = -1;
	int _val_to_load = -1;

public:
	FlipFlop(string id);
	~FlipFlop();

	int evaluate(const vector<Token> &input_vars);
};


struct Tree
{
	Operand* output;
	Node* tree_root;
};

#endif