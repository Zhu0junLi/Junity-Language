#pragma once
#include"utils.h"
#include"header.h"
#include"module.h"

struct VM;
template<class T>
struct Buffer;

enum TokenType {
	TOKEN_NUM,				//数字
	TOKEN_STR,				//字符串
	TOKEN_ID,				//标识符
	
	TOKEN_VAR,				//var定义
	TOKEN_IF,				//if
	TOKEN_ELSE,				//else
	TOKEN_WHILE,			//while
	TOKEN_FOR,				//for
	TOKEN_FUN,				//fun
	TOKEN_TRUE,				//true
	TOKEN_FALSE,			//false
	TOKEN_CONTINUE,			//continue
	TOKEN_BREAK,			//break
	TOKEN_RETURN,			//return
	TOKEN_NULL,				//null
	TOKEN_GETTER,			//getter
	TOKEN_SETTER,			//setter
	TOKEN_OPERATOR,			//operator
	TOKEN_CLASS,			//class
	TOKEN_SELF,				//self
	TOKEN_IS,				//is
	TOKEN_IN,				//in
	TOKEN_TO,				//to
	TOKEN_STATIC,			//static
	TOKEN_SUPER,			//super
	TOKEN_IMPORT,			//import
	TOKEN_PUBLIC,			//public
	TOKEN_PRIVATE,			//private

	TOKEN_COMMA,			//,
	TOKEN_COLON,			//:
	TOKEN_LEFT_PAREN,		//(
	TOKEN_RIGHT_PAREN,		//)
	TOKEN_LEFT_BRACKET,		//[
	TOKEN_RIGHT_BRACKET,	//]
	TOKEN_LEFT_BRACE,		//{
	TOKEN_RIGHT_BRACE,		//}
	TOKEN_DOT,				//.
	TOKEN_SECTION,			//~(区间选择）

	TOKEN_ADD,				//+
	TOKEN_ADD_ASSIGN,		//+=
	TOKEN_SELF_ADD,			//++
	TOKEN_SUB,				//-
	TOKEN_SELF_SUB,			//--
	TOKEN_SUB_ASSIGN,		//-=
	TOKEN_MUL,				//*
	TOKEN_MUL_ASSIGN,		//*=
	TOKEN_DIV,				// /(除号)
	TOKEN_DIV_ASSIGN,		// /=
	TOKEN_MOD,				//%
	TOKEN_MOD_ASSIGN,		//%=
	TOKEN_POW,				//^ 幂运算
	
	TOKEN_ASSIGN,			//=

	TOKEN_BIT_AND,			//&
	TOKEN_BIT_OR,			//|
	TOKEN_BIT_NOT,			//~(取反）
	TOKEN_BIT_SHIFT_RIGHT,	//>>
	TOKEN_BIT_SHIFT_LEFT,	//<<

	TOKEN_AND,				//&&
	TOKEN_OR,				//||
	TOKEN_NOT,				//!

	TOKEN_EQUAL,			//==
	TOKEN_NOT_EQUAL,		//!=
	TOKEN_GREATE,			//>
	TOKEN_GREATE_EQUAL,		//>=
	TOKEN_LESS,				//<
	TOKEN_LESS_EQUAL,		//<=

	TOKEN_QUESTION,			//?（三目运算符用）

	TOKEN_NONE,				//占位用
	TOKEN_END				//EOF
};


struct Token {
	TokenType type;
	const char* start;
	uint32_t length;
	uint32_t line;
	Value value;
	Token() {};
};

bool isKeyWord(const char* word, size_t length);
TokenType GetWordType(const char* word, size_t length);

struct Parser {
public:
	const char* file;
	const char* sourceCode;
	const char* nextCharPtr;
	char curChar;
	uint32_t line;
	Token curToken;
	Token preToken;
	ObjModule curModule;
	int numberOfBrackets;
	VM* vm;
	Parser(VM* vm, const char* _file, const char* _sourcecode);
	void init(VM* _vm,const char* _file,const char* _sourceCode);
	Token GetNextToken();
	char LookAheadChar();
	char GetNextChar();
	bool MatchNextChar(char ch);
	bool MatchToken(TokenType expected);
	void ConsumeCurToken(TokenType expected,const char* msg);
	void ConsumeNextToken(TokenType expected, const char* msg);
	void ParseId(TokenType type=TOKEN_NONE);
	void ParseString(char);
	void SkipLine();
	void SkipComment();
	void ParseHexNum();
	void ParseDecNum();
	void ParseOctNum();
	void ParseNum();
	void SkipBlanks();
	void ParseUnicodeCodePoint(Buffer<uint8_t> &buf);
};
