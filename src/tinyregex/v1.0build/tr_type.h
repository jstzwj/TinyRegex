#pragma once
#include<cctype>
#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<stack>
#include<deque>
#include<unordered_map>

#define TINYREGEX_UNICODE
namespace tinyregex
{
#ifdef TINYREGEX_UNICODE
	typedef wchar_t char_t;
	typedef std::wstring string_t;
	typedef std::wistream istream_t;
	typedef std::wostream ostream_t;
	typedef std::wstringstream stringstream_t;
	#define CHARMAX 65536
	#define T(X) L##X
#else
	typedef char char_t;
	typedef std::string string_t;
	typedef std::istream istream_t;
	typedef std::ostream ostream_t;
	typedef std::stringstream stringstream_t;
	#define CHARMAX 255
	#define T(X) X
#endif // TINYREGEX_UNICODE
	typedef unsigned char byte;
	typedef std::unordered_map<int,std::unordered_map<int,int> > StateTable;
}