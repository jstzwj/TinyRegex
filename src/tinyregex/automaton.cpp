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

    bool State::match(const string_t &str, int pos, RegexSubMatch & smatch, MatchFlag flag)
    {
        //empty string
        if((flag&MatchFlag::MATCH_NOT_NULL)!=0&&str.length()==0)
        {
            return false;
        }
        //match way choose
        if((flag&MatchFlag::MATCH_BFS)==0)
        {
            return matchDfs(str,pos-1,pos,smatch,flag);
        }
        else
        {
            return matchBfs(str,pos,smatch,flag);
        }
    }

    bool State::search(const string_t &str, int pos, RegexSubMatch & smatch, MatchFlag flag)
    {
        //empty string
        if((flag&MatchFlag::MATCH_NOT_NULL)!=0&&str.length()==0)
        {
            return false;
        }
        //search way choose
        if((flag&MatchFlag::MATCH_BFS)==0)
        {
            smatch.begin=pos;
            return searchDfs(str,pos,pos-1,pos,smatch,flag);
        }
        else
        {
            smatch.begin=pos;
            return searchBfs(str,pos-1,pos,smatch,flag);
        }
    }

    bool State::matchBfs(const string_t &str, int pos, RegexSubMatch &smatch, MatchFlag flag)
    {
        std::vector<State *> saveStack;

        while(true)
        {

        }

        return true;
    }

    bool State::searchBfs(const string_t &str, int acpos, int pos,RegexSubMatch & smatch, bool isLazy, MatchFlag flag)
    {
        return true;
    }

    bool State::matchDfs(const string_t &str,int acpos,int pos,RegexSubMatch & smatch,MatchFlag flag)
    {
        bool result(false);
        if((unsigned int)pos==str.length())
        {
            if(isEndState==true)
            {
                return true;
            }
        }
        if((unsigned int)pos>str.length())
        {
            return false;
        }
        for(unsigned int i=0;i<out.size();++i)
        {
            Transition &curTransition=*out[i];
            switch(curTransition.type)
            {
            case TransitionType::CHARS:
                if(curTransition.range.isSubSet(str[pos]))
                {
                    if(curTransition.target->matchDfs(str,acpos+1,pos+1,smatch,flag))
                    {
                        result = true;
                    }
                }
                break;
            case TransitionType::EMPTY:
                if(curTransition.target->matchDfs(str,acpos,pos,smatch,flag))
                {
                    result = true;
                }
                break;
            case TransitionType::BEGINLINE:
                if(str[pos-1]==T('\n')||str[pos-1]==T('\r'))
                {
                    if(curTransition.target->matchDfs(str,acpos,pos,smatch,flag))
                    {
                        result = true;
                    }
                }
                //attention no break
            case TransitionType::BEGINSTRING:
                if((flag&MATCH_NOT_BOL)==0)
                {
                    if(pos==0)
                    {
                        if(curTransition.target->matchDfs(str,acpos,pos,smatch,flag))
                        {
                            result = true;
                        }
                    }
                }
                break;
            case TransitionType::ENDLINE:
                if(str[pos]==T('\n')||str[pos]==T('\r'))
                {
                    if(curTransition.target->matchDfs(str,acpos,pos,smatch,flag))
                    {
                        result = true;
                    }
                }
                //attention no break
            case TransitionType::ENDSTRING:
                if((flag&MATCH_NOT_EOL)==0)
                {
                    if((unsigned int)pos==str.length())
                    {
                        if(curTransition.target->matchDfs(str,acpos,pos,smatch,flag))
                        {
                            result = true;
                        }
                    }
                }
                break;
            case TransitionType::BEGINCAPTURE:
                smatch.captureResult.push_back(RegexPosition(acpos+1,acpos+1));
                if(curTransition.target->matchDfs(str,acpos,pos,smatch,flag))
                {
                    result = true;
                }
                else
                {
                    smatch.captureResult.pop_back();
                }
                break;
            case TransitionType::ENDCAPTURE:
            {
                RegexPosition * topCapture=nullptr;
                int back_saver=0;
                if(!smatch.captureResult.empty())
                {
                    topCapture=&smatch.captureResult.back();
                    back_saver=topCapture->end;
                    //no greedy
                    if(topCapture->end<acpos)
                        topCapture->end=acpos;

                    if(curTransition.target->matchDfs(str,acpos,pos,smatch,flag))
                    {
                        result = true;
                    }
                    else
                    {
                        topCapture->end=back_saver;
                    }
                }

            }
                break;
            case TransitionType::BEGIN_NAMED_CAPTURE:
                smatch.namedCaptureResult.insert(std::make_pair(*(curTransition.captureName),RegexPosition(acpos+1,acpos+1)));//acpos
                if(curTransition.target->matchDfs(str,acpos,pos,smatch,flag))
                {
                    result = true;
                }
                else
                {
                    smatch.namedCaptureResult.erase(*(curTransition.captureName));
                }
                break;
            case TransitionType::END_NAMED_CAPTURE:
            {
                RegexPosition * topNamedCapture=nullptr;
                int back_saver=0;
                if(smatch.namedCaptureResult.find(*(curTransition.captureName))!=smatch.namedCaptureResult.end())
                {
                    topNamedCapture=&smatch.namedCaptureResult[*(curTransition.captureName)];
                    back_saver=topNamedCapture->end;
                    if(topNamedCapture->end<acpos)
                        topNamedCapture->end=acpos;


                    if(curTransition.target->matchDfs(str,acpos,pos,smatch,flag))
                    {
                        result = true;
                    }
                    else
                    {
                        topNamedCapture->end=back_saver;
                    }
                }
            }
                break;
            case TransitionType::CAPTURE_REFERENCE:
            {
                int num=curTransition.captureNum;
                if((num>=0)&&((unsigned)num<smatch.captureResult.size()))
                {
                    if(StringUtilities::isStr(str,
                                              pos,
                                              str,
                                              smatch.captureResult[num].begin,
                                              smatch.captureResult[num].end))
                    {
                        //Pay attention to the acpos is pos-1.
                        if(curTransition.target->matchDfs(str,pos-1,pos,smatch,flag))
                        {
                            result = true;
                        }
                    }
                }
            }
                break;
            case TransitionType::NAME_CAPTURE_REFERENCE:
            {
                string_t & name=*(curTransition.captureName);
                if(smatch.namedCaptureResult.find(name)!=smatch.namedCaptureResult.end())
                {
                    if(StringUtilities::isStr(str,
                                              pos,
                                              str,
                                              smatch.namedCaptureResult[name].begin,
                                              smatch.namedCaptureResult[name].end))
                    {
                        if(curTransition.target->matchDfs(str,pos-1,pos,smatch,flag))
                        {
                            result = true;
                        }
                    }
                }
            }
                break;
            default:
                break;
            }
            if(result==true)
            {
                break;
            }
        }
        return result;
    }

    bool State::searchDfs(const string_t &str,int beginpos,int acpos, int pos, RegexSubMatch & smatch,bool isLazy,MatchFlag flag)
    {
        bool result(false);
        if(pos-beginpos>10000)//replace by macro later
        {
            throw RegexError(ErrorCode::error_stack);
        }
        if((unsigned int)pos>str.length())
        {
            return false;
        }
        if(isEndState==true&&acpos>=0)
        {
            if(isLazy==true)
            {
                smatch.end=acpos;
                return true;
            }
            else
            {
                if(smatch.end<acpos)
                    smatch.end=acpos;
                result=true;
            }
        }
        for(unsigned int i=0;i<out.size();++i)
        {
            Transition &curTransition=*out[i];
            switch(curTransition.type)
            {
            case TransitionType::CHARS:
                if(curTransition.range.isSubSet(str[pos]))
                {
                    if(curTransition.target->searchDfs(str,beginpos,acpos+1,pos+1,smatch,isLazy,flag))
                    {
                        result = true;
                    }
                }
                break;
            case TransitionType::EMPTY:
                if(curTransition.target->searchDfs(str,beginpos,acpos,pos,smatch,isLazy,flag))
                {
                    result = true;
                }
                break;
            case TransitionType::BEGINLINE:
                if(str[pos-1]==T('\n')||str[pos-1]==T('\r'))
                {
                    if(curTransition.target->matchDfs(str,acpos,pos,smatch,flag))
                    {
                        result = true;
                    }
                }
                //attention no break
            case TransitionType::BEGINSTRING:
                if((flag&MATCH_NOT_BOL)==0)
                {
                    if(pos==0)
                    {
                        if(curTransition.target->searchDfs(str,beginpos,acpos,pos,smatch,isLazy,flag))
                        {
                            result = true;
                        }
                    }
                }
                break;
            case TransitionType::ENDLINE:
                if(str[pos]==T('\n')||str[pos]==T('\r'))
                {
                    if(curTransition.target->matchDfs(str,acpos,pos,smatch,flag))
                    {
                        result = true;
                    }
                }
                //attention no break
            case TransitionType::ENDSTRING:
                if((flag&MATCH_NOT_EOL)==0)
                {
                    if((unsigned int)pos==str.length())
                    {
                        if(curTransition.target->searchDfs(str,beginpos,acpos,pos,smatch,isLazy,flag))
                        {
                            result = true;
                        }
                    }
                }
                break;
            case TransitionType::LAZY:
                if(curTransition.target->searchDfs(str,beginpos,acpos,pos,smatch,true,flag))
                {
                    result = true;
                }
                break;
            case TransitionType::BEGINCAPTURE:
                smatch.captureResult.push_back(RegexPosition(acpos+1,acpos+1));
                if(curTransition.target->searchDfs(str,beginpos,acpos,pos,smatch,isLazy,flag))
                {
                    result = true;
                }
                else
                {
                    smatch.captureResult.pop_back();
                }
                break;
            case TransitionType::ENDCAPTURE:
            {
                RegexPosition * topPosition=nullptr;
                int back_saver=0;
                if(!smatch.captureResult.empty())
                {
                    topPosition=&smatch.captureResult.back();
                    back_saver=topPosition->end;
                    //no greedy
                    if(topPosition->end<acpos)
                        topPosition->end=acpos;


                    if(curTransition.target->searchDfs(str,beginpos,acpos,pos,smatch,isLazy,flag))
                    {
                        result = true;
                    }
                    else
                    {
                        topPosition->end=back_saver;
                    }
                }
            }
                break;
            case TransitionType::BEGIN_NAMED_CAPTURE:
                smatch.namedCaptureResult.insert(std::make_pair(*(curTransition.captureName),RegexPosition(acpos+1,acpos+1)));
                if(curTransition.target->searchDfs(str,beginpos,acpos,pos,smatch,isLazy,flag))
                {
                    result = true;
                }
                else
                {
                    smatch.namedCaptureResult.erase(*(curTransition.captureName));
                }
                break;
            case TransitionType::END_NAMED_CAPTURE:
            {
                RegexPosition * topNamePosition=nullptr;
                int back_saver=0;
                if(smatch.namedCaptureResult.find(*(curTransition.captureName))!=smatch.namedCaptureResult.end())
                {
                    topNamePosition=&smatch.namedCaptureResult[*(curTransition.captureName)];
                    back_saver=topNamePosition->end;
                    if(topNamePosition->end<acpos)
                        topNamePosition->end=acpos;


                    if(curTransition.target->searchDfs(str,beginpos,acpos,pos,smatch,isLazy,flag))
                    {
                        result = true;
                    }
                    else
                    {
                        topNamePosition->end=back_saver;
                    }
                }
            }
                break;
            case TransitionType::CAPTURE_REFERENCE:
            {
                int num=curTransition.captureNum;
                if((num>=0)&&((unsigned)num<smatch.captureResult.size()))
                {
                    if(StringUtilities::isStr(str,
                                              pos,
                                              str,
                                              smatch.captureResult[num].begin,
                                              smatch.captureResult[num].end))
                    {
                        //Pay attention to the acpos is pos-1.
                        if(curTransition.target->searchDfs(str,beginpos,pos-1,pos,smatch,isLazy,flag))
                        {
                            result = true;
                        }
                    }
                }
            }
                break;
            case TransitionType::NAME_CAPTURE_REFERENCE:
            {
                string_t & name=*(curTransition.captureName);
                if(smatch.namedCaptureResult.find(name)!=smatch.namedCaptureResult.end())
                {
                    if(StringUtilities::isStr(str,
                                              pos,
                                              str,
                                              smatch.namedCaptureResult[name].begin,
                                              smatch.namedCaptureResult[name].end))
                    {
                        //Pay attention to the acpos is pos-1.
                        if(curTransition.target->searchDfs(str,beginpos,pos-1,pos,smatch,isLazy,flag))
                        {
                            result = true;
                        }
                    }
                }
            }
                break;
            default:
                break;
            }
        }

        return result;
    }

}//namespace tyre

