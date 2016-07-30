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
	}


	TinyRegex::~TinyRegex()
	{
	}

	string_t TinyRegex::regexParse()
	{
		std::stack<Token> operatorStack;
		string_t ans;
		int len = pattern.length();
		//存入优先级最低的结束符
		operatorStack.push(make_token(L"",5));
		//遍历
		for (int i = 0; i < len;++i)
		{
			if (pattern[i] == '\\')	//1
			{
				if (pattern[i+1]=='c')
				{

				}
				else if (pattern[i + 1] == 'c')
				{


				}
				//优先级最高
				if (operatorStack.top().priority>=1)
				{
					operatorStack.push(make_token());
				}
			}
			else if (pattern[i]=='(')
			{

			}
			else if (pattern[i] == ')')
			{

			}
			else if (pattern[i] == '[')
			{

			}
			else if (pattern[i] == ']')
			{

			}
			else if (pattern[i] == '^')
			{

			}
			else if (pattern[i] == '$')
			{

			}
			else if (pattern[i] == '|')
			{

			}
			else
			{

			}
		}


		return 0;
	}


	bool regex_match(const string_t & source, const TinyRegex & pattern, TinyResult & result)
	{
		return false;
	}

}
