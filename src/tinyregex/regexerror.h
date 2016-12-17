#ifndef REGEXERROR_H
#define REGEXERROR_H
namespace tyre
{
    enum ErrorCode
    {
        error_collate,   //无效的元素校对
        error_ctype,     //无效的字符类
        error_escape,    //无效的转移字符或者无效的尾置转义
        error_backref,   //无效的向后引用
        error_brack,     //方括号不匹配
        error_paren,     //小括号不匹配
        error_brace,     //大括号不匹配
        error_badbrace,     //大括号中的范围无效
        error_name,         //命名无效
        error_range,         //无效的（不合法）字符范围
        error_space,         //内存不足
        error_badrepeat,     //重复字符之前没有正则表达式（* + ?）
        error_complexity,    //太复杂了，标准库君hold不住了
        error_stack         //栈溢出
    };
    class RegexError
    {
    public:
        RegexError(ErrorCode e):error_code(e){}
        ErrorCode code(){return error_code;}
    private:
        ErrorCode error_code;
    };


}
#endif // REGEXERROR_H
