#include "tinyregex.h"
namespace tyre
{
    TinyRegex::TinyRegex()
        :root(nullptr),graph(nullptr)
    {

    }

    TinyRegex::TinyRegex(const string_t &pattern, SyntaxFlag flag)
        :root(nullptr),graph(nullptr)
    {
        compile(pattern,flag);
    }
    TinyRegex::TinyRegex(const char_t * pattern, SyntaxFlag flag)
        :root(nullptr),graph(nullptr)
    {
        compile(pattern,flag);
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

    void TinyRegex::compile(const string_t &pattern, SyntaxFlag flag)
    {
        try
        {
            AstParser parser(flag);
            root=parser.parse(pattern);
            //Clean lastest automaton.
            if(graph!=nullptr)
            {
                delete graph;
            }
            graph=new Automaton;
            //Clean ast after generating the automaton.
            if(root!=nullptr)
            {
                delete root;
            }
            nfa.begin=graph->addState();
            nfa.end=graph->addState();
            root->generate(graph,nfa);
            nfa.applyToAutomaton(graph);
        }
        catch(const std::bad_alloc& ba)
        {
            throw RegexError(ErrorCode::error_space);
        }
    }

    bool TinyRegex::match(const tyre::string_t &str)
    {
        RegexSubMatch smatch;
        return graph->beginState->match(str,0,smatch);
    }

    RegexResult TinyRegex::search(const string_t &str)
    {
        RegexResult result;
        for(unsigned int i=0;i<str.length();++i)
        {
            RegexSubMatch smatch;
            if(graph->beginState->search(str,i,smatch)==true)
            {
                result.subMatch.push_back(smatch);
                i=smatch.end;
            }
        }
        return result;
    }

}


