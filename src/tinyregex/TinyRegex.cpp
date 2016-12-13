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
    TinyRegex::TinyRegex(const char_t * pattern)
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

    void TinyRegex::compile(const string_t &pattern)
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

        nfa.begin=graph->addState();
        nfa.end=graph->addState();
        root->generate(graph,nfa);
        nfa.applyToAutomaton(graph);

        return;
    }

    bool TinyRegex::match(const tyre::string_t &str)
    {
        return graph->beginState->match(str,0);
    }

    RegexResult TinyRegex::search(const string_t &str)
    {
        RegexResult result;
        int endPos;
        for(unsigned int i=0;i<str.length();++i)
        {
            if(graph->beginState->search(str,i,&endPos)==true)
            {
                result.captureResult.push_back(RegexPosition(i,endPos));
                i=endPos;
            }
        }
        return result;
    }

}


