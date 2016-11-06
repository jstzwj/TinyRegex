#pragma once
#include"tr_type.h"
#include"TinyRegexAst.h"
namespace tinyregex
{
	class TinyRegexParser
	{
	public:
		TinyRegexParser();
		~TinyRegexParser();
		ExpPtr Parse(char_t *& str);
		ExpPtr ParseChar(char_t *& str);
		ExpPtr ParseSeqChar(char_t *& str);
		ExpPtr ParseCharSet(char_t *& str);
		ExpPtr ParseAnyChar(char_t *& str);
		std::shared_ptr<Loop> ParseLoop(char_t *& str);
		ExpPtr ParseUnit(char_t *& str);
		ExpPtr ParseFunction(char_t *& str);
		ExpPtr ParseSequence(char_t *& str);
		ExpPtr ParseExp(char_t *& str);
	};
}


