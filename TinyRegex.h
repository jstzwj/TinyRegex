#pragma once
#include"tr_type.h"
#include"TinyResult.h"
namespace tinyregex
{
	class Token
	{
	public:
		string_t content;
		int priority;
		bool isOperator;
	};
	inline Token make_token(const string_t &content,int priority,bool isOperator )
	{
		Token temp;
		temp.content = content;
		temp.priority = priority;
		temp.isOperator = isOperator;
		return temp;
	}
	class TinyRegex
	{
	private:
		string_t pattern;
	public:
		TinyRegex();
		TinyRegex(const string_t &p);
		void assign(const string_t &p);
		~TinyRegex();
	private:
		std::vector<Token> regexParse();
		void addAnd(int &i, int &envNum, std::stack<Token> & operatorStack, std::vector<Token> &ans);
	};
	bool regex_match(const string_t &source,const TinyRegex &pattern,TinyResult & result);
}

