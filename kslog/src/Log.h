#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <thread>
#include <sstream>
#include <cstring>
#include <vector>

#if defined(__unix__) || defined(__linux__) 
#include <sys/stat.h>
#include <sys/time.h>
#elif defined(_WIN32)
#include <Windows.h>
#endif

namespace klog {

    //the s_ is used to avoid conflict with macros in windows
    enum class Severity
    {
        S_INFO = 0,
        S_DEBUG = 1,
        S_WARNING = 2,
        S_ERROR = 3,
        S_CRITICAL = 4
    };

    enum class TokenType
    {
        FILE_NAME = 0,
        LINE_NUM = 1,
        THREAD_NUM = 2,
        DATE_TIME = 3,
        SEVERITY = 4,
        MESSAGE = 5,
        STRING = 6
    };


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

    class RealtimeTokenParser
    {
    public:
        RealtimeTokenParser(const std::vector<Token>& tokenList_);
        std::string parse(const std::string& logMessage_, Severity logType_, const std::string& sourceFile_, long int lineNum_) const;
    private:
        const std::vector<Token> _tokenList;
        std::string getSimpleTime() const;
    };

    struct Log {

        typedef std::shared_ptr<std::ofstream>       StreamPtr;
        typedef std::shared_ptr<RealtimeTokenParser> ParserPtr;

        bool init(const std::string& dir_, 
            const std::string& appName_, 
            const std::string& formatString_,
            Severity constraint_ = Severity::S_INFO);

        bool init(const std::string& dir_,
            const std::string& appName_,
            Severity constraint_ = Severity::S_INFO);

        void write(const std::string& logMessage_, Severity logType_, const std::string& sourceFile_, long int lineNum_);

        static Log& getInstance();
      
    private:
        std::vector<Token> parseFormatString(const std::string& fmtString_);
        std::ofstream& getStream();
        Log() = default;

        bool                                            _isInitialized = false;
        std::string                                     _workDir;
        std::map<std::thread::id, StreamPtr>            _streamMap;
        ParserPtr                                       _parser;
        Severity                                        _constraint;
    };
}

#define CPP_SOURCE_FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? \
strrchr(__FILE__, '\\') + 1 : __FILE__)

#define LOG(statement, type) std::ostringstream s; s << statement; klog::Log::getInstance().write(s.str(), type, CPP_SOURCE_FILE, __LINE__)
#define LOG_INFO(statement) LOG(statement, klog::Severity::S_INFO)
#define LOG_DEBUG(statement) LOG(statement, klog::Severity::S_DEBUG)
#define LOG_WARNING(statement) LOG(statement, klog::Severity::S_WARNING)
#define LOG_ERROR(statement) LOG(statement, klog::Severity::S_ERROR)
#define LOG_CRITICAL(statement) LOG(statement, klog::Severity::S_CRITICAL)


//add 1. formatting
//    2. level of detail

/*
 formatting:
 FileName(LineNo:XX) ThreadNo:XX [Date Time] Log_Level: Msg
 "%f(LineNo:%l) %t [%d] %s:%m"
 %f - File Name
 %l - Line Number 
 %t - Thread Number
 %d - date and time
 %s - Severity
 %m - Message

 Levels:
 Info       - 0
 Debug      - 1
 Warning    - 2
 Error      - 3
 Critical   - 4
 */
