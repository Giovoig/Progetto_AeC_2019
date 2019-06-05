#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include "StructHeader.h"
using namespace std;

class Node
{
protected:
	Type _node_type;

public:
	Node();
	~Node();

	Type getType();

	virtual int evaluate(const vector<Token> &input_vars) = 0;
	virtual void update() = 0;
	virtual Node* find(string id, Type node_type) = 0;
	virtual void setInput(Node* input) = 0;
	
};


class Operator : public Node
{
private:
	Node* _left = nullptr;
	Node* _right = nullptr;
	int _curr_val = -2;
	int _prev_val = -2;

	int and2(int a, int b);
	int or2(int a, int b);
	int not2(int a);

public:
	Operator(Node* left, Node* right, Type op_type);
	~Operator();

	int evaluate(const vector<Token> &input_vars);
	void update();
	Node* find(string id, Type node_type);
	void setInput(Node* input) { return; };
};


class Operand : public Node
{
protected:
	string _id;

public:
	Operand();
	~Operand();

	string getId();
	virtual int evaluate(const vector<Token> &input_vars) = 0;
	virtual void update() = 0;
	Node* find(string id, Type node_type);
	virtual void setInput(Node* input) = 0;
};


class Variable : public Operand
{
private:
	int _val = -2;

public:
	Variable(string id);
	~Variable();

	int evaluate(const vector<Token> &input_vars);
	void update() { return; };
	void setInput(Node* input) { return; };
};


class FlipFlop : public Operand
{
private:
	int _val_mem = -1;
	int _val_to_load = -1;
	Node* _input;

public:
	FlipFlop(string id);
	~FlipFlop();

	int evaluate(const vector<Token> &input_vars);
	void update();
	void setInput(Node* input);
};


struct Tree
{
	Operand* output;
	Node* tree_root;
};

#endif