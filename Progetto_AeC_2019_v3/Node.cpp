#include "pch.h"
#include "Node.h"
#include "StructHeader.h"


Node::Node()
{
}


Node::~Node()
{
}


int Operator::and2(int a, int b)
{
	return 0;
}


int Operator::or2(int a, int b)
{
	return 0;
}


int Operator::not2(int a)
{
	return 0;
}


Operator::Operator(Node * left, Node * right, Type op_type)
{
	_left = left;
	_right = right;
	_op_type = op_type;
}


Operator::~Operator()
{
}


int Operator::evaluate(const vector<Token> &input_vars)
{
	_prev_val = _curr_val;
	
	switch (_op_type)
	{
	case AND:
		_curr_val = and2(_left->evaluate(input_vars), _right->evaluate(input_vars));
		return _curr_val;
		break;

	case OR:
		_curr_val = or2(_left->evaluate(input_vars), _right->evaluate(input_vars));
		return _curr_val;
		break;

	case NOT:
		_curr_val = not2(_left->evaluate(input_vars));
		return _curr_val;
		break;
	}

	return 0;
}


Variable::Variable(string id)
{
	_id = id;
}


Variable::~Variable()
{
}


int Variable::evaluate(const vector<Token>& input_vars)
{
	bool found = false;
	int i = 0;
	
	while (i < (int)input_vars.size() && found == false)
	{
		if (input_vars[i].id == _id)
		{
			found = true;
			_val = input_vars[i].val;
		}

		i++;
	}

	return _val;
}

FlipFlop::FlipFlop(string id)
{
	_id = id;
}


FlipFlop::~FlipFlop()
{
}


int FlipFlop::evaluate(const vector<Token>& input_vars)
{
	return _val_mem;
}

Operand::Operand()
{
}

Operand::~Operand()
{
}
