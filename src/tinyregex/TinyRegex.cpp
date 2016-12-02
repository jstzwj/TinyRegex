#include "tinyregex.h"
namespace tyre
{
    TinyRegex::TinyRegex()
    {

    }

    TinyRegex::TinyRegex(const string_t &pattern)
    {
        root=compile(pattern);
    }

    ExpBase *TinyRegex::compile(const tyre::string_t &pattern)
    {
        AstParser parser;
        root=parser.parse(pattern);
        return root;
    }

    bool tyre::TinyRegex::match(const tyre::TinyRegex &regex, const tyre::string_t &str)
    {

    }
}


