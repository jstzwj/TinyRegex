#ifndef REGEXRESULT_H
#define REGEXRESULT_H

#include<vector>
namespace tyre
{
    class RegexPosition
    {
    public:
        RegexPosition()
            :begin(0),end(0){}
        RegexPosition(int _begin,int _end)
            :begin(_begin),end(_end){}
        int line;//暂时不用
        int begin;
        int end;
    };

    class RegexResult
    {
    public:
        RegexResult(){}
        std::vector<RegexPosition> captureResult;
    };
}


#endif // REGEXRESULT_H
