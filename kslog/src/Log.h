#include <memory>
#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <ctime>
#include <map>
#include <vector>
#include <cstring>
#include <mutex>
#include <list>

#if defined(__unix__) || defined(__linux__) 
#include <sys/stat.h>
#include <sys/time.h>
#elif defined(_WIN32)
#include <Windows.h>
#endif

namespace klog {

    //the s_ is used to avoid conflict with macros in windows
    enum class S_Severity
    {
        S_INFO = 0,
        S_DEBUG = 1,
        S_WARNING = 2,
        S_ERROR = 3,
        S_CRITICAL = 4
    };

    struct Severity {
        virtual std::string getSeverityString() const = 0;
        virtual S_Severity getLevel() const = 0;
    };

    struct CriticalSeverity :public Severity {
        std::string getSeverityString() const
        {
            return "CRITICAL";
        }

        S_Severity getLevel() const { return S_Severity::S_CRITICAL; }
    };

    struct DebugSeverity :public Severity {
        std::string getSeverityString() const
        {
            return "DEBUG";
        }

        S_Severity getLevel() const { return S_Severity::S_DEBUG; }
    };

    struct ErrorSeverity :public Severity {
        std::string getSeverityString() const
        {
            return "ERROR";
        }

        S_Severity getLevel() const { return S_Severity::S_ERROR; }
    };

    struct InfoSeverity :public Severity {
        std::string getSeverityString() const
        {
            return "INFO";
        }

        S_Severity getLevel() const { return S_Severity::S_INFO; }
    };

    struct WarningSeverity:public Severity {
        std::string getSeverityString() const
        {
            return "WARNING";
        }

        S_Severity getLevel() const { return S_Severity::S_WARNING; }
    };

    struct LogData {
        LogData(const std::string& fileName_, long int lineNo_,const std::string& message_, const Severity& severity_) :
            _fileName(fileName_),
            _lineNum(lineNo_),
            _message(message_),
            _severity(severity_){}
        std::string getFileName() const { return _fileName; }
        long int getLineNumber() const { return _lineNum; }
        const Severity& getSeverity() const { return _severity; }
        std::string getMessage() const { return _message; }
    private:
        std::string                 _fileName;
        long int                    _lineNum;
        const Severity             &_severity;
        std::string                 _message;
    };

    struct Token {
        virtual void getContent(const LogData& logData_, std::ostringstream& stream_) = 0;
    };

    struct FileNameToken :public Token {
        void getContent(const LogData& logData_, std::ostringstream& stream_)
        {
            stream_ << logData_.getFileName();
        }
    };
    
    struct LineNumToken :public Token {
        void getContent(const LogData& logData_, std::ostringstream& stream_) 
        {
            stream_ << logData_.getLineNumber();
        }
    };

    struct ThreadNumToken :public Token {
        void getContent(const LogData& logData_, std::ostringstream& stream_)  
        {
            stream_ << std::this_thread::get_id();
        }
    };

    struct DateTimeToken :public Token {
        void getContent(const LogData& logData_, std::ostringstream& stream_) 
        {
            stream_ << getSimpleTime();
        }

        std::string getSimpleTime() const
        {
            std::ostringstream timeStream;

#if defined(__unix__) || defined(__linux__)
            char timeArray[50];
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

            std::vector<std::string> months = { "Jan", "Feb", "Mar", "Jun", "Jul", "Aug", "Sep", "Nov", "Dec" };
            timeStream << st.wYear << "-" << months[st.wMonth] << "-" << st.wDay
                << " " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "." << t % 1000000;
#endif
            return timeStream.str();

        }
    };

    struct SeverityToken :public Token {
        void getContent(const LogData& logData_, std::ostringstream& stream_)  
        {
            stream_ << logData_.getSeverity().getSeverityString();
        }
    };

    struct MessageToken :public Token {
        void getContent(const LogData& logData_, std::ostringstream& stream_)  
        {
            stream_ << logData_.getMessage();
        }
    };

    struct StringToken :public Token {

        StringToken(const std::string& str_) :_str(str_) {}

        void getContent(const LogData& logData_, std::ostringstream& stream_)  
        {
            stream_ << _str;
        }

    private:
        std::string _str;
    };

    struct Log {

        typedef std::shared_ptr<std::ofstream> OfStreamPtr;
        typedef std::shared_ptr<std::ostringstream> StringStreamPtr;
        typedef std::pair<OfStreamPtr, StringStreamPtr> StreamsPair;

        bool init(const std::string& dir_,
            const std::string& appName_,
            const std::string& formatString_,
            S_Severity constraint_ = S_Severity::S_INFO);

        bool init(const std::string& dir_,
            const std::string& appName_,
            S_Severity constraint_ = S_Severity::S_INFO);

        void write(const LogData& logData_);
        StreamsPair& getStreams();

        static Log& getInstance();
    
        static CriticalSeverity& criticalSeverity();
        static ErrorSeverity& errorSeverity();
        static DebugSeverity& debugSeverity();
        static WarningSeverity& warningSeverity();
        static InfoSeverity& infoSeverity();

    private:
        std::vector<std::shared_ptr<Token>> parseFormatString(const std::string& fmtString_);
        std::shared_ptr<Token> getToken(const std::string& str_);
        Log() = default;

        bool                                                        _isInitialized = false;
        std::string                                                 _workDir;
        std::list<std::pair<std::thread::id, StreamsPair>>          _streamMap;
        S_Severity                                                  _constraint;
        std::vector<std::shared_ptr<Token>>                         _tokenList;
        std::mutex                                                  _streamCreateMutex;
    };

}

#define CPP_SOURCE_FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? \
strrchr(__FILE__, '\\') + 1 : __FILE__)

#define LOG(statement, type) klog::Log::getInstance().getStreams().second->str(""); *(klog::Log::getInstance().getStreams().second) << statement;\
        klog::Log::getInstance().write(klog::LogData(CPP_SOURCE_FILE, __LINE__, klog::Log::getInstance().getStreams().second->str(), type));

#define LOG_INFO(statement) LOG(statement, klog::Log::infoSeverity())
#define LOG_DEBUG(statement) LOG(statement, klog::Log::debugSeverity())
#define LOG_WARNING(statement) LOG(statement, klog::Log::warningSeverity())
#define LOG_ERROR(statement) LOG(statement, klog::Log::errorSeverity())
#define LOG_CRITICAL(statement) LOG(statement, klog::Log::criticalSeverity())


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
