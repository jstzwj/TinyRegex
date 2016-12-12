#ifndef NFAGRAPH_H
#define NFAGRAPH_H
#include"base.h"

#include"automaton.h"
class State;
namespace tyre
{
    class NfaGraph
    {
    public:
        NfaGraph();
        NfaGraph(State *begin_,State *end_)
            :begin(begin_),end(end_){}
    public:
        State * begin;
        State * end;
    public:
        void applyToAutomaton(Automaton *automaton);
    };
}


#endif // NFAGRAPH_H
