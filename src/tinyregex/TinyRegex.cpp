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
                graph=nullptr;
            }
            graph=new Automaton;


            nfa.begin=graph->addState();
            nfa.end=graph->addState();
            //check if root have not ast
            if(root!=nullptr)
            {
                root->generate(graph,nfa);
            }
            //check if the nfa have not transition.
            if(nfa.begin->out.empty())
            {
                graph->addEmptyTransition(nfa.begin,nfa.end);
            }
            nfa.applyToAutomaton(graph);

            //Clean ast after generating the automaton.
            if(root!=nullptr)
            {
                delete root;
                root=nullptr;
            }
        }
        catch(const std::bad_alloc& ba)
        {
            throw RegexError(ErrorCode::error_space);
        }
    }

    bool TinyRegex::match(const tyre::string_t &str, MatchFlag flag)
    {
        RegexSubMatch smatch;
        RegexSearch matchAutomaton(smatch,flag);
        return matchAutomaton.match(graph->beginState,str,0);
    }

    RegexResult TinyRegex::search(const string_t &str, MatchFlag flag)
    {
        RegexResult result;
        for(unsigned int i=0;i<str.length();++i)
        {
            RegexSubMatch smatch;
            RegexSearch searchAutomaton(smatch,flag);
            if(searchAutomaton.search(graph->beginState,str,i)==true)
            {
                result.subMatch.push_back(smatch);
                i=smatch.end;
            }
        }
        return result;
    }

    string_t TinyRegex::replace(const string_t &str, const string_t &replaceStr, MatchFlag flag)
    {
        RegexResult result=this->search(str,flag);
        RegexSubMatch smatch;
        RegexSearch searchAutomaton(smatch,flag);
        return searchAutomaton.replace(result,str,0,replaceStr);
    }

}


