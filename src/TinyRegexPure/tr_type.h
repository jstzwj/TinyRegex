#pragma once
#include<cctype>
#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<stack>
#include<deque>
#include<unordered_map>

#define TINYREGEX_UNICODE
namespace tinyregex
{
#ifdef TINYREGEX_UNICODE
	typedef wchar_t char_t;
	typedef std::wstring string_t;
	typedef std::wistream istream_t;
	typedef std::wostream ostream_t;
	typedef std::wstringstream stringstream_t;
	#define CHARMAX 65536
	#define T(X) L##X
#else
	typedef char char_t;
	typedef std::string string_t;
	typedef std::istream istream_t;
	typedef std::ostream ostream_t;
	typedef std::stringstream stringstream_t;
	#define CHARMAX 255
	#define T(X) X
#endif // TINYREGEX_UNICODE
	typedef unsigned char byte;
	typedef std::unordered_map<int,std::unordered_map<int,int> > StateTable;

	class TextPos
	{
	public:
		//开始位置的行数
		int row;
		//开始位置的列数
		int col;
	};

	class StringPos
	{
	public:
		//在原字符串中开始的位置
		int start;
		//在源字符串中结束的位置
		int end;
	};




	enum _Meta_type
	{	// meta character representations for parser
		_Meta_lpar = '(',
		_Meta_rpar = ')',
		_Meta_dlr = '$',
		_Meta_caret = '^',
		_Meta_dot = '.',
		_Meta_star = '*',
		_Meta_plus = '+',
		_Meta_query = '?',
		_Meta_lsq = '[',
		_Meta_rsq = ']',
		_Meta_bar = '|',
		_Meta_esc = '\\',
		_Meta_dash = '-',
		_Meta_lbr = '{',
		_Meta_rbr = '}',
		_Meta_comma = ',',
		_Meta_colon = ':',
		_Meta_equal = '=',
		_Meta_exc = '!',
		_Meta_eos = -1,
		_Meta_nl = '\n',
		_Meta_cr = '\r',
		_Meta_bsp = '\b',
		_Meta_chr = 0,

		_Esc_bsl = '\\',
		_Esc_word = 'b',
		_Esc_not_word = 'B',
		_Esc_ctrl_a = 'a',
		_Esc_ctrl_b = 'b',
		_Esc_ctrl_f = 'f',
		_Esc_ctrl_n = 'n',
		_Esc_ctrl_r = 'r',
		_Esc_ctrl_t = 't',
		_Esc_ctrl_v = 'v',
		_Esc_ctrl = 'c',
		_Esc_hex = 'x',
		_Esc_uni = 'u'
	};

	// constants used in regular expressions
	enum syntax_option_type
	{	// specify RE syntax rules
		ECMAScript = 0x01,
		basic = 0x02,
		extended = 0x04,
		awk = 0x08,
		grep = 0x10,
		egrep = 0x20,
		_Gmask = 0x3F,

		icase = 0x0100,
		nosubs = 0x0200,
		optimize = 0x0400,
		collate = 0x0800
	};


	enum match_flag_type
	{	// specify matching and formatting rules
		match_default = 0x0000,
		match_not_bol = 0x0001,
		match_not_eol = 0x0002,
		match_not_bow = 0x0004,
		match_not_eow = 0x0008,
		match_any = 0x0010,
		match_not_null = 0x0020,
		match_continuous = 0x0040,
		match_prev_avail = 0x0100,
		format_default = 0x0000,
		format_sed = 0x0400,
		format_no_copy = 0x0800,
		format_first_only = 0x1000,
		_Match_not_null = 0x2000
	};


	enum error_type
	{	// identify error
		error_collate,
		error_ctype,
		error_escape,
		error_backref,
		error_brack,
		error_paren,
		error_brace,
		error_badbrace,
		error_range,
		error_space,
		error_badrepeat,
		error_complexity,
		error_stack,
		error_parse,
		error_syntax
	};
	// namespace regex_constants


}