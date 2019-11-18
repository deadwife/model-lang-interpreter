#pragma once
#include "stdafx.h"
#include <vector>
#include <map>
#include <string>
using namespace std;

enum lextype {
	CONSTANT,		//
	IDENTIFICATOR,	//
	PLUS,			//
	MINUS,			//
	MULT,			//
	DIV,			//
	LBR,		
	RBR,			
	MORE,			//
	LESS,			//
	MOREQ,			//
	LESSQ,			//
	EQUAL,			//
	NOTEQUAL,		//
	PRISVOIT,		//
	MOD,			//
	COMMENT,		
	TDOCK,			//
	FOX,			//
	LFBR, 			
	RFBR,			
	STRING,			//
	PROGRAM,
	INTTYPE,
	STRINGTYPE,
	IF,
	ELSE,
	WHILE,
	READ,			//
	WRITE,			//
	NOT,			//
	AND,			//
	OR,				//
	DO,
	BREAK,			//
	BOOLEANTYPE,
	TRUE,			//
	FALSE,			//
	TYPE,
	UMINUS,			//
	EMPTY,
	NEW,			//
	IFNOT,			//
	LABEL,			//
	GOTO,			//
	PRIKOL,			//
	ENDOFCYCLE,
	IFYES
};
enum eflag {
	HARD,
	SOFT
};

const char* const lextext[] = {
	"CONST",
	"IDENT", //1
	"PLUS", //2
	"MINUS", //3
	"MULT", //4
	"DIV", //5
	"LBR", //6
	"RBR", //7
	"MORE", //10
	"LESS", //11
	"MOREQ", //12
	"LESSQ", //13
	"EQUAL", //14
	"NEQUAL", //15
	"RAVNO", //16
	"MOD", //17
	"COMMENT", //18
	"TDOCK", //19
	"FOX", //20
	"LFBR", //27
	"RFBR", //28
	"STR", //29
	"PROGR",
	"T_INT",
	"T_STR",
	"IF",
	"ELSE",
	"WHILE",
	"READ",
	"WRITE",
	"NOT",
	"AND",
	"OR",
	"DO",
	"BREAK",
	"T_BOOL",
	"TRUE",
	"FALSE",
	"TYPE",
	"UMINUS",
	"EMPTY",
	"NEW",
	"IFNOT",
	"LAB",
	"GOTO",
	"",
	"EOC"
};

const char* const poliztext[] = {
	"CONST",
	"IDENT", //1
	"+", //2
	"-", //3
	"*", //4
	"/", //5
	"LBR", //6
	"RBR", //7
	">", //10
	"<", //11
	">=", //12
	"<=", //13
	"==", //14
	"!=", //15
	"=", //16
	"%", //17
	"COMMENT", //18
	"\n", //19
	",", //20
	"LFBR", //27
	"RFBR", //28
	"STR", //29
	"PROGR",
	"T_INT",
	"T_STR",
	"IF",
	"ELSE",
	"WHILE",
	"READ",
	"WRITE",
	"NOT",
	"AND",
	"OR",
	"DO",
	"BREAK",
	"T_BOOL",
	"TRUE",
	"FALSE",
	"TYPE",
	"U-",
	"EMPTY",
	"\n",
	"IFNOT",
	"LAB",
	"GOTO",
	"",
	"EOC",
	"IFYES"
};

class Lexem;
class PolizLexem;
struct VarValue;

typedef vector<Lexem> lexvector;
typedef vector<PolizLexem> polizlexvector;
typedef map<string, VarValue> varmap;


