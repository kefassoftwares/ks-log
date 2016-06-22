#include <sstream>
#include <ctime>

#if defined(__unix__) || defined(__linux__) 
#include <sys/stat.h>
#elif defined(_WIN32)
#include <Windows.h>
#endif

#include "Log.h"
#include "Utils.h"

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
}
