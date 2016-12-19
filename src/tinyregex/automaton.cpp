#include "automaton.h"


namespace tyre
{
    Automaton::Automaton()
        :beginState(nullptr)
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
        State * newState=new State();
        newState->isEndState=false;

        states.push_back(newState);
        return newState;
    }

    Transition *Automaton::addTransition(State *start, State *end)
    {
        Transition * newTransition=new Transition;

        newTransition->source=start;
        newTransition->target=end;
        //newTransition->passCounter=0;

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

    Transition *Automaton::addLazyTransition(State *start, State *end)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::LAZY;
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

    Transition *Automaton::addBeginString(State *start, State *end)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::BEGINSTRING;
        return newTransition;
    }

    Transition *Automaton::addEndString(State *start, State *end)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::ENDSTRING;
        return newTransition;
    }

    Transition *Automaton::addBeginLine(State *start, State *end)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::BEGINLINE;
        return newTransition;
    }

    Transition *Automaton::addEndLine(State *start, State *end)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::ENDLINE;
        return newTransition;
    }

    Transition *Automaton::addBeginCapture(State *start, State *end)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::BEGINCAPTURE;
        return newTransition;
    }

    Transition *Automaton::addEndCapture(State *start, State *end)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::ENDCAPTURE;
        return newTransition;
    }

    Transition *Automaton::addBeginNamedCapture(State *start, State *end, const string_t &name)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::BEGIN_NAMED_CAPTURE;
        newTransition->captureName=new string_t(name);
        return newTransition;
    }

    Transition *Automaton::addEndNamedCapture(State *start, State *end, const string_t &name)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::END_NAMED_CAPTURE;
        newTransition->captureName=new string_t(name);
        return newTransition;
    }

    Transition *Automaton::addCaptureReference(State *start, State *end, int pos)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::CAPTURE_REFERENCE;
        newTransition->captureNum=pos;
        return newTransition;
    }

    Transition *Automaton::addCaptureReference(State *start, State *end, const string_t &name)
    {
        Transition * newTransition=addTransition(start,end);
        newTransition->type=TransitionType::NAME_CAPTURE_REFERENCE;
        newTransition->captureName=new string_t(name);
        return newTransition;
    }

    Automaton Automaton::NfaToDfa(const Automaton &automaton)
    {
        Automaton dfa;

        return dfa;
    }



}//namespace tyre

