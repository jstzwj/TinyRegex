#include <iostream>
#include"tinyregex.h"

using namespace std;
using namespace tyre;

int main(int argc, char *argv[])
{
    while(1)
    {
        string_t str;
        std::wcin>>str;
        TinyRegex r(T("c*d"));
        bool result = r.match(str);
        std::cout << result<<std::endl;
    }
    return 0;
}



