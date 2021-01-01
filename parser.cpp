#include "parser.h"
#include "utils.h"
#include "unicodeUtf8.h"
#include <string.h>
#include <ctype.h>

template<class T>
struct Buffer;

struct KeyWordToken {
	const char* keyword;
	size_t length;
	TokenType type;
};

const KeyWordToken keyWordToken[]{
	{"var",3,TOKEN_VAR},
	{"function",8,TOKEN_FUN},
	{"if",2,TOKEN_IF},
	{"is",2,TOKEN_IS},
	{"in",2,TOKEN_IN},
	{"to",2,TOKEN_TO},
	{"else",4,TOKEN_ELSE},
	{"while",5,TOKEN_WHILE},
	{"for",3,TOKEN_FOR},
	{"true",4,TOKEN_TRUE},
	{"false",5,TOKEN_FALSE},
	{"continue",8,TOKEN_CONTINUE},
	{"break",5,TOKEN_BREAK},
	{"return",6,TOKEN_RETURN},
	{"null",4,TOKEN_NULL},
	{"class",5,TOKEN_CLASS},
	{"self",4,TOKEN_SELF},
	{"public",6,TOKEN_PUBLIC},
	{"private",7,TOKEN_PRIVATE},
	{"super",5,TOKEN_SUPER},
	{"import",6,TOKEN_IMPORT},
	{"getter",6,TOKEN_GETTER},
	{"setter",6,TOKEN_SETTER},
	{"operator",8,TOKEN_OPERATOR},
	{NULL,0,TOKEN_NONE}
};

Parser::Parser(VM*vm, const char*_file, const char*_sourcecode)
{
	init(vm, _file, _sourcecode);
}

void Parser::init(VM* _vm, const char* _file, const char* _sourceCode)
{
	file = _file;
	sourceCode = _sourceCode;
	nextCharPtr = sourceCode + 1;
	curChar = *sourceCode;
	line = 1;
	curToken.type = TOKEN_NONE;
	curToken.start = NULL;
	curToken.length = 0;
	preToken = curToken;
	vm = _vm;
}

Token Parser::GetNextToken()
{
	preToken = curToken;
	SkipBlanks();
	curToken.type = TOKEN_END;
	curToken.length = 0;
	curToken.start = nextCharPtr - 1;
	while (curChar != 0) {
		switch (curChar) {
		case ',':
			curToken.type = TOKEN_COMMA;
			break;
		case ':':
			curToken.type = TOKEN_COLON;
			break;
		case '(':
			curToken.type = TOKEN_LEFT_PAREN;
			break;
		case ')':
			curToken.type = TOKEN_RIGHT_BRACE;
			break;
		case '[':
			curToken.type = TOKEN_LEFT_BRACKET;
			break;
		case ']':
			curToken.type = TOKEN_RIGHT_BRACKET;
			break;
		case '{':
			curToken.type = TOKEN_LEFT_BRACE;
			break;
		case '}':
			curToken.type = TOKEN_RIGHT_BRACE;
			break;
		case '.':
			curToken.type = TOKEN_DOT;
			break;
		case '=':
			if (MatchNextChar('='))
			{
				curToken.type = TOKEN_EQUAL;
			}
			else {
				curToken.type = TOKEN_ASSIGN;
			}
			break;
		case '+':
			if (MatchNextChar('='))
			{
				curToken.type = TOKEN_ADD_ASSIGN;
			}
			else if (MatchNextChar('+')) {
				curToken.type = TOKEN_SELF_ADD;
			}
			else
			{
				curToken.type = TOKEN_ADD;
			}
			break;
		case '-':
			if (MatchNextChar('='))
			{
				curToken.type = TOKEN_SUB_ASSIGN;
			}
			else if (MatchNextChar('-')) {
				curToken.type = TOKEN_SELF_SUB;
			}
			else {
				curToken.type = TOKEN_SUB;
			}
			break;
		case '*':
			if (MatchNextChar('='))
			{
				curToken.type = TOKEN_MUL_ASSIGN;
			}
			else {
				curToken.type = TOKEN_MUL;
			}
			break;

		case '/':
			if (MatchNextChar('='))
			{
				curToken.type = TOKEN_DIV_ASSIGN;
			}
			else if (MatchNextChar('/') || MatchNextChar('*')) {
				SkipComment();
				curToken.start = nextCharPtr - 1;
				continue;
			}
			else {
				curToken.type = TOKEN_DIV;
			}
			break;
		case '%':
			if (MatchNextChar('='))
			{
				curToken.type = TOKEN_MOD_ASSIGN;
			}
			else {
				curToken.type = TOKEN_MOD;
			}
			break;

		case '~':
			if (preToken.type == TOKEN_NUM || preToken.type == TOKEN_ID)
			{
				curToken.type = TOKEN_SECTION;
			}
			else {
				curToken.type = TOKEN_BIT_NOT;
			}
			break;
		case '&':
			curToken.type = TOKEN_BIT_AND;
			break;
		case '|':
			curToken.type = TOKEN_BIT_OR;
			break;
		case '?':
			curToken.type = TOKEN_QUESTION;
			break;
		case '>':
			if (MatchNextChar('=')) {
				curToken.type = TOKEN_GREATE_EQUAL;
			}
			else if (MatchNextChar('>')) {
				curToken.type = TOKEN_BIT_SHIFT_RIGHT;
			}
			else {
				curToken.type = TOKEN_GREATE;
			}
			break;
		case '<':
			if (MatchNextChar('=')) {
				curToken.type = TOKEN_LESS_EQUAL;
			}
			else if (MatchNextChar('<')) {
				curToken.type = TOKEN_BIT_SHIFT_LEFT;
			}
			else {
				curToken.type = TOKEN_LESS;
			}
			break;
		case '!':
			if (MatchNextChar('=')) {
				curToken.type = TOKEN_NOT_EQUAL;
			}
			else {
				curToken.type = TOKEN_NOT;
			}
			break;
		case '"':
			ParseString('"');
			break;
		case '\'':
			ParseString('\'');
			break;
		case '\0':
			curToken.type = TOKEN_END;
			break;
		case ';':
			GetNextChar();
			SkipBlanks();
			curToken.start = nextCharPtr - 1;
			continue;
		case '#':
			SkipLine();
			curToken.start = nextCharPtr - 1;
			continue;
		default:
			curToken.line = line;
			if (ISALPHA(curChar) || curChar == '_'||ISWORD(curChar)||curChar=='$') {
				ParseId();
			}
			else if (ISNUM(curChar)) {
				ParseNum();
			}
			else {
				LEX_ERROR(this, "Unknown charactor:\'%c\'.", curChar);
			}
			return curToken;
		}
		curToken.length = (uint32_t)(nextCharPtr - curToken.start);
		curToken.line = line;
		GetNextChar();
		return curToken;
	}
	curToken.type = TOKEN_END;
	curToken.line = line;
	return curToken;
}

char Parser::LookAheadChar()
{
	return *nextCharPtr;
}

char Parser::GetNextChar()
{
	curChar = *(nextCharPtr++);
	return curChar;
}

bool Parser::MatchNextChar(char ch)
{
	if (LookAheadChar() == ch)
	{
		GetNextChar();
		return true;
	}
	return false;
}

bool Parser::MatchToken(TokenType expected)
{
	if (curToken.type == expected) {
		GetNextToken();
		return true;
	}
	return false;
}

void Parser::ConsumeCurToken(TokenType expected, const char* msg)
{
	if (curToken.type != expected) {
		LEX_ERROR(this, msg);
	}
	return;
}

void Parser::ConsumeNextToken(TokenType expected, const char* msg)
{
	if (GetNextToken().type != expected) {
		LEX_ERROR(this, msg);
	}
	return;
}

void Parser::ParseId(TokenType type)
{
	while (ISPLNUM(curChar) || curChar == '_' || curChar == '$'||ISWORD(curChar)) {
		GetNextChar();
	}
	uint32_t length = (uint32_t)(nextCharPtr - curToken.start - 1);
	if (type != TOKEN_NONE) {
		curToken.type = type;
	}
	else {
		curToken.type = GetWordType(curToken.start, length);
	}
	curToken.length = length;
}

void Parser::ParseString(char symbol)
{
	Buffer<uint8_t> str;
	str.init();
	while (true) {
		GetNextChar();
		if (curChar == '\0') {
			LEX_ERROR(this, "String not ended");
		}
		else if (curChar == symbol) {
			curToken.type = TOKEN_STR;
			break;
		}
		else if (curChar == '\\') {
			GetNextChar();
			switch (curChar) {
			case 'a':str.Add(this->vm, '\a'); break;
			case 'b':str.Add(this->vm, '\b'); break;
			case 'f':str.Add(this->vm, '\f'); break;
			case 'n':str.Add(this->vm, '\n'); break;
			case 'r':str.Add(this->vm, '\r'); break;
			case 't':str.Add(this->vm, '\t'); break;
			case 'v':str.Add(this->vm, '\v'); break;
			case '\\':str.Add(this->vm, '\\'); break;
			case '"':str.Add(this->vm, '"'); break;
			case '\'':str.Add(this->vm, '\''); break;
			case '?':str.Add(this->vm, '\?'); break;
			case '0':str.Add(this->vm, '\0'); break;
			case 'u':ParseUnicodeCodePoint(str); break;
			default:
				LEX_ERROR(this, "Unknown escape character:'\\%c'", curChar);
				break;
			}
		}
		else {
			str.Add(vm,curChar);
		}
	}
	ObjString objString(vm, (const char*)str.datas, str.count);
	curToken.value =OBJ_TO_VALUE(objString);
	str.Clear(vm);
}

void Parser::SkipLine()
{
	GetNextChar();
	while (curChar != '\0') {
		if (curChar == '\n') {
			line++;
			GetNextChar();
			break;
		}
		GetNextChar();
	}
	line++;
}

void Parser::SkipComment()
{
	if (LookAheadChar() == '/') {
		SkipLine();
	}
	else {
		while (curChar!='\0') {
			if (curChar == '*' && MatchNextChar('/')) {
				GetNextChar();
				break;
			}
			else if (curChar == '\n')line++;
			GetNextChar();
		}
	}
	SkipBlanks();
}

void Parser::ParseHexNum()
{
	while (ISXNUM(curChar)) {
		GetNextChar();
	}
}

void Parser::ParseDecNum()
{
	while (ISNUM(curChar)) {
		GetNextChar();
	}
	if (curChar == '.' && ISNUM(LookAheadChar())) {
		GetNextChar();
		while (ISNUM(curChar)) {
			GetNextChar();
		}
	}
}

void Parser::ParseOctNum()
{
	while (curChar >= '0' && curChar <= '7') {
		GetNextChar();
	}
}

void Parser::ParseNum()
{
	if (curChar == '0') {
		if (MatchNextChar('x')) {
			GetNextChar();
			ParseHexNum();
			curToken.value = NUM_TO_VALUE(strtol(curToken.start, NULL,16));
		}
		else if(ISNUM(LookAheadChar())) {
			ParseOctNum();
			curToken.value = NUM_TO_VALUE(strtol(curToken.start, NULL, 8));
		}
	}
	else
	{
		ParseDecNum();
		curToken.value = NUM_TO_VALUE(strtod(curToken.start, NULL));
	}
	curToken.length = (uint32_t)(nextCharPtr - curToken.start - 1);
	curToken.type = TOKEN_NUM;
}

void Parser::SkipBlanks()
{
	while (ISSPACE(curChar)) {
		if (curChar == '\n') {
			line++;
		}
		GetNextChar();
	}
}

void Parser::ParseUnicodeCodePoint(Buffer<uint8_t> &buf)
{
	uint32_t index = 0;
	int value = 0;
	uint32_t temp = 0;
	while (index++ < 4) {
		GetNextChar();
		if (ISNUM(curChar))temp = curChar - '0';
		else if (ISLOWWER(curChar))temp = curChar - 'a';
		else if (ISUPPER(curChar))temp = curChar - 'A';
		else {
			LEX_ERROR(this, "Unknown Unicode!");
		}
		value = value * 16 | temp;
	}
	uint32_t byteNum = getByteNumOfEncodeUtf8(value);
	ASSERT(byteNum != 0, "utf-8 encode bytes should be between 1 and 4!");
	buf.FillWrite(vm, 0, byteNum);
	encodeUtf8(buf.datas + buf.count - byteNum, value);
}

bool isKeyWord(const char* word, size_t length)
{
	int index = 0;
	while (keyWordToken[index].keyword != NULL)
	{
		if ((length == keyWordToken[index].length) && (memcmp(keyWordToken[index].keyword, word,length)) == 0)
		{
			return true;
		}
		index++;
	}
	return false;
}

TokenType GetWordType(const char* word, size_t length)
{
	int index = 0;
	while (keyWordToken[index].keyword != NULL)
	{
		if ((length == keyWordToken[index].length) && (memcmp(keyWordToken[index].keyword, word, length)) == 0)
		{
			return keyWordToken[index].type;
		}
		index++;

	}
	return TOKEN_ID;
}
