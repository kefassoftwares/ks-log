#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <regex>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/locale.hpp>

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
        boost::posix_time::ptime getTime() { return _time; }
    private:
        std::string getDateString();

        std::string                 _content;
        TokenType                   _type;
        boost::posix_time::ptime    _time;
    };

}
