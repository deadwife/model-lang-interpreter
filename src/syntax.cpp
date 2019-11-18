#include <string>
#include <vector>
#include <fstream>
#include <map>
#include "prototypes.h"
#include "classes.h"
using namespace std;

polizlexvector Syntezator3000::defense_matrix_activated(varmap & MAP)
{
	if (!expect_program())
		throw 1; //Œÿ»¡ ¿ ¬ —»Õ“¿ —»—≈
	MAP = idmap;
	return strahovoy_polis;
}

bool Syntezator3000::expect(lextype expected_lexem, eflag flag)
{
	if (not_eov())
	{
		if (it->getid() == expected_lexem)
		{
			it++;
			return true;
		}
	}
	if (flag == SOFT)
		return false;
	throw 1; //Œ∆»ƒ¿≈ÃŒ√Œ Õ¿ Ã≈—“≈ Õ≈ Õ¿ÿÀŒ—‹
}

bool Syntezator3000::expect(lextype expected_lexem, eflag flag, Lexem & op)
{
	if (not_eov())
	{
		if (it->getid() == expected_lexem)
		{
			op = *it;
			it++;
			return true;
		}
	}
	if (flag == SOFT)
		return false;
	throw 1; //Œ∆»ƒ¿≈ÃŒ√Œ Õ¿ Ã≈—“≈ Õ≈ Õ¿ÿÀŒ—‹
}

bool Syntezator3000::expect_program()
{
	try
	{
		expect(PROGRAM, HARD);
		expect(LFBR, HARD);
		expect_definitions();
		expect_operators();
		expect(RFBR, HARD);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Syntezator3000::expect_type(lextype & type)
{
	if(not_eov())
		if (it->getid() == INTTYPE || it->getid() == STRINGTYPE || it->getid() == BOOLEANTYPE)
		{
			type = it->getid();
			it++;
			return true;
		}
	return false;
}

bool Syntezator3000::expect_const(Lexem & lex, bool & minusflag, lextype type)
{
	if (not_eov())
	{
		switch (it->getid())
		{
		case CONSTANT: {if (type == INTTYPE) { lex = *it; it++; } else throw 1; break; } //Œÿ»¡ ¿: œ–»—¬Œ≈Õ»≈ »Õ“¿ Õ≈ »Õ“”
		case TRUE: case FALSE: {if (type == BOOLEANTYPE) { lex = *it; it++; } else throw 1; break; } //Œÿ»¡ ¿: œ–»—¬Œ≈Õ»≈ ¡”À»Õ¿ Õ≈ ¡”À»Õ”
		case STRING: {if (type == STRINGTYPE) { lex = *it; it++; } else throw 1; break; } //Œÿ»¡ ¿: œ–»—¬Œ≈Õ»≈ —“–»Õ√¿ Õ≈ —“–»Õ√”
		case MINUS: {if (type == INTTYPE) { it++; minusflag = true;  lex = *it; expect(CONSTANT, HARD); } else throw 1; break; }/////
		case PLUS: {if (type == INTTYPE) { it++; lex = *it; expect(CONSTANT, HARD); } else throw 1; break; }/////////////////////////
		default: {return false; }
		}
		return true;
	}
	return false;
}

bool Syntezator3000::not_eov()
{
	if (it == eov)
		return false;
	return true;
}

bool Syntezator3000::expect_identificator(string & var)
{
	if (not_eov())
	{
		var = it->getname();
		expect(IDENTIFICATOR, HARD);
		return true;
	}
	return false;
}

bool Syntezator3000::expect_definitions()
{
	lextype type;
	Lexem lex;
	while (expect_type(type))
	{
		do
		{
			bool minusflag = false;
			string var;
			if (!expect_identificator(var))
				throw 1;
			VarValue val(type);
			if(idmap.find(var) == idmap.end())
				idmap.emplace(var, val);
			else throw 1; //œ≈–≈Ã≈ÕÕ¿ﬂ ”∆≈ Œ¡⁄ﬂ¬À≈Õ¿
			if (expect(PRISVOIT, SOFT))
				if (!expect_const(lex, minusflag, type))
					throw 1; //Œ∆»ƒ¿À¿—‹  ŒÕ—“¿Õ“¿, ÕŒ ”¬€
				else
					setinfo(lex, idmap[var], minusflag);
		}
		while (expect(FOX, SOFT));
		expect(TDOCK, HARD);
	}
	return true;
}

bool Syntezator3000::expect_operators()
{
	while (expect_operator())
		;
	return true;
} 

bool Syntezator3000::expect_else()
{
	if (expect(ELSE, SOFT))
	{
		if (expect(LFBR, SOFT))
		{
			expect_operators();
			expect(RFBR, HARD);
		}
		else
			if (!expect_operator())
				throw 1; //›À— ◊“Œ?
		return true;
	}
	return false;
}

bool Syntezator3000::expect_if()
{
	expect(IF, HARD);
	expect(LBR, HARD);
	size_t size2 = strahovoy_polis.size();
	if (!expect_expression())
		throw 1; //Œÿ»¡ ¿ ¬ »‘≈
	if (!(expresstype == BOOLEANTYPE) || prisvoitflag)
		throw 1; //BOOL EXPR EXPECTED
	size_t size = strahovoy_polis.size();
	strahovoy_polis.push_back(PolizLexem(LABEL, strahovoy_polis.size()));
	strahovoy_polis.push_back(PolizLexem(IFNOT, strahovoy_polis.size(), size2));
	expect(RBR, HARD);
	if (expect(LFBR, SOFT))
	{
		expect_operators();
		expect(RFBR, HARD);
	}
	else if (!expect_operator())
		expect(TDOCK, HARD);
	size_t size1 = strahovoy_polis.size();
	strahovoy_polis.push_back(PolizLexem(GOTO, strahovoy_polis.size()));
	strahovoy_polis[size].setlabel(strahovoy_polis.size());
	while (expect_else());
	strahovoy_polis[size1].setlabel(strahovoy_polis.size());
	return true;
}

bool Syntezator3000::expect_while()
{
	expect(WHILE, HARD);
	expect(LBR, HARD);
	size_t size1 = strahovoy_polis.size();
	if (!expect_expression())
		throw 1;// Œÿ»¡ ¿ ¬ ¬¿…À≈
	if (!(expresstype == BOOLEANTYPE) || prisvoitflag)
		throw 1; //BOOL EXPR EXPECTED
	size_t size = strahovoy_polis.size();
	strahovoy_polis.push_back(PolizLexem(LABEL, strahovoy_polis.size()));
	strahovoy_polis.push_back(PolizLexem(IFNOT, strahovoy_polis.size(), size1));
	expect(RBR, HARD);
	cycleflag = true;		//START
	if (expect(LFBR, SOFT))
	{
		expect_operators();
		expect(RFBR, HARD);
	}
	else if (!expect_operator())
		expect(TDOCK, HARD);
	cycleflag = false;		//FINISH
	strahovoy_polis.push_back(PolizLexem(GOTO, strahovoy_polis.size(), size+1));
	strahovoy_polis.push_back(PolizLexem(ENDOFCYCLE, strahovoy_polis.size()));
	strahovoy_polis[size].setlabel(strahovoy_polis.size());
	return true;
}

bool Syntezator3000::expect_do_while()
{
	size_t size = strahovoy_polis.size();
	expect(DO, HARD);
	cycleflag = true;		//START
	if (expect(LFBR, SOFT))
	{
		expect_operators();
		expect(RFBR, HARD);
	}
	else if (!expect_operator())
		expect(TDOCK, HARD);
	cycleflag = false;		//FINISH
	expect(WHILE, HARD);
	expect(LBR, HARD);
	size_t size1 = strahovoy_polis.size();
	if (!expect_expression())
		throw 1;// Œÿ»¡ ¿ ¬ ¬¿…À≈
	if (!(expresstype == BOOLEANTYPE) || prisvoitflag)
		throw 1; //BOOL EXPR EXPECTED
	expect(RBR, HARD);
	expect(TDOCK, HARD);
	strahovoy_polis.push_back(PolizLexem(LABEL, strahovoy_polis.size(), size));
	strahovoy_polis.push_back(PolizLexem(IFYES, strahovoy_polis.size(), size1));
	strahovoy_polis.push_back(PolizLexem(ENDOFCYCLE, strahovoy_polis.size()));
	return true;
}

bool Syntezator3000::expect_read()
{
	DONTPUSHNEW = true;
	Lexem current_lexem;
	expect(READ, HARD);
	expect(LBR, HARD);
	if (not_eov())
		if (idmap[it->getname()].type == BOOLEANTYPE)
			throw 1; //BOOLEAN ¬ –»ƒ≈
	expect(IDENTIFICATOR, HARD, current_lexem);
	expect(RBR, HARD);
	expect(TDOCK, HARD);
	strahovoy_polis.push_back(PolizLexem(current_lexem, strahovoy_polis.size()));
	strahovoy_polis.push_back(PolizLexem(READ, strahovoy_polis.size()));
	DONTPUSHNEW = false;
	return true;
}

bool Syntezator3000::expect_write()
{
	DONTPUSHNEW = true;
	expect(WRITE, HARD);
	expect(LBR, HARD);
	if (!expect_expression())
		throw 1; // Œÿ»¡ ¿ ¬ –¿…“≈
	if (prisvoitflag)
		throw 1; // WRITE(a=b) ETO CHTO?
	while (expect(FOX, SOFT))
	{
		strahovoy_polis.push_back(PolizLexem(FOX, strahovoy_polis.size()));
		if (!expect_expression())
			throw 1; // Œÿ»¡ ¿ ¬ –¿…“≈
		if (prisvoitflag)
			throw 1; // WRITE(a=b) ETO CHTO?
	}
	strahovoy_polis.push_back(PolizLexem(WRITE, strahovoy_polis.size()));
	expect(RBR, HARD);
	expect(TDOCK, HARD);
	DONTPUSHNEW = false;
	return true;
}

bool Syntezator3000::expect_operator()
{
	if (not_eov())
		switch (it->getid())
		{
		case IF: 
		{
			if (expect_if())
			{
				strahovoy_polis.push_back(PolizLexem(TDOCK, strahovoy_polis.size()));
				
				return true;
			}
		}
		case WHILE: 
		{
			if (expect_while())
			{
				strahovoy_polis.push_back(PolizLexem(TDOCK, strahovoy_polis.size()));
				
				return true;
			}
		}
		case DO: 
		{
			if (expect_do_while())
			{
				strahovoy_polis.push_back(PolizLexem(TDOCK, strahovoy_polis.size()));
				
				return true;
			}
		}
		case READ: 
		{
			if (expect_read())
			{
				strahovoy_polis.push_back(PolizLexem(TDOCK, strahovoy_polis.size()));
				
				return true;
			}
		}
		case WRITE:
		{
			if (expect_write())
			{
				strahovoy_polis.push_back(PolizLexem(TDOCK, strahovoy_polis.size()));
				
				return true;
			}
		}
		case BREAK: 
		{
			if (cycleflag)
			{
				strahovoy_polis.push_back(PolizLexem(BREAK, strahovoy_polis.size()));
				
				it++;
				expect(TDOCK, HARD);
				return true;
			}
			else
				throw 1; //BREAK NE V CIKLE
		}
		default:
		{
			if (expect_expression())
			{
				strahovoy_polis.push_back(PolizLexem(TDOCK, strahovoy_polis.size()));
				
				expect(TDOCK, HARD);
				return true;
			}
		}
		}
	return false;
}



bool Syntezator3000::expect_expression()
{
	bool expressionflag = true;
	prisvoitflag = false;
	int prisvoit_counter = 0;
	if (!expect_expr())
		return false;
 	switch (expresstype)
	{
	case INTTYPE:
	{
		while (expect(PRISVOIT, SOFT))
		{
			prisvoit_counter++;
			prisvoitflag = true;
			if (idflag && expressionflag)
			{
				if (!expect_expr())
					return false;
				expressionflag = (expresstype == INTTYPE);
			}
			else
				return false;
		}
		for (int i = 0; i < prisvoit_counter; ++i)
			strahovoy_polis.push_back(PolizLexem(PRISVOIT, strahovoy_polis.size())); //DEBUGPOLIZ
		return expressionflag;
	}
	case STRINGTYPE:
	{
		while (expect(PRISVOIT, SOFT))
		{
			prisvoit_counter++;
			prisvoitflag = true;
			if (idflag && expressionflag)
			{
				if (!expect_expr())
					return false;
				expressionflag = (expresstype == STRINGTYPE);
			}
			else
				return false;
		}
		for (int i = 0; i < prisvoit_counter; ++i)
			strahovoy_polis.push_back(PolizLexem(PRISVOIT, strahovoy_polis.size())); //DEBUGPOLIZ
		return expressionflag;
	}
	case BOOLEANTYPE:
	{
		while (expect(PRISVOIT, SOFT))
		{
			prisvoit_counter++;
			prisvoitflag = true;
			if (idflag && expressionflag)
			{
				if (!expect_expr())
					return false;
				expressionflag = (expresstype == BOOLEANTYPE);
			}
			else
				return false;
		}
		for (int i = 0; i < prisvoit_counter; ++i)
			strahovoy_polis.push_back(PolizLexem(PRISVOIT, strahovoy_polis.size())); //DEBUGPOLIZ
		return expressionflag;
	}
	}
	return false;
} 

bool Syntezator3000::expect_expr()
{
	if (!expect_sum())
		return false;
	switch (expresstype)
	{
	case INTTYPE: case STRINGTYPE:
		return true;
	case BOOLEANTYPE:
	{
		bool exprflag = true;
		while (expect(OR, SOFT))
		{
			idflag = false;
			if (exprflag)
			{
				if (!expect_sum())
					return false;
				exprflag = (expresstype == BOOLEANTYPE);
				strahovoy_polis.push_back(PolizLexem(OR, strahovoy_polis.size())); //DEBUGPOLIZ
			}
			else
				return false;
		}
		return exprflag;
	}
	}
	return false;
}

bool Syntezator3000::expect_sum()
{
	if (!expect_subsum())
		return false;
	switch (expresstype)
	{
	case INTTYPE: case STRINGTYPE:
		return true;
	case BOOLEANTYPE:
	{
		bool sumflag = true;
		while (expect(AND, SOFT))
		{
			idflag = false;
			if (sumflag)
			{
				if (!expect_subsum())
					return false;
				sumflag = (expresstype == BOOLEANTYPE);
				strahovoy_polis.push_back(PolizLexem(AND, strahovoy_polis.size())); //DEBUGPOLIZ
			}
			else
				return false;
		}
		return sumflag;
	}
	}
	return false;
}

bool Syntezator3000::expect_subsum()
{
	if (!expect_logic())
		return false;
	switch (expresstype)
	{
	case INTTYPE:
	{
		Lexem current_op;
		if (expect_logic_int_op(current_op))
		{
			idflag = false;
			if (!expect_logic())
				return false;
			strahovoy_polis.push_back(PolizLexem(current_op, strahovoy_polis.size())); //DEBUGPOLIZ
			if (expresstype == INTTYPE)
			{
				expresstype = BOOLEANTYPE;
				return true;
			}
			return false;
		}
		return true;
	}
	case STRINGTYPE:
	{
		Lexem current_op;
		if (expect_logic_string_op(current_op))
		{
			idflag = false;
			if (!expect_logic())
				return false;
			strahovoy_polis.push_back(PolizLexem(current_op, strahovoy_polis.size())); //DEBUGPOLIZ
			if (expresstype == STRINGTYPE)
			{
				expresstype = BOOLEANTYPE;
				return true;
			}
			return false;
		}
		return true;
	}
	case BOOLEANTYPE: 
		return true;
	}
	return false;
}

bool Syntezator3000::expect_logic_int_op(Lexem & current_op)
{
	if (not_eov())
	{
		switch (it->getid())
		{
		case MORE: case LESS: case LESSQ: case MOREQ: case NOTEQUAL: case EQUAL: current_op = *it;  it++;  return true;
		}
	}
	return false;
}

bool Syntezator3000::expect_logic_string_op(Lexem & current_op)
{
	if (not_eov())
	{
		switch (it->getid())
		{
		case MORE: case LESS: case NOTEQUAL: case EQUAL: current_op = *it; it++; return true;
		}
	}
	return false;
}

bool Syntezator3000::expect_logic()
{
	if (!expect_area())
		return false;
	switch (expresstype)
	{
	case INTTYPE:
	{
		bool logicflag = true;
		Lexem current_op;
		while (expect(PLUS, SOFT, current_op) || expect(MINUS, SOFT, current_op))
		{
			idflag = false;
			if (logicflag)
			{
				if (!expect_area())
					return false;
				logicflag = (expresstype == INTTYPE);
				strahovoy_polis.push_back(PolizLexem(current_op, strahovoy_polis.size())); //DEBUGPOLIZ
			}
			else
				return false;
		}
		return logicflag;
	}
	case STRINGTYPE:
	{
		bool logicflag = true;
		while (expect(PLUS, SOFT))
		{
			idflag = false;
			if (logicflag)
			{
				if (!expect_area())
					return false;
				logicflag = (expresstype == STRINGTYPE);
				strahovoy_polis.push_back(PolizLexem(PLUS, strahovoy_polis.size())); //DEBUGPOLIZ
			}
			else
				return false;
		}
		return logicflag;
	}
	case BOOLEANTYPE:
		return true;
	}
	return false;
}

bool Syntezator3000::expect_area()
{
	if (!expect_mult())
		return false;
	switch(expresstype)
	{
	case INTTYPE:
	{
		Lexem current_op;
		bool areaflag = true;
		while (expect(MULT, SOFT, current_op) || expect(DIV, SOFT, current_op) || expect(MOD, SOFT, current_op))
		{
			idflag = false;
			if (areaflag)
			{
				if (!expect_mult())
					return false;
				areaflag = (expresstype == INTTYPE);
				strahovoy_polis.push_back(PolizLexem(current_op, strahovoy_polis.size())); //DEBUGPOLIZ
			}
			else
				return false;
		}
		return areaflag;
	}
	case STRINGTYPE: case BOOLEANTYPE:
		return true;
	}
	return false;
}

bool Syntezator3000::expect_mult()
{
	if (expect(NOT, SOFT))
	{
		idflag = false;
		if (!expect_atom())
			return false;
		strahovoy_polis.push_back(PolizLexem(NOT, strahovoy_polis.size())); //DEBUGPOLIZ
		return (expresstype == BOOLEANTYPE);
	}
	if (!expect_atom())
		return false;
	return true;
}
	
bool Syntezator3000::expect_atom()
{
	if (not_eov())
		switch (it->getid())
		{
		case CONSTANT:
		{
			strahovoy_polis.push_back(PolizLexem(*it, strahovoy_polis.size())); //DEBUGPOLIZ
			expresstype = INTTYPE;
			idflag = false;
			it++;
			return true;
		}
		case IDENTIFICATOR:
		{
			if (idmap.find(it->getname()) != idmap.end())
			{
				expresstype = idmap[it->getname()].type;
			}
			else
				throw 1; // œ≈–≈Ã≈ÕÕ¿ﬂ Õ≈ Œ¡⁄ﬂ¬À≈Õ¿ OR ›“Œ Õ≈ œ≈–≈Ã≈ÕÕ¿ﬂ
			strahovoy_polis.push_back(PolizLexem(*it, strahovoy_polis.size())); //DEBUGPOLIZ
			idflag = true;
			it++;
			return true;
		}
		case TRUE:
		{
			strahovoy_polis.push_back(PolizLexem(TRUE, strahovoy_polis.size())); //DEBUGPOLIZ
			expresstype = BOOLEANTYPE;
			idflag = false;
			it++;
			return true;
		}
		case FALSE:
		{
			strahovoy_polis.push_back(PolizLexem(FALSE, strahovoy_polis.size())); //DEBUGPOLIZ
			expresstype = BOOLEANTYPE;
			idflag = false;
			it++;
			return true;
		}
		case STRING:
		{
			strahovoy_polis.push_back(PolizLexem(*it, strahovoy_polis.size())); //DEBUGPOLIZ
			expresstype = STRINGTYPE;
			idflag = false;
			it++;
			return true;
		}
		case PLUS:
		{
			it++;
			idflag = false;
			if (expect(LBR, SOFT))
			{
				if (!expect_expr())
					return false;
				expect(RBR, HARD);
			}
			else
			{
				Lexem current_op;
				expresstype = INTTYPE;
				if (!expect(CONSTANT, SOFT, current_op))
				{
					if (not_eov())
					{
						if (idmap.find(it->getname()) != idmap.end())
						{
							if (idmap[it->getname()].type == INTTYPE)
								expect(IDENTIFICATOR, HARD, current_op);
						}
						else
							throw 1; // œ≈–≈Ã≈ÕÕ¿ﬂ Õ≈ Œ¡⁄ﬂ¬À≈Õ¿ OR ›“Œ Õ≈ œ≈–≈Ã≈ÕÕ¿ﬂ
					}
					else
						throw 1; // EOV
				}
				strahovoy_polis.push_back(PolizLexem(current_op, strahovoy_polis.size()));
			}
			return true;
		}
		case MINUS:
		{
			it++;
			idflag = false;
			if (expect(LBR, SOFT))
			{
				if (!expect_expr())
					return false;
				expect(RBR, HARD);
			}
			else
			{
				Lexem current_op;
				expresstype = INTTYPE;
				if (!expect(CONSTANT, SOFT, current_op))
				{
					if (not_eov())
					{
						if (idmap.find(it->getname()) != idmap.end())
						{
							if (idmap[it->getname()].type == INTTYPE)
								expect(IDENTIFICATOR, HARD, current_op);
						}
						else
							throw 1; // œ≈–≈Ã≈ÕÕ¿ﬂ Õ≈ Œ¡⁄ﬂ¬À≈Õ¿ OR ›“Œ Õ≈ œ≈–≈Ã≈ÕÕ¿ﬂ
					}
					else
						throw 1; // EOV
				}
				strahovoy_polis.push_back(PolizLexem(current_op, strahovoy_polis.size()));
				strahovoy_polis.push_back(PolizLexem(UMINUS, strahovoy_polis.size())); //DEBUGPOLIZ
			}
			return true;
		}
		case LBR:
		{
			it++;
			if (!expect_expr())
				return false;
			expect(RBR, HARD);
			return true;
		}
		}
	return false;
}


