#ifndef TINYREGEX_H
#define TINYREGEX_H
#include"base.h"
#include"astnode.h"
#include"astparser.h"
namespace tyre {
    class TinyRegex
    {
    public:
        TinyRegex();
        TinyRegex(const string_t& pattern);
        ExpBase * compile (const string_t& pattern);
        bool match(const TinyRegex & regex,const string_t & str);
    private:
        ExpBase * root;
    };
}


#endif // TINYREGEX_H
