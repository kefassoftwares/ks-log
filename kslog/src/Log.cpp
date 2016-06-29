#include <sstream>
#include <ctime>

#include "Log.h"

namespace klog {

    /*
     * initialization function. This function should be called initially in user app to use the library
     * formatting:
     * %f - File Name
     * %l - Line Number
     * %t - Thread Number
     * %d - date and time
     * %s - Severity
     * %m - Message
     */
    bool Log::init(const std::string& dir_, const std::string& appName_, const std::string& formatString_, Severity constraint_)
    {
        
        std::vector<Token> tokenList = parseFormatString(formatString_);
        _parser.reset(new RealtimeTokenParser(tokenList));

        _constraint = constraint_;
            
        char timeArray[50];

        std::time_t now = std::time(NULL);
        std::strftime(timeArray, sizeof(timeArray), "%Y%m%dT%H%M%S", std::localtime(&now));

        _workDir = dir_ + "/" + appName_ + "_" + std::string(timeArray);
        
#if defined(__unix__) || defined(__linux__)
        const int dirError = mkdir(_workDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (-1 == dirError)
        {
            return false;
        }
#elif defined(_WIN32)
        if (!CreateDirectory(_workDir.c_str(), NULL))
        {
            return false;
        }
#endif
        _isInitialized = true;
        return true;
    }
    /*
    * Gives developers facility to specify severity constraint without specifying format
    */
    bool Log::init(const std::string& dir_, const std::string& appName_, Severity constraint_)
    {
        return init(dir_, appName_, "%f(LineNo:%l) ThreadNo:%t [%d] %s:%m", constraint_);
    }

    /*
     * interface to write a log entry
    */
    void Log::write(const std::string & logMessage_, Severity logType_, const std::string & sourceFile_, long int lineNum_)
    {
        if (!_isInitialized)
        {
            std::cout << "Writing failed. init not called." << std::endl;
            return;
        }

        if (logType_ < _constraint)
            return;
        
        getStream() << _parser->parse(logMessage_, logType_, sourceFile_, lineNum_);
    }

    /*
     * just a parser
    */
    std::vector<Token> Log::parseFormatString(const std::string& fmtString_)
    {
        std::string fmtString = fmtString_;
        std::vector<Token> tokenList;
        while (true)
        {
            unsigned long loc = fmtString.find_first_of('%');
            if (loc == std::string::npos)
            {
                if(fmtString.length() != 0)
                    tokenList.push_back(Token(fmtString));
                break;
            }

            if (loc > 0)
            {
                tokenList.push_back(fmtString.substr(0, loc));
            }

            if (loc < (fmtString.length() - 1))
            {
                char fmtChar = fmtString[loc + 1];
                tokenList.push_back(Token(std::string("%") + fmtChar));
                fmtString.erase(0, loc + 2);
            }
            else
            {
                tokenList.push_back(Token("%"));
                break;
            }
        }

        return tokenList;
    }
    /*
     * returns current thread's stream, creates one if not exists
    */
    std::ofstream& Log::getStream()
    {
        std::thread::id threadId = std::this_thread::get_id();
        auto it = _streamMap.find(threadId);
        if (it == _streamMap.end())
        {
            std::string threadNum = "";
            std::ostringstream stream;
            stream << threadId;
            StreamPtr threadFileStream(new std::ofstream(_workDir + "/" + stream.str() + ".txt"));
            _streamMap.insert(std::make_pair(threadId, threadFileStream));
            return *threadFileStream;
        }
        return *it->second;
    }
    /*
     * This is to prevent creation of log object every time
     */
    Log& Log::getInstance()
    {
        static Log log;
        return log;
    }

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
