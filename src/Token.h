#pragma once
#include <string>
#include <thread>
#include <sstream>

#include "Utils.h"
namespace klog {

    class Token
    {
    public:

        Token(const std::string& tokenStr_)
        {
            if (tokenStr_.length() == 2 && tokenStr_[0] == '%')
            {
                findTokenType(tokenStr_[1]);
                return;
            }
            
            _type = TokenType::STRING;
            _content = tokenStr_;
                
        }

        TokenType getType() const { return _type; }
        std::string getContent() const { return _content; }

    private:

        void findTokenType(char c)
        {
            switch (c)
            {
            case 'f':
                _type = TokenType::FILE_NAME;
                break;
            case 'l':
                _type = TokenType::LINE_NUM;
                break;
            case 't':
                _type = TokenType::THREAD_NUM;
                break;
            case 'd':
                _type = TokenType::DATE_TIME;
                break;
            case 's':
                _type = TokenType::SEVERITY;
                break;
            case 'm':
                _type = TokenType::MESSAGE;
                break;
            default:
                _type = TokenType::STRING;
                _content = std::string("%") + c;
            }
        }

        std::string     _content;
        TokenType       _type;
    };


}