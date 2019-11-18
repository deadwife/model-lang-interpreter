#pragma once
#include "stdafx.h"
#include <iostream>
#include "types.h" 

using namespace std;

class Lexem
{
	lextype id;
	string stringvalue;
	int intvalue;

public:
	Lexem(lextype id)
	{
		this->id = id;
		this->stringvalue = poliztext[id];
		this->intvalue = intvalue;
	}
	Lexem(lextype id, string stringvalue, int intvalue = 0)
	{
		this->id = id;
		this->stringvalue = stringvalue;
		this->intvalue = intvalue;
	}
	Lexem()
	{
		this->id = EMPTY;
		this->stringvalue = "";
		this->intvalue = 0;
	}


	void setid(lextype id)
	{
		this->id = id;
	}
	void setid(lextype id, string str)
	{
		this->id = id;
		this->stringvalue = str;
	}
	void setintval(int intvalue)
	{
		this->intvalue = intvalue;
	}
	void setstringval(string str)
	{
		this->stringvalue = str;
	}
	void getinfo()
	{
		cout << lextext[this->id] << " \t\t" << this->stringvalue << "\t\t\t\t" << this->intvalue << endl;
	}
	string getname()
	{
		return this->stringvalue;
	}
	lextype getid()
	{
		return this->id;
	}
	int getval()
	{
		return this->intvalue;
	}
};

class PolizLexem : public Lexem
{
private:
	size_t label_pos;
	size_t pos;
public:
	PolizLexem()
	{
		this->label_pos = 0;
		this->pos = 0;
	}
	PolizLexem(Lexem lex, size_t pos)
	{
		this->setid(lex.getid());
		this->setstringval(lex.getname());
		this->setintval(lex.getval());
		this->pos = pos;
	}
	PolizLexem(lextype type, size_t pos)
	{
		this->setid(type);
		this->pos = pos;
		setstringval(poliztext[type]);
	}
	PolizLexem(lextype type, size_t pos, size_t adress)
	{
		this->setid(type);
		this->label_pos = adress;
		this->pos = pos;
		setstringval(poliztext[type]);
	}
	PolizLexem(bool val)
	{
		if (val)
			setid(TRUE);
		else
			setid(FALSE);
	}
	PolizLexem(int val)
	{
		setid(CONSTANT);
		setintval(val);
	}
	PolizLexem(string val)
	{
		setid(STRING, val);
	}

	size_t getpos()
	{
		return this->pos;
	}
	size_t getadr()
	{
		return this->label_pos;
	}
	void setlabel(size_t adr)
	{
		this->label_pos = adr;
	}
};

struct VarValue
{
	int int_value;
	string string_value;
	bool bool_value;
	lextype type;
	bool is_modified;

	VarValue() = default;
	VarValue(lextype type) { this->type = type; is_modified = false;}
	VarValue(bool val) { this->bool_value = val; is_modified = true;}
	VarValue(int val) { this->int_value = val; is_modified = true;}
	VarValue(string val) { this->string_value = val; is_modified = true;}


	void getinfo()
	{
		if (this->is_modified == true)
			switch(this->type)
			{
			case INTTYPE: cout << this->int_value; break;
			case STRINGTYPE: cout << this->string_value; break;
			case BOOLEANTYPE: cout << this->bool_value; break;
			}
		cout << endl;
	}
};

class LexemScan
{
public:
	LexemScan() = default;
	lexvector scanner2000( const char * str);

private:
	typedef bool (LexemScan::*statetype)(char c,  const char * &str);
	statetype state = nullptr;
	lexvector newvector;
	string tstr = "";
	int tsum = 0;

	bool stateSTART(char c,  const char * &str);
	bool stateDMORE(char c,  const char * &str);
	bool stateDLESS(char c,  const char * &str);
	bool stateDEQUAL(char c,  const char * &str);
	bool stateDNOT(char c,  const char * &str);
	bool stateSTRID(char c,  const char * &str);
	bool stateESTRID(char c,  const char * &str);
	bool stateDIVER(char c,  const char * &str);
	bool stateCOMMENT(char c,  const char * &str);
	bool stateENDCOMMENT(char c,  const char * &str);
	bool stateIDENT(char c,  const char * &str);
	bool stateCONST(char c,  const char * &str);

	void SYSCHECK(Lexem & lex);

	bool is_LLetter(char c)
	{
		if (((('a' <= c) && (c <= 'z') || (('A' <= c) && (c) <= 'Z'))))
			return true;
		return false;
	}

	bool is_NNum(char c)
	{
		if (('0' <= c) && (c <= '9'))
			return true;
		return false;
	}
};

class Syntezator3000
{
public:
	Syntezator3000(lexvector & supervector) 
	{
		eov = supervector.end(); 
		it = supervector.begin();
		cycleflag = false;
		prisvoitflag = false;
	}
	polizlexvector defense_matrix_activated(varmap & MAP);
private:
	varmap idmap;
	string current_var;
	lexvector::iterator eov;
	lexvector::iterator it;
	polizlexvector strahovoy_polis;
	lextype expresstype;
	bool idflag;
	bool prisvoitflag;
	bool cycleflag;
	bool DONTPUSHNEW;

	void setinfo(Lexem lex, VarValue & val, bool minusflag)
	{
		switch (lex.getid())
		{
		case CONSTANT:
		{
			val.is_modified = true;
			if (!minusflag)
				val.int_value = lex.getval();
			else 
				val.int_value = (-1)*lex.getval();
			break;
		}
		case STRING:
		{
			val.is_modified = true;
			val.string_value = lex.getname();
			break;
		}
		case TRUE:
		{
			val.is_modified = true;
			val.bool_value = true;
			break;
		}
		case FALSE:
		{
			val.is_modified = true;
			val.bool_value = false;
			break;
		}
		}
	}
	bool not_eov();
	bool expect(lextype expected_lexem, eflag flag);
	bool expect(lextype expected_lexem, eflag flag, Lexem & op);
	bool expect_program();
	bool expect_definitions();
	bool expect_type(lextype & type);
	bool expect_const(Lexem & lex, bool & minusflag, lextype type);
	bool expect_identificator(string & var);

	bool expect_operators();
	bool expect_operator();
	bool expect_if();
	bool expect_else();
	bool expect_while();
	bool expect_do_while();
	bool expect_read();
	bool expect_write();

	bool expect_expression();
	bool expect_expr();
	bool expect_sum();
	bool expect_subsum();
	bool expect_logic();
	bool expect_logic_int_op(Lexem & current_op);
	bool expect_logic_string_op(Lexem & current_op);
	bool expect_area();
	bool expect_mult();
	bool expect_atom();
};

class OkoloStack
{
private:
	polizlexvector trashbin;
public:
	OkoloStack() {};

	PolizLexem pop()
	{
		if (!trashbin.empty())
		{
			PolizLexem lex = trashbin.back();
			trashbin.pop_back();
			return lex;
		}
		return PolizLexem(EMPTY, 0);
	}
	void push(PolizLexem lex)
	{
		trashbin.push_back(lex);
	}
	void clear()
	{
		if (!trashbin.empty())
			trashbin.clear();
	}
	PolizLexem back()
	{
		return trashbin.back();
	}
	bool is_empty()
	{
		return trashbin.empty();
	}
};

class ThePolice
{
private:
	OkoloStack stack777;
	polizlexvector::iterator it;
	varmap idmap;
public:
	void executioner_tri_topora(polizlexvector & polizvector, varmap IDmap);

	VarValue value(PolizLexem lex)
	{
		switch (lex.getid())
		{
		case CONSTANT: return VarValue(lex.getval());
		case TRUE: return VarValue(true);
		case FALSE: return VarValue(false);
		case STRING: return VarValue(lex.getname());
		case IDENTIFICATOR: if (idmap[lex.getname()].is_modified) return idmap[lex.getname()]; else throw 1; //NO VALUE
		default:
			throw 1;  //DAFUQ?!
		}
	}

	lextype gettype(PolizLexem lex)
	{
		switch (lex.getid())
		{
		case CONSTANT: return INTTYPE;
		case TRUE: case FALSE: return BOOLEANTYPE;
		case STRING: return STRINGTYPE;
		case IDENTIFICATOR: return idmap[lex.getname()].type;
		default:
			throw 1;  //DAFUQ?!
		}
	}

};