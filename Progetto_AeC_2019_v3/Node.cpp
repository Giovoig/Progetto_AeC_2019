#include "pch.h"
#include "Node.h"
#include "StructHeader.h"


Node::Node()
{
}


Node::~Node()
{
}


Type Node::getType()
{
	return _node_type;
}


int Operator::and2(int a, int b)
{
	if (a != -1 && b != -1)
	{
		return a && b;
	}
	else if ((a == 0 && b == -1) || (a == -1 && b == 0))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}


int Operator::or2(int a, int b)
{
	if (a != -1 && b != -1)
	{
		return a || b;
	}
	else if ((a == 1 && b == -1) || (a == -1 && b == 1))
	{
		return 1;
	}
	else
	{
		return -1;
	}
}


int Operator::not2(int a)
{
	if (a != -1)
	{
		return !a;
	}
	else
	{
		return -1;
	}
}


Operator::Operator(Node * left, Node * right, Type op_type)
{
	_left = left;
	_right = right;
	_node_type = op_type;
}


Operator::~Operator()
{
}


int Operator::evaluate(const vector<Token> &input_vars)
{
	_prev_val = _curr_val;
	
	switch (_node_type)
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


void Operator::update()
{
	_left->update();
	_right->update();
	return;
}


Node * Operator::find(string id, Type node_type)
{
	Node* found;
	
	found = _left->find(id, node_type);
	if (found != nullptr)
	{
		return found;
	}
	else
	{
		found = _right->find(id, node_type);
		if (found != nullptr)
		{
			return found;
		}
		else
		{
			return nullptr;
		}
	}
}


Variable::Variable(string id)
{
	_id = id;
	_node_type = VARIABLE;
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
	_node_type = FLIPFLOP;
}


FlipFlop::~FlipFlop()
{
}


int FlipFlop::evaluate(const vector<Token>& input_vars)
{
	_val_to_load = _input->evaluate(input_vars);
	return _val_mem;
}


void FlipFlop::update()
{
	_val_mem = _val_to_load;
	_val_to_load = -1;
	return;
}


void FlipFlop::setInput(Node * input)
{
	_input = input;
}


Operand::Operand()
{
}


Operand::~Operand()
{
}


Node * Operand::find(string id, Type node_type)
{
	if (id == _id && node_type == _node_type)
	{
		return this;
	}
	else
	{
		return nullptr;
	}
}


string Operand::getId()
{
	return _id;
}
