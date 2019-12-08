#include "lexer.h"
using std::string;
const string white_line = "\011\012\013\014\015\040";
const string Lexer::delim = white_line;

string clearWhites(const string &s);

LineType Lexer::load(const string& source){
    s = source;
    pos = 0;
    eof_ = false;
    if(s.empty()){
        type = LineType::Empty;
        pos = string::npos;}
    else{
        pos = 0;

        size_t i = s.find_first_of(delim, pos);//there possible that i == npos
        string tmp = clearWhites(s.substr(pos, i - pos));

        if(tmp == "")
            type = LineType::Empty;
        else{
            if(tmp == "footer")
                type = LineType::Footer;
            else{
                if(tmp == "header")
                    type = LineType::Header;
                else type = LineType::Line;
            }
        }
        if(type == LineType::Footer || type == LineType::Header){
        if(i == s.size() || i == string::npos){
            s = "";
            eof_ = true;
        }
        else
            pos = i + 1;
        }
    }
    return type;
}

bool Lexer::next(string &res){
    if(eof_)
        return false;
    if(s.empty()){
        res = "";
    eof_ = true;
        return true;}
    size_t i = s.find_first_of(delim, pos);
    res = clearWhites(s.substr(pos, i - pos));
    if(i != string::npos)
    {
        ++i;
        pos = i;
        return true;
    }
    else
        eof_ = true;
    return true;
}

bool Lexer::eof()const noexcept{ return eof_; }

string clearWhites(const string &s){
    size_t first = s.find_first_not_of(white_line);

    if(first == string::npos) return "";
    size_t last = s.find_last_not_of(white_line, s.size() );
    return s.substr(first, last - first + 1);
}


