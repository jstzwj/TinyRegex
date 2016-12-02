#ifndef BASE_H
#define BASE_H
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
namespace tyre
{
#ifdef TINYREGEX_UNICODE
    typedef wchar_t char_t;
    typedef std::wstring string_t;
    typedef std::wistream istream_t;
    typedef std::wostream ostream_t;
    typedef std::wstringstream stringstream_t;
    #define CHARMAX 65536
    #define T(X) L##X
    #define toString(X) std::to_wstring(X)
    #define MAX_CHAR 65536
#else
    typedef char char_t;
    typedef std::string string_t;
    typedef std::istream istream_t;
    typedef std::ostream ostream_t;
    typedef std::stringstream stringstream_t;
    #define CHARMAX 255
    #define T(X) X
    #define toString(X) std::to_string(X)
    #define MAX_CHAR 256
#endif // TINYREGEX_UNICODE
    typedef unsigned char byte;
    typedef std::unordered_map<int,std::unordered_map<int,int> > StateTable;
}
#endif // BASE_H
