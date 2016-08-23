#include "LogToken.h"

namespace KMergeLogs {
    LogToken::LogToken(const std::string & content_)
        :_content(content_)
        ,_type(TokenType::GOOD_TOKEN)
    {
        std::string dateStr = getDateString();
        std::istringstream is(dateStr);
        is.imbue(std::locale::classic());

        is >> boost::locale::as::ftime("%Y-%b-%e %H:%M:%S%f") >> _time;
        std::cout << _time << std::endl;
    }


    std::string LogToken::getDateString()
    {
        std::regex regExp("([0-9]{4}\\-(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)\\-[0-3]?[0-9]\\s[0-2]?[0-9]\\:[0-5]?[0-9]\\:[0-5]?[0-9]\\.[0-9]{6})");
        std::smatch match;

        if (std::regex_search(_content, match, regExp))
        {
            return match[0].str();
        }

        return "";
    }
}