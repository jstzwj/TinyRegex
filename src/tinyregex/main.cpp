#include <iostream>
#include"tinyregex.h"
#include<cassert>

#include<ctime>
#include<regex>


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

    //char range test
    r.compile(T("[1237-9---]"));
    assert(true==r.match(T("1")));
    assert(false==r.match(T("4")));
    assert(true==r.match(T("8")));
    assert(true==r.match(T("7")));
    assert(true==r.match(T("-")));
    assert(false==r.match(T("a")));
    assert(false==r.match(T("abc")));

    //begin and end position of string test
    r.compile(T("^123$"));
    assert(true==r.match(T("123")));
    assert(false==r.match(T("12")));
    assert(false==r.match(T("23")));
    assert(false==r.match(T("\n123\n")));

    r.compile(T("^123$\\n23"));
    assert(false==r.match(T("123")));
    assert(false==r.match(T("12")));
    assert(false==r.match(T("23")));
    assert(true==r.match(T("123\n23")));


    //complex march test
    r.compile(T("(\\+|-)?[0-9]+"));
    assert(true==r.match(T("+12")));
    assert(true==r.match(T("-23")));
    assert(true==r.match(T("1234")));
    assert(false==r.match(T("123-3")));

    //=================
    //search test
    //=================
    RegexResult set;
    //single character search test
    r.compile(T("a"));
    set=r.search(T("abcadaarta"));
    assert(0==set.captureResult[0].begin);
    assert(3==set.captureResult[1].begin);
    assert(5==set.captureResult[2].begin);
    assert(6==set.captureResult[3].begin);
    //loop search test
    r.compile(T("a+"));
    set=r.search(T("aaaabbaaaby"));
    assert(0==set.captureResult[0].begin);
    assert(6==set.captureResult[1].begin);











    //compare with c++ regex
    int n=10000;
    clock_t s,e;

    s=clock();
    std::regex re1("finance\\.sina\\.cn|stock1\\.sina\\.cn|3g\\.sina\\.com\\.cn.*(channel=finance|_finance$|ch=stock|/stock/)|dp\\.sina\\.cn/.*ch=9&");
    std::cmatch cmat;
    std::smatch smat;
    for(int i=0;i<n;++i)
    {
        //std::regex re1("(\\+|-)?[0-9]+");
        std::regex_search("finance.sina.cn/google.com/baidu.com.google.sina.cn",re1);
        std::regex_search("3g.com.sina.cn.google.com.baidu.com.sina.egooooooooo",re1);
        std::regex_search("http://3g.sina.com.cn/google.baiduchannel=financegogo",re1);
    }
    e=clock();
    std::cout<<"case1:\ntime:\n"<<e-s<<std::endl;

    s=clock();
    TinyRegex re2(T("finance\\.sina\\.cn|stock1\\.sina\\.cn|3g\\.sina\\.com\\.cn.*(channel=finance|_finance$|ch=stock|/stock/)|dp\\.sina\\.cn/.*ch=9&"));

    for(int i=0;i<n;++i)
    {
        //TinyRegex re2(T("(\\+|-)?[0-9]+"));
        re2.search(T("finance.sina.cn/google.com/baidu.com.google.sina.cn"));
        re2.search(T("3g.com.sina.cn.google.com.baidu.com.sina.egooooooooo"));
        re2.search(T("http://3g.sina.com.cn/google.baiduchannel=financegogo"));
    }
    e=clock();
    std::cout<<"case2:\ntime:\n"<<e-s<<std::endl;


    std::cout<<"Pass all tests."<<std::endl;
    return 0;
}




