#include <iostream>
#include"tinyregex.h"

using namespace std;
using namespace tyre;

int main(int argc, char *argv[])
{
    TinyRegex r(T("hello regex\\.*|hell0"));
    bool result = r.match(r,T("hello"));
    std::cout << result;
    return 0;
}
#include<iostream>


