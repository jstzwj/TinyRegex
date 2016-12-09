#include <iostream>
#include"tinyregex.h"
#include<cassert>

using namespace std;
using namespace tyre;

int main(int argc, char *argv[])
{
    TinyRegex r;
    //single character test
    r.compile(T("a"));
    assert(true==r.match(T("a")));

    r.compile(T("b"));
    assert(true==r.match(T("b")));

    r.compile(T("c"));
    assert(true==r.match(T("c")));

    r.compile(T("z"));
    assert(true==r.match(T("z")));

    //unicode char test
    r.compile(T("你"));
    assert(true==r.match(T("你")));

    r.compile(T("测试ok"));
    assert(true==r.match(T("测试ok")));

    //escape operator test

    r.compile(T("\\."));
    assert(true==r.match(T(".")));

    r.compile(T("\\+"));
    assert(true==r.match(T("+")));

    r.compile(T("\\*"));
    assert(true==r.match(T("*")));

    r.compile(T("\\["));
    assert(true==r.match(T("[")));

    r.compile(T("\\]"));
    assert(true==r.match(T("]")));

    r.compile(T("\\{"));
    assert(true==r.match(T("{")));

    r.compile(T("\\}"));
    assert(true==r.match(T("}")));

    //basic loop test

    r.compile(T("1+"));
    assert(false==r.match(T("")));
    assert(true==r.match(T("1")));
    assert(true==r.match(T("11")));
    assert(true==r.match(T("111111")));

    r.compile(T("2*"));
    assert(true==r.match(T("2")));
    assert(true==r.match(T("22")));
    assert(true==r.match(T("22222")));

    r.compile(T("2{2,5}"));
    assert(false==r.match(T("2")));
    assert(true==r.match(T("22")));
    assert(true==r.match(T("22222")));
    assert(false==r.match(T("222222")));

    r.compile(T("3{,5}"));
    assert(true==r.match(T("")));
    assert(true==r.match(T("33")));
    assert(true==r.match(T("33333")));
    assert(false==r.match(T("333333")));

    r.compile(T("4{3,}"));
    assert(false==r.match(T("")));
    assert(true==r.match(T("444")));
    assert(true==r.match(T("44444")));
    assert(true==r.match(T("444444")));

    r.compile(T("5{,}"));
    assert(true==r.match(T("5")));
    assert(true==r.match(T("5555")));
    assert(true==r.match(T("555555")));
    assert(true==r.match(T("555555555")));

    r.compile(T("6{}"));
    assert(true==r.match(T("")));
    assert(true==r.match(T("666")));
    assert(true==r.match(T("66666")));

    //char range test
    r.compile(T("."));
    assert(true==r.match(T("4")));
    assert(true==r.match(T("6")));
    assert(true==r.match(T("b")));

    //bracket test
    r.compile(T("(ab)+"));
    assert(false==r.match(T("a")));
    assert(true==r.match(T("ab")));
    assert(false==r.match(T("aba")));
    assert(true==r.match(T("abab")));
























































    std::cout<<"Pass all tests."<<std::endl;
    return 0;
}




