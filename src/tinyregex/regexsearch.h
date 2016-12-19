#ifndef REGEXSEARCH_H
#define REGEXSEARCH_H
#include<map>
#include<vector>

#include"base.h"
#include"attribute.h"
#include"regexresult.h"
#include"stringutilities.h"
#include"automaton.h"

namespace tyre
{
    class RegexSearch
    {
    private:
        RegexSubMatch &smatch;
        MatchFlag flag;
        int beginPos;
    public:
        RegexSearch(RegexSubMatch &_smatch, MatchFlag _flag=MatchFlag::MATCH_DEFAULT);
        bool match(State * beginState,const string_t &str, int pos);
        bool search(State * beginState,const string_t &str, int pos);
    private:
        bool matchDfs(State * beginState,const string_t &str, int acpos, int pos);
        bool matchBfs(State * beginState,const string_t &str, int pos);
        bool searchDfs(State * beginState,const string_t &str, int acpos, int pos, bool isLazy);
        bool searchBfs(State * beginState,const string_t &str, int acpos, int pos, bool isLazy);
    };
}

#endif // REGEXSEARCH_H
