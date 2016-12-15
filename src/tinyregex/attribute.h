#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H
#include"base.h"
namespace tyre
{
    enum SyntaxFlag
    {
        DEFAULT=0x0,
        DFA_AUTOMATON=0x1,  //&~DFA_AUTOMATON
        ICASE=0x2,          //Character matching should be performed without regard to case. 
        NOSUBS=0x4,         //
        OPTIMIZE=0x8,
        COLLATE=0x10,
        MULTILINE=0x20,
        BASIC=0x40,
        EXTENDS=0x80,
        AWK=0x100,
        GREP=0x200,
        EGREP=0x400
        
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
