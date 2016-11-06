#include "TinyRegexParser.h"

namespace tinyregex
{
	TinyRegexParser::TinyRegexParser()
	{
	}


	TinyRegexParser::~TinyRegexParser()
	{
	}
	ExpPtr TinyRegexParser::Parse(char_t *& str)
	{
		return ParseExp(str);
	}
	//µÝ¹éÏÂ½µÏµÁÐ
	ExpPtr TinyRegexParser::ParseChar(char_t *& str)
	{
		std::shared_ptr<SeqChar> cur(new SeqChar);
		cur->range.start = *str;
		cur->range.end = (*str) + 1;
		return cur;
	}
	ExpPtr TinyRegexParser::ParseSeqChar(char_t *& str)
	{
		std::shared_ptr<SeqChar> seqchar(new SeqChar);
		seqchar->range.start = eatchar(str);
		if (*(str+1)==T('-'))
		{
			eatchar(str);
			seqchar->range.end= eatchar(str);
			return seqchar;
		}
		else
		{
			return NULL;
		}
	}
	ExpPtr TinyRegexParser::ParseCharSet(char_t *& str)
	{

	}
	ExpPtr TinyRegexParser::ParseAnyChar(char_t *& str)
	{
		std::shared_ptr<ExpBase> cur;
		cur = ParseCharSet(str);
		if (cur != NULL)
		{
			return cur;
		}
		else
		{
			return NULL;
		}
	}
	std::shared_ptr<Loop> TinyRegexParser::ParseLoop(char_t *& str)
	{
		std::shared_ptr<Loop> cur(new Loop);
		if (*str == T('{'))
		{
			++str;
			if (*str == T(','))
			{
				cur->range.start = 0;
				++str;
				if (*str == T('}'))
				{
					cur->range.end = -1;
					++str;
				}
				else
				{
					cur->range.end = ArrayToInt(str) + 1;
					if (*str == T('}'))
					{
						++str;
					}
					else
					{
						throw "error : cannot find right \'}\'.";
					}
				}
			}
			else
			{
				cur->range.start = ArrayToInt(str) + 1;
				if (*str == T(','))
				{

					if (*str == T('}'))
					{
						cur->range.end = -1;
						++str;
					}
					else
					{
						cur->range.end = ArrayToInt(str) + 1;
						if (*str == T('}'))
						{
							++str;
						}
						else
						{
							throw "error : cannot find right \'}\'.";
						}

					}
				}
				else
				{
					throw "error: cannot find \',\' in loop.";
				}
			}
		}
		else if (*str == T('*'))
		{
			cur->range.start = 0;
			cur->range.end = -1;
			++str;
		}
		else if (*str == T('+'))
		{
			cur->range.start = 1;
			cur->range.end = -1;
			++str;
		}
		else
		{
			return NULL;
		}
	}

	ExpPtr TinyRegexParser::ParseFunction(char_t *& str)
	{

	}
	ExpPtr TinyRegexParser::ParseSequence(char_t *& str)
	{
		ExpPtr cur = ParseUnit(str);
		while (true)
		{
			std::shared_ptr<OpAnd> op(new OpAnd);
			ExpPtr right = ParseUnit(str);
			if (right != NULL)
			{
				op->left = cur;
				op->right = right;
				cur = op;
			}
			else
			{
				break;
			}
		}
		return cur;
	}
	ExpPtr TinyRegexParser::ParseUnit(char_t *& str)
	{
		ExpPtr cur = ParseAnyChar(str);
		if (cur == NULL)
		{
			cur = ParseFunction(str);
			if (cur == NULL)
			{
				throw "Error: find nothing before loop charactor;";
			}
		}
		std::shared_ptr<Loop> op=ParseLoop(str);
		if (op != NULL)
		{
			op->left = cur;
			cur = op;
		}
		return cur;
	}

	ExpPtr TinyRegexParser::ParseExp(char_t *& str)
	{
		ExpPtr cur = ParseSequence(str);
		while (true)
		{
			if (*str == T('|'))
			{
				std::shared_ptr<OpOr> op(new OpOr);
				ExpPtr right = ParseSequence(str);
				if (right != NULL)
				{
					op->left = cur;
					op->right = right;
					cur = op;
				}
				else
				{
					throw "error:can not find any charactor after alternate.";
				}
			}
			else
			{
				break;
			}
		}
		return cur;
	}
	
}

