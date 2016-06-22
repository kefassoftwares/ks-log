#include "RealtimeTokenParser.h"
#include <sstream>
#include <vector>

#if defined(__unix__) || defined(__linux__) 
#include <sys/time.h>
#elif defined(_WIN32)
#include <Windows.h>
#endif

namespace klog {

    std::string RealtimeTokenParser::getSimpleTime() const
    {
        char timeArray[50];
        std::ostringstream timeStream;
    
#if defined(__unix__) || defined(__linux__)
        timeval tv;
        gettimeofday(&tv, 0);
        std::strftime(timeArray, sizeof(timeArray), "%Y-%b-%d %H:%M:%S.", std::localtime(&tv.tv_sec));
        timeStream << timeArray << tv.tv_usec;
#elif defined(_WIN32)
        FILETIME ft;
        SYSTEMTIME st;
        GetSystemTimeAsFileTime(&ft);
        FileTimeToSystemTime(&ft, &st);
        
        unsigned long long int t = ft.dwHighDateTime;
        t <<= 32;
        t |= ft.dwLowDateTime;

        t /= 10;
        t -= 11644473600000000;

        std::vector<std::string> months = {"Jan", "Feb", "Mar", "Jun", "Jul", "Aug", "Sep", "Nov", "Dec"};
        timeStream  << st.wYear << "-" << months[st.wMonth] << "-" << st.wDay 
                    << " " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << t%1000000;
#endif
        return timeStream.str();

    }

    std::string RealtimeTokenParser::parse(const std::string& logMessage_, Severity logType_, const std::string& sourceFile_, long int lineNum_) const 
    {
        std::map<Severity, std::string> SeverityStringMap = {
            { Severity::S_INFO, "INFO" },
            { Severity::S_DEBUG, "DEBUG" },
            { Severity::S_WARNING, "WARNING" },
            { Severity::S_ERROR, "ERROR" },
            { Severity::S_CRITICAL, "CRITICAL" }
        };

        std::ostringstream parserStream;
        parserStream.setf(std::ios::fixed, std::ios::floatfield);

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
                parserStream << getSimpleTime();
                break;
            case TokenType::THREAD_NUM:
                parserStream << std::this_thread::get_id();
                break;
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