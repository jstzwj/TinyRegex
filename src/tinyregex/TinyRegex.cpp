#include "tinyregex.h"
namespace tyre
{
    TinyRegex::TinyRegex()
    {

    }

    TinyRegex::TinyRegex(const string_t &pattern)
    {
        compile(pattern);
    }

    void TinyRegex::compile(const tyre::string_t &pattern)
    {
        AstParser parser;
        root=parser.parse(pattern);

        nfa=new NfaGraph;
        graph=new Automaton;
        *nfa=root->generate(graph);
        return;
    }

    bool TinyRegex::match(const tyre::TinyRegex &regex, const tyre::string_t &str)
    {
        std::list<StateFrame> vstack;
        State * curState=nfa->begin;
        int i=0;




    }
}


