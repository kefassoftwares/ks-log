#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>

namespace KMergeLogs {

    struct LogToken {
        enum class TokenType {
            GOOD_TOKEN,
            BAD_TOKEN
        };

        LogToken(const std::string& content_);
        LogToken():_content(""), _type(TokenType::BAD_TOKEN) { }
        const std::string& getContent() const { return _content; }
        TokenType getType() { return _type; }
    private:
        std::string _content;
        TokenType   _type;
    };

}
