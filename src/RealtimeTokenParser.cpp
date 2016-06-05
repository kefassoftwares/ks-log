#include "RealtimeTokenParser.h"
#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>

namespace klog {

    std::string RealtimeTokenParser::parse(const std::string& logMessage_, Severity logType_, const std::string& sourceFile_, long int lineNum_) const 
    {
        std::map<Severity, std::string> SeverityStringMap = {
            { Severity::INFO, "INFO" },
            { Severity::DEBUG, "DEBUG" },
            { Severity::WARNING, "WARNING" },
            { Severity::ERROR, "ERROR" },
            { Severity::CRITICAL, "CRITICAL" }
        };

        std::ostringstream parserStream;
        parserStream.setf(std::ios::fixed, std::ios::floatfield);

        boost::posix_time::ptime now;

        for (const Token &t : _tokenList)
        {
            switch (t.getType())
            {
            case TokenType::FILE_NAME:
                parserStream << sourceFile_;
                break;
            case TokenType::LINE_NUM:
                parserStream << lineNum_;
                break;
            case TokenType::MESSAGE:
                parserStream << logMessage_;
                break;
            case TokenType::SEVERITY:
                parserStream << SeverityStringMap.find(logType_)->second;
                break;
            case TokenType::DATE_TIME:
                now = boost::posix_time::second_clock::local_time();
                parserStream << boost::posix_time::to_simple_string(now);
            case TokenType::THREAD_NUM:
                parserStream << std::this_thread::get_id();
            default:
                parserStream << t.getContent();
                break;
            }
        }
        std::cout << "Parser string:" << parserStream.str() << std::endl; 
        return parserStream.str();
    }

    RealtimeTokenParser::RealtimeTokenParser(const std::vector<Token>& tokenList_):_tokenList(tokenList_)
    {
    }

}