#ifndef REGEXRESULT_H
#define REGEXRESULT_H

#include<vector>
#include<map>
#include"base.h"
namespace tyre
{
    class RegexPosition
    {
    public:
        RegexPosition():begin(0),end(0){}
        RegexPosition(int _begin,int _end):begin(_begin),end(_end){}
        RegexPosition(int _begin,int _end,const string_t name)
            :begin(_begin),end(_end),captureName(name){}
        int begin;
        int end;
        string_t captureName;
    };

    class RegexSubMatch
    {
    public:
        RegexSubMatch()
            :begin(0),end(0){}
        RegexSubMatch(int _begin,int _end)
            :begin(_begin),end(_end){}

        int begin;
        int end;
        std::vector<RegexPosition> captureResult;
        std::map<string_t,RegexPosition> namedCaptureResult;

        int mark_count(){return captureResult.size();}
    };

    class RegexResult
    {
    public:
        RegexResult(){}
        std::vector<RegexSubMatch> subMatch;
    };
}


#endif // REGEXRESULT_H
