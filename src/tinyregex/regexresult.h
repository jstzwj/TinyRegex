#ifndef REGEXRESULT_H
#define REGEXRESULT_H

#include<vector>
#include"base.h"
namespace tyre
{
    class RegexPosition
    {
    public:
        RegexPosition()
            :begin(0),end(0){}
        RegexPosition(int _begin,int _end)
            :begin(_begin),end(_end){}
        RegexPosition(int _begin,int _end,const string_t name)
            :begin(_begin),end(_end),captureName(name){}
        int line;//暂时不用
        int begin;
        int end;
        string_t captureName;
    };

    class RegexResult
    {
    public:
        RegexResult(){}
        std::vector<RegexPosition> captureResult;
    };
}


#endif // REGEXRESULT_H
