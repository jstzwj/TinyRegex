#include "nfagraph.h"
namespace tyre
{
    NfaGraph::NfaGraph()
    {

    }

    void NfaGraph::applyToAutomaton(Automaton *automaton)
    {
        this->end->isEndState=true;
        automaton->beginState=this->begin;
    }
}

