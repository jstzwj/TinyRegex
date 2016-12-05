#include "automaton.h"


namespace tyre
{
    Automaton::Automaton()
    {

    }

    State *Automaton::addState()
    {
        State * newState=new State;
        states.push_back(newState);
        return newState;
    }

    Transition *Automaton::addTransition(State *start, State *end)
    {
        Transition * newTransition=new Transition;

        newTransition->source=start;
        newTransition->target=end;

        transitions.push_back(newTransition);
        start->out.push_back(newTransition);
        end->in.push_back(newTransition);
        return newTransition;
    }

    Transition *Automaton::addCharRange(State *start, State *end, Range range)
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

    Transition *Automaton::addLoop(State *start, State *end, int beginLoop, int endLoop)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::LOOP;
        newTransition->range=Range(beginLoop,endLoop);
        return newTransition;
    }

}

