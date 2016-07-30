#pragma once
#include"tr_type.h"
#include"TinyResult.h"
namespace tinyregex
{
	class Repeat
	{
	public:
		Repeat():start(1),end(2),isPositive(true){}
		Repeat(int s, int e, bool ip):start(s), end(e), isPositive(ip) {}
		int start;
		int end;	//³¬Î²
		bool isPositive;
	};

	class Token
	{
	public:
		string_t content;
		int priority;
		bool isOperator;
		Repeat repeat;
	};

	inline Token make_token(const string_t &content,int priority,bool isOperator )
	{
		Token temp;
		temp.content = content;
		temp.priority = priority;
		temp.isOperator = isOperator;
		return temp;
	}
	inline int wctoi(wchar_t c)
	{
		int wi[10] = { 0,1,2,3,4,5,6,7,8,9 };
		if (c>=L'0'&&c<=L'9')
		{
			return wi[c-L'0'];
		}
		else
		{
			return -1;
		}
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
		inline void addAnd(int &i, int &envNum, std::vector<Token> & operatorStack, std::vector<Token> &ans);
	};
	bool regex_match(const string_t &source,const TinyRegex &pattern,TinyResult & result);
}

