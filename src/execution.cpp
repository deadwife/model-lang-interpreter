#include <string>
#include <vector>
#include <fstream>
#include <map>
#include "prototypes.h"
#include "classes.h"
using namespace std;

void ThePolice::executioner_tri_topora(polizlexvector & polizvector, varmap IDmap)
{
	PolizLexem op1, op2;
	this->idmap = IDmap;
	for (it = polizvector.begin(); it != polizvector.end(); ++it)
	{
		switch (it->getid())
		{
		case CONSTANT: case FOX: case IDENTIFICATOR: case STRING: case LABEL: case TRUE: case FALSE:
		{
			stack777.push(*it);
			break;
		}
		case MORE: 
		{
			op2 = stack777.pop();
			op1 = stack777.pop();
			
			switch (gettype(op1))
			{
			case INTTYPE: stack777.push(PolizLexem(value(op1).int_value > value(op2).int_value)); break;
			case STRINGTYPE: stack777.push(PolizLexem(value(op1).string_value > value(op2).string_value)); break;
			case BOOLEANTYPE: throw 1; //bool > bool?!
			}
			break;
		}
		case LESS: 
		{
			op2 = stack777.pop();
			op1 = stack777.pop();
			switch (gettype(op1))
			{
			case INTTYPE: stack777.push(PolizLexem(value(op1).int_value < value(op2).int_value)); break;
			case STRINGTYPE: stack777.push(PolizLexem(value(op1).string_value < value(op2).string_value)); break;
			case BOOLEANTYPE: throw 1; //bool < bool?!
			}
			break;
		}
		case MOREQ: 
		{
			op2 = stack777.pop();
			op1 = stack777.pop();
			stack777.push(PolizLexem(value(op1).int_value >= value(op2).int_value));
			break;
		}
		case LESSQ: 
		{
			op2 = stack777.pop();
			op1 = stack777.pop();
			stack777.push(PolizLexem(value(op1).int_value <= value(op2).int_value));
			break; 
		}
		case PLUS: 
		{
			op2 = stack777.pop();
			op1 = stack777.pop();
			switch (gettype(op1))
			{
			case INTTYPE: stack777.push(PolizLexem(value(op1).int_value + value(op2).int_value)); break;
			case STRINGTYPE: stack777.push(PolizLexem(value(op1).string_value + value(op2).string_value)); break;
			case BOOLEANTYPE: throw 1; //bool + bool?!
			}
			break;
		}
		case MINUS: 
		{
			op2 = stack777.pop();
			op1 = stack777.pop();
			stack777.push(PolizLexem(value(op1).int_value - value(op2).int_value));
			break;
		}
		case MULT:
		{
			op2 = stack777.pop();
			op1 = stack777.pop();
			stack777.push(PolizLexem(value(op1).int_value * value(op2).int_value));
			break;
		}
		case DIV:
		{
			op2 = stack777.pop();
			op1 = stack777.pop();
			if (value(op2).int_value != 0)
				stack777.push(PolizLexem(value(op1).int_value / value(op2).int_value));
			else
				throw 1; // /0
			break;
		}
		case MOD:
		{
			op2 = stack777.pop();
			op1 = stack777.pop();
			if (value(op2).int_value != 0)
				stack777.push(PolizLexem(value(op1).int_value % value(op2).int_value));
			else
				throw 1; // /0
			break;
		}
		case AND:
		{
			op2 = stack777.pop();
			op1 = stack777.pop();
			stack777.push(PolizLexem(value(op1).bool_value && value(op2).bool_value));
			break;
		}
		case OR:
		{
			op2 = stack777.pop();
			op1 = stack777.pop();
			stack777.push(PolizLexem(value(op1).bool_value || value(op2).bool_value));
			break;
		}
		case NOT:
		{
			op1 = stack777.pop();
			stack777.push(PolizLexem(!value(op1).bool_value));
			break;
		}
		case EQUAL:
		{
			op2 = stack777.pop();
			op1 = stack777.pop();
			switch (gettype(op1))
			{
			case INTTYPE: stack777.push(PolizLexem(value(op1).int_value == value(op2).int_value)); break;
			case STRINGTYPE: stack777.push(PolizLexem(value(op1).string_value == value(op2).string_value)); break;
			case BOOLEANTYPE: stack777.push(PolizLexem(value(op1).bool_value == value(op2).bool_value)); break;
			}
			break;
		}
		case NOTEQUAL:
		{
			op2 = stack777.pop();
			op1 = stack777.pop();
			switch (gettype(op1))
			{
			case INTTYPE: stack777.push(PolizLexem(value(op1).int_value != value(op2).int_value)); break;
			case STRINGTYPE: stack777.push(PolizLexem(value(op1).string_value != value(op2).string_value)); break;
			case BOOLEANTYPE: stack777.push(PolizLexem(value(op1).bool_value != value(op2).bool_value)); break;
			}
			break;
		}
		case UMINUS:
		{
			op1 = stack777.pop();
			stack777.push(PolizLexem(-value(op1).int_value));
			break;
		}
		case READ:
		{
			op1 = stack777.pop();
			switch (gettype(op1))
			{
			case INTTYPE: cin >> idmap[op1.getname()].int_value; idmap[op1.getname()].is_modified = true; break;
			case STRINGTYPE: cin >> idmap[op1.getname()].string_value; idmap[op1.getname()].is_modified = true; break;
			case BOOLEANTYPE: cin >> idmap[op1.getname()].bool_value; idmap[op1.getname()].is_modified = true; break;
			}
			break;
		}
		case WRITE:
		{
			OkoloStack stack888;
			do
			{
				op1 = stack777.pop();
				stack888.push(op1);
				op1 = stack777.pop();
			} 
			while (op1.getid() == FOX);
			stack777.push(op1);

			do
			{
				op1 = stack888.pop();
				switch (gettype(op1))
				{
				case INTTYPE: cout << value(op1).int_value; break;
				case STRINGTYPE: cout << value(op1).string_value; break;
				case BOOLEANTYPE: cout << value(op1).bool_value; break;
				}
			} 
			while(!stack888.is_empty());
			break;
		}
		case TDOCK:
		{
			stack777.clear();
			break;
		}
		case GOTO:
		{
			size_t adr = it->getadr();
			if (adr > it->getpos())
			{
				do
					it++;
				while (it->getpos() != adr);
			}
			else
			{
				do
					it--;
				while (it->getpos() != adr);
			}
			it--;
			break;
		}
		case BREAK:
		{
			do
				++it;
			while (it->getid() != ENDOFCYCLE);
			break;
		}
		case IFNOT:
		{
			if (stack777.is_empty())
			{
				size_t adr = it->getadr();
				do
					it--;
				while (it->getpos() != adr);
				it--;
			}
			else
			{
				op2 = stack777.pop();
				op1 = stack777.pop();
				if (!value(op1).bool_value)
				{
					if (op2.getadr() > op2.getpos())
					{
						do
							it++;
						while (it->getpos() != op2.getadr());
						it--;
					}
					else
					{
						do
							it--;
						while (it->getpos() != op2.getadr());
						it--;
					}

				}
			}
			break;
		}
		case IFYES:
		{
			if (stack777.is_empty())
			{
				size_t adr = it->getadr();
				do
					it--;
				while (it->getpos() != adr);
				it--;
			}
			else
			{
				op2 = stack777.pop();
				op1 = stack777.pop();
				if (value(op1).bool_value)
				{
					if (op2.getadr() > op2.getpos())
					{
						do
							it++;
						while (it->getpos() != op2.getadr());
						it--;
					}
					else
					{
						do
							it--;
						while (it->getpos() != op2.getadr());
						it--;
					}

				}
			}
			break;
		}
		case PRISVOIT:
		{
			op2 = stack777.pop();
			op1 = stack777.pop();
			switch (gettype(op1))
			{
			case INTTYPE:
			{
				idmap[op1.getname()].int_value = value(op2).int_value;
				stack777.push(PolizLexem(value(op2).int_value));
				idmap[op1.getname()].is_modified = true;
				break;
			}
			case STRINGTYPE:
			{
				idmap[op1.getname()].string_value = value(op2).string_value;
				stack777.push(PolizLexem(value(op2).string_value));
				idmap[op1.getname()].is_modified = true;
				break;
			}
			case BOOLEANTYPE:
			{
				idmap[op1.getname()].bool_value = value(op2).bool_value;
				stack777.push(PolizLexem(value(op2).bool_value));
				idmap[op1.getname()].is_modified = true;
				break;
			}
			}
			break;
		}
		}
	}
}