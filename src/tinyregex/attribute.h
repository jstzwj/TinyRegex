#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H
#include"base.h"
namespace tyre
{
    enum CompileFlag
    {
        NFA_AUTOMATON=0x1,  //&~DFA_AUTOMATON
    };
    enum MatchFlag
    {
        MATCH_DEFAULT=0x0,
        MATCH_NOT_BOL=0x1,  //The first character in [first,last) will be treated as if it is not at the beginning of a line (i.e. ^ will not match [first,first)
        MATCH_NOT_EOL=0x2,  //The last character in [first,last) will be treated as if it is not at the end of a line (i.e. $ will not match[last,last)
        MATCH_NOT_NULL=0x4, //Do not match empty sequences
        MATCH_CONTINUOUS=0x8,  //Only match a sub-sequence that begins at first
        MATCH_BFS=0x10, //&~MATCH_DFS
    };
}

#endif // ATTRIBUTE_H
