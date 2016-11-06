#pragma once
#ifndef TINYREGEXPURE
#define TINYREGEXPURE
#include"tr_type.h"
namespace tinyregex
{
	class Token
	{
	public:
		//源字符串
		const char_t * sourceStr;
		//字符串位置
		StringPos strPos;
	};
	class TinyRegexPure
	{
	private:
		string_t exp;
	public:
		TinyRegexPure();
		TinyRegexPure(const string_t & str);
		//编译正则表达式
		std::vector<Token> compile(const string_t &exp);
		//转换为Token
		std::vector<Token> GetToken(const string_t &exp);
	};
}
#endif // !TINYREGEXPURE

