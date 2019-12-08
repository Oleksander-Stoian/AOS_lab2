#pragma once
#include <string>
enum class LineType {Empty, Footer, Header, Line};
class Lexer{
//class analyzes string line, allocates fields
public:
    LineType load(const std::string & );
    bool next(std::string &res);
    bool eof()const noexcept;
private:
    static const std::string delim;
    std::string s;
    size_t pos = 0;
    bool eof_ = true;
    LineType type = LineType::Empty;
};
