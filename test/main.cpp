﻿#include <iostream>
#include"tinyregex.h"
#include<cassert>

#include<ctime>
#include<regex>


using namespace std;
using namespace tyre;

int main(int argc, char *argv[])
{
    TinyRegex r;
    //============
    //match test
    //============

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
    assert(false==r.match(T("123\n23")));


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
    assert(0==set.subMatch[0].begin);
    assert(0==set.subMatch[0].end);
    assert(3==set.subMatch[1].begin);
    assert(3==set.subMatch[1].end);
    assert(5==set.subMatch[2].begin);
    assert(5==set.subMatch[2].end);
    assert(6==set.subMatch[3].begin);
    assert(6==set.subMatch[3].end);
    //number cahracter search test
    r.compile(T("1"));
    set=r.search(T("12341234123"));
    assert(0==set.subMatch[0].begin);
    assert(0==set.subMatch[0].end);
    assert(4==set.subMatch[1].begin);
    assert(4==set.subMatch[1].end);
    assert(8==set.subMatch[2].begin);
    assert(8==set.subMatch[2].end);
    //loop search test
    r.compile(T("a+"));
    set=r.search(T("aaaabbaaaby"));
    assert(0==set.subMatch[0].begin);
    assert(3==set.subMatch[0].end);
    assert(6==set.subMatch[1].begin);
    assert(8==set.subMatch[1].end);
    //lazy search test
    r.compile(T("c.+?d"));
    set=r.search(T("cadcad"));
    assert(0==set.subMatch[0].begin);
    assert(2==set.subMatch[0].end);
    assert(3==set.subMatch[1].begin);
    assert(5==set.subMatch[1].end);
    //inner loop search test
    r.compile(T("c.+d"));
    set=r.search(T("caaaadcaaaad"));
    assert(0==set.subMatch[0].begin);
    assert(11==set.subMatch[0].end);

    //bracket search test
    r.compile(T("c(.+)"));
    set=r.search(T("cade"));
    assert(0==set.subMatch[0].begin);
    assert(3==set.subMatch[0].end);
    assert(1==set.subMatch[0].captureResult[0].begin);
    assert(3==set.subMatch[0].captureResult[0].end);

    //nocapture test
    r.compile(T("c(?:.+)"));
    set=r.search(T("cade"));
    assert(0==set.subMatch[0].begin);
    assert(3==set.subMatch[0].end);
    assert(true==set.subMatch[0].captureResult.empty());

    //capture ref test
    r.compile(T("c(.+)\\k0"));
    set=r.search(T("cadeade"));
    assert(0==set.subMatch[0].begin);
    assert(6==set.subMatch[0].end);
    set=r.search(T("cade"));
    assert(true==set.subMatch.empty());

    r.compile(T("c(.*)abc\\k0"));
    set=r.search(T("c123abc123"));
    assert(0==set.subMatch[0].begin);
    assert(9==set.subMatch[0].end);
    assert(1==set.subMatch[0].captureResult[0].begin);
    assert(3==set.subMatch[0].captureResult[0].end);

    r.compile(T("c(.*)abc\\k0"));
    assert(true==r.match(T("c123abc123")));
    assert(false==r.match(T("c1234")));
    assert(false==r.match(T("c23abc24")));
    assert(true==r.match(T("cabcabcabc")));


    //named capture ref test
    r.compile(T("c(?<anything>.+)\\k\'anything\'"));
    set=r.search(T("cadeade"));
    assert(0==set.subMatch[0].begin);
    assert(6==set.subMatch[0].end);

    //posix character test
    r.compile(T("1[:digit:]2"));
    assert(true==r.match(T("122")));
    assert(true==r.match(T("192")));
    assert(true==r.match(T("102")));
    assert(false==r.match(T("1a2")));


    //=========================
    //compile option test
    //=========================

    //nosubs
    r.compile(T("(abc)"),SyntaxFlag::NOSUBS);
    set=r.search(T("abcdabc"));
    assert(0==set.subMatch[0].mark_count());

    //multiline
    r.compile(T("^123$\\n23"),SyntaxFlag::MULTILINE);
    assert(true==r.match(T("123\n23")));

    //icase
    r.compile(T("abc1Qty"),SyntaxFlag::ICASE);
    assert(true==r.match(T("ABC1qTy")));
    assert(false==r.match(T("ABC2qty")));

    //=========================
    //replace test
    //=========================
    string_t afterStr;
    r.compile(T("abc"));
    afterStr=r.replace(T("abcdabc"),T("e"));
    //std::wcout<<afterStr<<std::endl;
    assert(afterStr==T("ede"));

    r.compile(T("a+"));
    afterStr=r.replace(T("aaaaac"),T("e"));
    //std::wcout<<afterStr<<std::endl;
    assert(afterStr==T("ec"));



    //compare with c++ regex
    int n=1000;
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




