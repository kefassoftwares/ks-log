#include "LogToken.h"

namespace KMergeLogs {
    LogToken::LogToken(const std::string & content_)
        :_content(content_)
        ,_type(TokenType::GOOD_TOKEN)
    {

    }

}