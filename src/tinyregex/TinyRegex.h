#pragma once
#include"tr_type.h"
#include"TinyResult.h"
#include"TinyRegexAst.h"
#include"TinyRegexParser.h"
#include<memory>
namespace tinyregex
{
	
	//正则表达式编译
	class TinyRegex
	{
	private:
		string_t pattern;
		std::vector<CharRange> charrange;
		ExpPtr out;
		string_t error;
	public:
		TinyRegex();
		TinyRegex(const string_t &p);
		void assign(const string_t &p);
		~TinyRegex();
		
	private:
		ExpPtr regexParse();
	};

	bool regex_match(const string_t &source,const TinyRegex &pattern,TinyResult & result);
}

