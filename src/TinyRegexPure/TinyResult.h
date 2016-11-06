#pragma once
#include"tr_type.h"
namespace tinyregex
{
	

	class TinyMatch
	{
	public:
		//源字符串
		char_t * sourceStr;
		//捕获名称
		string_t name;
		//捕获序号
		int id;
		//字符串位置
		StringPos strPos;
		//在文本中开始位置
		TextPos strStart;
		//文本中结束位置
		TextPos strEnd;

	};

	class TinyResult
	{
	private:
		std::vector<TinyMatch> matchlist;
	public:
		TinyResult();
	};
}


