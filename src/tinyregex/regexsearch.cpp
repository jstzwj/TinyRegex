#include "regexsearch.h"

namespace tyre
{
    RegexSearch::RegexSearch(RegexSubMatch &_smatch, MatchFlag _flag)
        :smatch(_smatch),flag(_flag){}
    bool RegexSearch::match(State * beginState,const string_t &str, int pos)
    {
        beginPos=pos;
        //empty string
        if((flag&MatchFlag::MATCH_NOT_NULL)!=0&&str.length()==0)
        {
            return false;
        }
        //match way choose
        if((flag&MatchFlag::MATCH_BFS)==0)
        {
            return matchDfs(beginState,str,pos-1,pos);
        }
        else
        {
            return matchBfs(beginState,str,pos);
        }
    }

    bool RegexSearch::search(State * beginState, const string_t &str, int pos)
    {
        beginPos=pos;
        //empty string
        if((flag&MatchFlag::MATCH_NOT_NULL)!=0&&str.length()==0)
        {
            return false;
        }
        //search way choose
        if((flag&MatchFlag::MATCH_BFS)==0)
        {
            smatch.begin=pos;
            return searchDfs(beginState,str,pos-1,pos,false);
        }
        else
        {
            smatch.begin=pos;
            return searchBfs(beginState,str,pos-1,pos,false);
        }
    }

    bool RegexSearch::matchBfs(State * beginState,const string_t &str, int pos)
    {
        std::vector<State *> saveStack;

        while(true)
        {

        }

        return true;
    }

    bool RegexSearch::searchBfs(State *beginState, const string_t &str, int acpos, int pos, bool isLazy)
    {
        return true;
    }

    bool RegexSearch::matchDfs(State * beginState,const string_t &str, int acpos, int pos)
    {
        bool result(false);
        if((unsigned int)pos==str.length())
        {
            if(beginState->isEndState==true)
            {
                return true;
            }
        }
        if((unsigned int)pos>str.length())
        {
            return false;
        }
        for(unsigned int i=0;i<beginState->out.size();++i)
        {
            Transition &curTransition=*(beginState->out[i]);
            switch(curTransition.type)
            {
            case TransitionType::CHARS:
                if(curTransition.range.isSubSet(str[pos]))
                {
                    if(matchDfs(curTransition.target,str,acpos+1,pos+1))
                    {
                        result = true;
                    }
                }
                break;
            case TransitionType::EMPTY:
                if(matchDfs(curTransition.target,str,acpos,pos))
                {
                    result = true;
                }
                break;
            case TransitionType::BEGINLINE:
                if(pos>=1)
                {
                    if(str[pos-1]==T('\n')||str[pos-1]==T('\r'))
                    {
                        if(matchDfs(curTransition.target,str,acpos,pos))
                        {
                            result = true;
                        }
                    }
                }
                //attention no break
            case TransitionType::BEGINSTRING:
                if((flag&MATCH_NOT_BOL)==0)
                {
                    if(pos==0)
                    {
                        if(matchDfs(curTransition.target,str,acpos,pos))
                        {
                            result = true;
                        }
                    }
                }
                break;
            case TransitionType::ENDLINE:
                if(str[pos]==T('\n')||str[pos]==T('\r'))
                {
                    if(matchDfs(curTransition.target,str,acpos,pos))
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
                        if(matchDfs(curTransition.target,str,acpos,pos))
                        {
                            result = true;
                        }
                    }
                }
                break;
            case TransitionType::BEGINCAPTURE:
                smatch.captureResult.push_back(RegexPosition(acpos+1,acpos+1));
                if(matchDfs(curTransition.target,str,acpos,pos))
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
                std::vector<RegexPosition>::iterator topCapture;
                int back_saver=0;
                if(!smatch.captureResult.empty())
                {
                    topCapture=smatch.captureResult.end()-1;
                    back_saver=topCapture->end;
                    //no greedy
                    if(topCapture->end<acpos)
                        topCapture->end=acpos;

                    if(matchDfs(curTransition.target,str,acpos,pos))
                    {
                        result = true;
                    }
                    else
                    {
                        //A little bug
                        //I use back() instead of top capture because I found that push_back and pop_back
                        //may change the back of vector.-_-
                        smatch.captureResult.back().end=back_saver;
                    }
                }

            }
                break;
            case TransitionType::BEGIN_NAMED_CAPTURE:
                smatch.namedCaptureResult.insert(std::make_pair(*(curTransition.captureName),RegexPosition(acpos+1,acpos+1)));//acpos
                if(matchDfs(curTransition.target,str,acpos,pos))
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
                std::map<string_t,RegexPosition>::iterator topNamedCapture;
                int back_saver=0;
                if(smatch.namedCaptureResult.find(*(curTransition.captureName))!=smatch.namedCaptureResult.end())
                {
                    topNamedCapture=smatch.namedCaptureResult.find(*(curTransition.captureName));
                    back_saver=topNamedCapture->second.end;
                    if(topNamedCapture->second.end<acpos)
                        topNamedCapture->second.end=acpos;


                    if(matchDfs(curTransition.target,str,acpos,pos))
                    {
                        result = true;
                    }
                    else
                    {
                        //A little bug
                        //I am afraid of the problem of vector.
                        smatch.namedCaptureResult[*(curTransition.captureName)].end=back_saver;
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
                        if(matchDfs(curTransition.target,str,pos-1,pos))
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
                        if(matchDfs(curTransition.target,str,pos-1,pos))
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

    bool RegexSearch::searchDfs(State * beginState,const string_t &str, int acpos, int pos, bool isLazy)
    {
        bool result(false);
        if(pos-beginPos>10000)//replace by macro later
        {
            throw RegexError(ErrorCode::error_stack);
        }
        if((unsigned int)pos>str.length())
        {
            return false;
        }
        if(beginState->isEndState==true&&acpos>=0)
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
        for(unsigned int i=0;i<beginState->out.size();++i)
        {
            Transition &curTransition=*(beginState->out[i]);
            switch(curTransition.type)
            {
            case TransitionType::CHARS:
                if(curTransition.range.isSubSet(str[pos]))
                {
                    if(searchDfs(curTransition.target,str,acpos+1,pos+1,isLazy))
                    {
                        result = true;
                    }
                }
                break;
            case TransitionType::EMPTY:
                if(searchDfs(curTransition.target,str,acpos,pos,isLazy))
                {
                    result = true;
                }
                break;
            case TransitionType::BEGINLINE:
                if(pos>=1)
                {
                    if(str[pos-1]==T('\n')||str[pos-1]==T('\r'))
                    {
                        if(searchDfs(curTransition.target,str,acpos,pos,isLazy))
                        {
                            result = true;
                        }
                    }
                }
                //attention no break
            case TransitionType::BEGINSTRING:
                if((flag&MATCH_NOT_BOL)==0)
                {
                    if(pos==0)
                    {
                        if(searchDfs(curTransition.target,str,acpos,pos,isLazy))
                        {
                            result = true;
                        }
                    }
                }
                break;
            case TransitionType::ENDLINE:
                if(str[pos]==T('\n')||str[pos]==T('\r'))
                {
                    if(searchDfs(curTransition.target,str,acpos,pos,isLazy))
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
                        if(searchDfs(curTransition.target,str,acpos,pos,isLazy))
                        {
                            result = true;
                        }
                    }
                }
                break;
            case TransitionType::LAZY:
                if(searchDfs(curTransition.target,str,acpos,pos,true))
                {
                    result = true;
                }
                break;
            case TransitionType::BEGINCAPTURE:
                smatch.captureResult.push_back(RegexPosition(acpos+1,acpos+1));
                if(searchDfs(curTransition.target,str,acpos,pos,isLazy))
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


                    if(searchDfs(curTransition.target,str,acpos,pos,isLazy))
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
                if(searchDfs(curTransition.target,str,acpos,pos,isLazy))
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


                    if(searchDfs(curTransition.target,str,acpos,pos,isLazy))
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
                        if(searchDfs(curTransition.target,str,pos-1,pos,isLazy))
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
                        if(searchDfs(curTransition.target,str,pos-1,pos,isLazy))
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
}

