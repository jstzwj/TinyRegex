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
	};
	inline Token make_token(const string_t &content,int priority )
	{
		Token temp;
		temp.content = content;
		temp.priority = priority;
		return temp;
	}
	class TinyRegex
	{
	private:
		string_t pattern;
	public:
		TinyRegex();
		TinyRegex(const string_t &pattern);
		~TinyRegex();
	private:
		string_t regexParse();
	};
	bool regex_match(const string_t &source,const TinyRegex &pattern,TinyResult & result);
}

