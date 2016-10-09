#pragma once
#include"tr_type.h"
#include"TinyResult.h"
namespace tinyregex
{
	//用于表示重复
	class Repeat
	{
	public:
		Repeat():start(1),end(2),isPositive(true){}
		Repeat(int s, int e, bool ip):start(s), end(e), isPositive(ip) {}
		int start;
		int end;	//超尾
		bool isPositive;
	};
	//表示一个字符或操作符
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
	//宽字符转数字
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
	//字符转数字
	inline int wctoi(char c)
	{
		int wi[10] = { 0,1,2,3,4,5,6,7,8,9 };
		if (c >= '0'&&c <= '9')
		{
			return wi[c - '0'];
		}
		else
		{
			return -1;
		}
	}
	//表示一个连续的字符集合
	class CharRange
	{
	public:
		char_t start;
		char_t end;
	};
	//正则表达式编译
	class TinyRegex
	{
	private:
		string_t pattern;
		std::vector<CharRange> genCharSetTable;
	public:
		TinyRegex();
		TinyRegex(const string_t &p);
		void assign(const string_t &p);
		~TinyRegex();
	private:
		std::vector<Token> regexParse();
		void genCharSetTable(const std::vector<CharRange> &charrange,const std::vector<Token> &re);
		inline void addAnd(int &i, int &envNum, std::vector<Token> & operatorStack, std::vector<Token> &ans);
	};

	bool regex_match(const string_t &source,const TinyRegex &pattern,TinyResult & result);
}

