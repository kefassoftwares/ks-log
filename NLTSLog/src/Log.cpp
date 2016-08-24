#include "Log.h"

namespace NLTSLog{

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
    bool Log::init(const std::string& dir_, const std::string& appName_, const std::string& formatString_, S_Severity constraint_)
    {

        _tokenList = parseFormatString(formatString_);

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
        std::ofstream of(_workDir + std::string("/.loginfo"));
        of.write(formatString_.c_str(), formatString_.length());
        of.close();

        _isInitialized = true;
        return true;
    }
    /*
    * Gives developers facility to specify severity constraint without specifying format
    */
    bool Log::init(const std::string& dir_, const std::string& appName_, S_Severity constraint_)
    {
        return init(dir_, appName_, "%f(LineNo:%l) ThreadNo:%t [%d] %s:%m", constraint_);
    }

    /*
    * just a parser
    */
    std::vector<std::shared_ptr<Token>> Log::parseFormatString(const std::string& fmtString_)
    {
        std::string fmtString = fmtString_;
        std::vector<std::shared_ptr<Token>> tokenList;
        while (true)
        {
            unsigned long loc = fmtString.find_first_of('%');
            if (loc == std::string::npos)
            {
                if (fmtString.length() != 0)
                    tokenList.push_back(getToken(fmtString));
                break;
            }

            if (loc > 0)
            {
                tokenList.push_back(getToken(fmtString.substr(0, loc)));
            }

            if (loc < (fmtString.length() - 1))
            {
                char fmtChar = fmtString[loc + 1];
                tokenList.push_back(getToken(std::string("%") + fmtChar));
                fmtString.erase(0, loc + 2);
            }
            else
            {
                tokenList.push_back(getToken("%"));
                break;
            }
        }

        return tokenList;
    }

    std::shared_ptr<Token> Log::getToken(const std::string& str_)
    {
        std::shared_ptr<Token> tokenPtr;

        if (str_.length() == 2 && str_[0] == '%')
        {
            
            switch (str_[1])
            {
            case 'f':
                tokenPtr.reset(new FileNameToken());
                break;
            case 'l':
                tokenPtr.reset(new LineNumToken());
                break;
            case 't':
                tokenPtr.reset(new ThreadNumToken());
                break;
            case 'd':
                tokenPtr.reset(new DateTimeToken());
                break;
            case 's':
                tokenPtr.reset(new SeverityToken());
                break;
            case 'm':
                tokenPtr.reset(new MessageToken());
                break;
            default:
                tokenPtr.reset(new StringToken(std::string("%") + str_[1]));
            }

            return tokenPtr;
        }

        tokenPtr.reset(new StringToken(str_));
        return tokenPtr;
    }
    /*
     * Writes the log
    */
    void Log::write(const LogData& logData_)
    {
        if (logData_.getSeverity().getLevel() <= _constraint)
            return;

        std::ostringstream ret;
        for (auto tokenPtr : _tokenList)
        {
            tokenPtr->getContent(logData_, ret);
        }

        getStream() << ret.str() << std::endl;
    }

    /*
    * returns current thread's stream, creates one if not exists
    */
    std::ofstream& Log::getStream()
    {
        std::thread::id threadId = std::this_thread::get_id();
        for (auto& streamPair : _streamMap)
        {
            if (streamPair.first == threadId)
            {
                return *streamPair.second;
            }
        }
        //thread safe stream adding
        std::string threadNum = "";
        std::ostringstream stream;
        stream << threadId;
        OfStreamPtr threadFileStream(new std::ofstream(_workDir + "/" + stream.str() + ".txt"));
        std::lock_guard<std::mutex> lock(_streamCreateMutex);
        _streamMap.push_back(std::make_pair(threadId, threadFileStream));
        return *_streamMap.back().second;
    }

    /*
    * This is to prevent creation of log object every time
    */
    Log& Log::getInstance()
    {
        static Log log;
        return log;
    }

    CriticalSeverity & Log::criticalSeverity()
    {
        static CriticalSeverity cSeverity;
        return cSeverity;
    }

    ErrorSeverity & Log::errorSeverity()
    {
        static ErrorSeverity eSeverity;
        return eSeverity;
    }

    DebugSeverity & Log::debugSeverity()
    {
        static DebugSeverity dSeverity;
        return dSeverity;
    }

    WarningSeverity & Log::warningSeverity()
    {
        static WarningSeverity wSeverity;
        return wSeverity;
    }

    InfoSeverity & Log::infoSeverity()
    {
        static InfoSeverity iSeverity;
        return iSeverity;
    }

    std::string DateTimeToken::getSimpleTime() const
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
        timeStream << st.wYear << "-" << months[st.wMonth] << "-" << handleSingleDigit(st.wDay)
            << " " << handleSingleDigit(st.wHour) << ":" << handleSingleDigit(st.wMinute) << ":" << handleSingleDigit(st.wSecond) << ".";

        std::string mSec = "000000";
        unsigned mISec = t % 1000000;
        for (int i = 0; i < 6; i++)
        {
            mSec[5 - i] = (char)(48 + (mISec % 10));
            mISec /= 10;
        }

        timeStream << mSec;
#endif
        return timeStream.str();

    }

    std::string DateTimeToken::handleSingleDigit(int digit_) const
    {
        int i = digit_ / 10;
        if (i == 0)
        {
            return std::string("0") + std::to_string(digit_);
        }

        return std::to_string(digit_);
    }
}
