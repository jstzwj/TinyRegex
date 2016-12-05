#ifndef TINYREGEX_H
#define TINYREGEX_H

#include<list>

#include"base.h"
#include"astnode.h"
#include"astparser.h"
namespace tyre {
    class TinyRegex
    {
    public:
        TinyRegex();
        TinyRegex(const string_t& pattern);
        void compile(const string_t& pattern);
        bool match(const TinyRegex & regex,const string_t & str);
    private:
        ExpBase * root;
        Automaton * graph;
        NfaGraph * nfa;
    };
}


#endif // TINYREGEX_H
