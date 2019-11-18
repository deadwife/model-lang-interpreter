#include <string>
#include <vector>
#include <fstream>
#include <set>
#include <cassert>
#include "prototypes.h"
#include "classes.h"
using namespace std;

lexvector LexemScan::scanner2000(const char * str)
{
	assert(str != nullptr);
	state = &LexemScan::stateSTART;
	for (; *str != '\0'; str++)
	{
		if (!(this->*state)(*str, str))
			throw 1;
	}
	return this->newvector;
}

bool LexemScan::stateSTART(char c, const char * &str)
{
	tstr = c;
	switch (c)
	{
		// state остается равен stateSTART
	case '{': {	Lexem lex(LFBR, tstr); newvector.push_back(lex); break;  }
	case '}': {	Lexem lex(RFBR, tstr); newvector.push_back(lex); break;  }
	case '+': {	Lexem lex(PLUS, tstr); newvector.push_back(lex); break;  }
	case '-': {	Lexem lex(MINUS, tstr); newvector.push_back(lex); break; }
	case '*': {	Lexem lex(MULT, tstr); newvector.push_back(lex); break;  }
	case '%': {	Lexem lex(MOD, tstr); newvector.push_back(lex); break;   }
	case '(': {	Lexem lex(LBR, tstr); newvector.push_back(lex); break;   }
	case ')': {	Lexem lex(RBR, tstr); newvector.push_back(lex); break;   }
	case ';': {	Lexem lex(TDOCK, tstr); newvector.push_back(lex); break; }
	case ',': {	Lexem lex(FOX, tstr); newvector.push_back(lex); break;   }
	case ' ': case '\t': case '\n': { break; }
			  // state меняется
	case '/': { state = &LexemScan::stateDIVER; break; }
	case '>': { state = &LexemScan::stateDMORE; break; }
	case '<': { state = &LexemScan::stateDLESS; break; }
	case '=': { state = &LexemScan::stateDEQUAL; break; }
	case '!': { state = &LexemScan::stateDNOT; break; }
	case '"': { state = &LexemScan::stateSTRID; break; }
	default:
	{
		if (is_NNum(c))
		{
			state = &LexemScan::stateCONST;
			--str;
		}
		else if (is_LLetter(c))
		{
			state = &LexemScan::stateIDENT;
			--str;
		}
		else //ошибка - неопознанный символ
			return false;
	}
	}
	tstr = "";
	return true;
}

bool LexemScan::stateDMORE(char c, const char * &str)
{
	Lexem lex(MOREQ, ">=");
	if (c != '=')
	{
		lex.setid(MORE, ">");
		--str;
	}
	newvector.push_back(lex);
	state = &LexemScan::stateSTART;
	return true;
}

bool LexemScan::stateDLESS(char c, const char * &str)
{
	Lexem lex(LESSQ, "<=");
	if (c != '=')
	{
		lex.setid(LESS, "<");
		--str;
	}
	newvector.push_back(lex);
	state = &LexemScan::stateSTART;
	return true;
}

bool LexemScan::stateDEQUAL(char c, const char * &str)
{
	Lexem lex(EQUAL, "==");
	if (c != '=')
	{
		lex.setid(PRISVOIT, "=");
		--str;
	}
	newvector.push_back(lex);
	state = &LexemScan::stateSTART;
	return true;
}

bool LexemScan::stateDNOT(char c, const char * &str)
{
	Lexem lex(NOTEQUAL, "!=");
	if (c != '=')
		return false;
	newvector.push_back(lex);
	state = &LexemScan::stateSTART;
	return true;
}

bool LexemScan::stateDIVER(char c, const char * &str)
{
	if (c == '*')
		state = &LexemScan::stateCOMMENT;
	else
	{
		Lexem lex(DIV, "/");
		newvector.push_back(lex);
		--str;
		state = &LexemScan::stateSTART;
	}
	return true;
}

bool LexemScan::stateCOMMENT(char c, const char * &str)
{
	if (c == '*')
		state = &LexemScan::stateENDCOMMENT;
	//else state остается stateCOMMENT
	return true;
}

bool LexemScan::stateENDCOMMENT(char c, const char * &str)
{
	if (c == '/')
		state = &LexemScan::stateSTART;
	else
		state = &LexemScan::stateCOMMENT;
	return true;
}

bool LexemScan::stateCONST(char c, const char * &str)
{
	//state остается stateCONST
	if (is_NNum(c))
	{
		tstr += c;
		tsum = tsum * 10 + (c - '0');
	}
	else
	{
		Lexem lex(CONSTANT, tstr, tsum);
		tstr = "";
		newvector.push_back(lex);
		tsum = 0;
		--str;
		state = &LexemScan::stateSTART;
	}
	return true;
}

bool LexemScan::stateSTRID(char c, const char * &str)
{
	if (c == '"')
	{
		Lexem lex(STRING, tstr);
		newvector.push_back(lex);
		state = &LexemScan::stateSTART;
		tstr = "";
	}
	else if (c == '\\')
		state = &LexemScan::stateESTRID;
	//state остается stateSTRID
	else
		tstr += c;
	return true;
}

bool LexemScan::stateESTRID(char c, const char * &str)
{
	if (c == 'n')
		tstr += '\n';
	else if (c == '\\')
		tstr += '\\';
	else if (c == '"')
		tstr += '"';
	else
		return false;
	state = &LexemScan::stateSTRID;
	return true;
}

bool LexemScan::stateIDENT(char c, const char * &str)
{
	if (is_LLetter(c) || is_NNum(c))
		tstr += c;
	else
	{
		Lexem lex(IDENTIFICATOR, tstr);
		LexemScan::SYSCHECK(lex);
		newvector.push_back(lex);
		tstr = "";
		--str;
		state = &LexemScan::stateSTART;
	}
	return true;
}

void LexemScan::SYSCHECK(Lexem & lex)
{
	string t = lex.getname();
	if (t == "program")
		lex.setid(PROGRAM);
	else if (t == "int")
		lex.setid(INTTYPE);
	else if (t == "string")
		lex.setid(STRINGTYPE);
	else if (t == "if")
		lex.setid(IF);
	else if (t == "else")
		lex.setid(ELSE);
	else if (t == "while")
		lex.setid(WHILE);
	else if (t == "read")
		lex.setid(READ);
	else if (t == "write")
		lex.setid(WRITE);
	else if (t == "not")
		lex.setid(NOT);
	else if (t == "and")
		lex.setid(AND);
	else if (t == "or")
		lex.setid(OR);
	else if (t == "do")
		lex.setid(DO);
	else if (t == "break")
		lex.setid(BREAK);
	else if (t == "boolean")
		lex.setid(BOOLEANTYPE);
	else if (t == "true")
		lex.setid(TRUE);
	else if (t == "false")
		lex.setid(FALSE);
}