#ifndef TINYREGEX_H
#define TINYREGEX_H

#include<list>

#include"base.h"
#include"astnode.h"
#include"astparser.h"

namespace tyre
{
    class TinyRegex
    {
    public:
        TinyRegex();
        TinyRegex(const char_t * pattern,SyntaxFlag flag=SyntaxFlag::DEFAULT);
        TinyRegex(const string_t& pattern,SyntaxFlag flag=SyntaxFlag::DEFAULT);
        ~TinyRegex();
        void compile(const string_t& pattern, SyntaxFlag flag=SyntaxFlag::DEFAULT);
        bool match(const string_t & str);
        RegexResult search(const string_t & str);
    private:
        ExpBase * root;
        Automaton * graph;
        NfaGraph nfa;
    };
}


#endif // TINYREGEX_H
