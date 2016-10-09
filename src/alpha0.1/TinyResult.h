#pragma once
#include"tr_type.h"
namespace tinyregex
{
	class TinyMatch
	{
	public:
		string_t name;
		int start;
		int end;
	};
	class TinyResult
	{
	private:
		std::vector<TinyMatch> matchlist;
	public:
		TinyResult();
		~TinyResult();
	};
}


