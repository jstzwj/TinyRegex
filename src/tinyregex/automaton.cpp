#include "automaton.h"


namespace tyre
{
    Automaton::Automaton()
    {

    }

    Automaton::~Automaton()
    {
        for(unsigned int i=0;i<states.size();++i)
        {
            delete states[i];
        }
        for(unsigned int i=0;i<transitions.size();++i)
        {
            delete transitions[i];
        }
    }

    State *Automaton::addState()
    {
        State * newState=new State;
        newState->isEndState=false;

        states.push_back(newState);
        return newState;
    }

    Transition *Automaton::addTransition(State *start, State *end)
    {
        Transition * newTransition=new Transition;

        newTransition->source=start;
        newTransition->target=end;
        newTransition->passCounter=0;

        transitions.push_back(newTransition);
        start->out.push_back(newTransition);
        end->in.push_back(newTransition);
        return newTransition;
    }

    Transition *Automaton::addCharRange(State *start, State *end, CharRange range)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::CHARS;
        newTransition->range=range;
        return newTransition;
    }



    Transition *Automaton::addEmptyTransition(State *start, State *end)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::EMPTY;
        return newTransition;
    }

    Transition *Automaton::addLoop(State *start, State *end)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::EMPTY;
        return newTransition;
    }
    Transition *Automaton::addLoop(State *start, State *end, CharRange range)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::CHARS;
        newTransition->range=range;
        return newTransition;
    }
}

