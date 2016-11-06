#include"TinyRegexPure.h"
namespace tinyregex
{




	TinyRegexPure::TinyRegexPure()
	:exp(){}

	TinyRegexPure::TinyRegexPure(const string_t & str)
	{
		std::vector<Token> vecToken;
		exp = str;
		vecToken = compile(exp);


	}

	std::vector<Token> TinyRegexPure::compile(const string_t & exp)
	{
		int len = exp.length();
		std::vector<Token> result;
		for (unsigned int i = 0; i < len;++i)
		{
			Token token;
			token.sourceStr = exp.c_str();
			
			switch (exp[i])
			{
			case T('\\'):
				if (i < len - 3 && exp[i + 1] == T('x'))
				{
					token.strPos.start = i;
					token.strPos.end = i + 4;
					i = i + 3;
					continue;
				}
				else if (i<len - 5 && exp[i+1]==T('u'))
				{
					token.strPos.start = i;
					token.strPos.end = i + 6;
					i = i + 5;
					continue;
				}
				else
				{
					token.strPos.start = i;
					token.strPos.end = i + 2;
					i = i + 1;
					continue;
				}
				break;
			case T('('):
				if (i < len - 2 && exp[i + 1] == T('?') && (exp[i + 2] == T(':') || exp[i + 2] == T('=') || exp[i + 2] == T('!')))
				{
					token.strPos.start = i;
					token.strPos.end = i + 3;
					i = i + 2;
				}
				else
				{
					token.strPos.start = i;
					token.strPos.end = i + 1;
				}
				break;
			case T(')'):
				token.strPos.start = i;
				token.strPos.end = i + 1;
				break;
			case T('^'):

				break;
			case T('$'):
				break; 
			case T('*'):
				break;
			case T('.'):
				break;
			case T('+'):
				break;
			case T('?'):
				break;
			case T('['):
				break;
			case T(']'):
				break;
			case T('-'):
				break;
			case T('{'):
				break;
			case T('}'):
				break;
			case T(','):
				break;
			case T('|'):
				break;
			case T(':'):
				break;
			case T('='):
				break;
			case T('!'):
				break;
			default:
				
			}
			result.push_back(token);
		}

		return ;
	}

	std::vector<Token> TinyRegexPure::GetToken(const string_t & exp)
	{
		return std::vector<Token>();
	}

}