#include "TinyRegex.h"


namespace tinyregex
{
	TinyRegex::TinyRegex()
	{
	}

	tinyregex::TinyRegex::TinyRegex(const string_t & p)
		:pattern(p)
	{
		//编译表达式
		out = regexParse();
	}

	void TinyRegex::assign(const string_t & p)
	{
		if (p != pattern)
		{
			pattern = p;
			out=regexParse();
		}
	}


	TinyRegex::~TinyRegex()
	{
	}
	
	//递归下降系列
	ExpPtr TinyRegex::ParseChar(char_t *& str)
	{
		std::shared_ptr<SeqChar> cur(new SeqChar);
		cur->range.start = *str;
		cur->range.end = (*str) + 1;
		return cur;
	}
	ExpPtr TinyRegex::ParseSeqChar(char_t *& str)
	{

	}
	ExpPtr TinyRegex::ParseCharSet(char_t *& str)
	{

	}
	ExpPtr TinyRegex::ParseAnyChar(char_t *& str)
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
	ExpPtr TinyRegex::ParseLoop(char_t *& str)
	{
		std::shared_ptr<Loop> cur (new Loop);
		if (*str==T('{'))
		{
			++str;
			if (*str==T(','))
			{
				cur->range.start = 0;
				++str;
				if (*str==T('}'))
				{
					cur->range.end = -1;
					++str;
				}
				else
				{
					cur->range.end = ArrayToInt(str)+1;
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
				cur->range.start = ArrayToInt(str)+1;
				if (*str==T(','))
				{
					
					if (*str==T('}'))
					{
						cur->range.end = -1;
						++str;
					}
					else
					{
						cur->range.end = ArrayToInt(str)+1;
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
		else if(*str==T('*'))
		{
			cur->range.start = 0;
			cur->range.end = -1;
			++str;
		}
		else if(*str==T('+'))
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

	ExpPtr TinyRegex::ParseFunction(char_t *& str)
	{

	}
	ExpPtr TinyRegex::ParseSequence(char_t *& str)
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
	ExpPtr TinyRegex::ParseUnit(char_t *& str)
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
		std::shared_ptr<Loop> op = ParseLoop(str);
		if (op!=NULL)
		{
			op->left = cur;
			cur = op;
		}
		return cur;
	}
	
	ExpPtr TinyRegex::ParseExp(char_t *& str)
	{
		ExpPtr cur = ParseSequence(str);
		while (true)
		{
			if (*str==T('|'))
			{
				std::shared_ptr<OpOr> op(new OpOr);
				ExpPtr right = ParseSequence(str);
				if (right!=NULL)
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
	//解析为语法树
	ExpPtr TinyRegex::regexParse()
	{
		char_t * start = &*(pattern.begin());
		ExpPtr root = ParseExp(start);

	}

	void TinyRegex::genCharSetTable(const std::vector<CharRange>& charrange, const std::vector<Token> & re)
	{
		std::vector<std::pair<char_t, int> > rangetemp;
		std::vector<Token> numStack;
		int len = re.size();
		for (int i = 0; i < len;++i)
		{
			if (re[i].isOperator==false)
			{
				numStack.push_back(re[i]);
			}
			else
			{
				//判断是哪个符号
				if (re[i].content==T("&"))
				{

				}
				else if (re[i].content == T("|"))
				{

				}
			}

		}


	}



	bool regex_match(const string_t & source, const TinyRegex & pattern, TinyResult & result)
	{
		return false;
	}

}
