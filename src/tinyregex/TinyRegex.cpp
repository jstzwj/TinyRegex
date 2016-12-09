#include "tinyregex.h"
namespace tyre
{
    TinyRegex::TinyRegex()
        :root(nullptr),graph(nullptr)
    {

    }

    TinyRegex::TinyRegex(const string_t &pattern)
        :root(nullptr),graph(nullptr)
    {
        compile(pattern);
    }

    TinyRegex::~TinyRegex()
    {
        if(root!=nullptr)
        {
            delete root;
        }
        if(graph!=nullptr)
        {
            delete graph;
        }
    }

    void TinyRegex::compile(const tyre::string_t &pattern)
    {
        AstParser parser;
        if(root!=nullptr)
        {
            delete root;
        }
        root=parser.parse(pattern);
        if(graph!=nullptr)
        {
            delete graph;
        }
        graph=new Automaton;

        nfa=root->generate(graph);
        nfa.end->isEndState=true;
        return;
    }

    bool TinyRegex::match(const tyre::string_t &str)
    {
        return nfa.begin->match(str,0);
    }
}


