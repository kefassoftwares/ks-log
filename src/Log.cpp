#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>

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
    bool Log::init(const std::string& dir_, const std::string& appName_, std::string formatString_, Severity constraint_)
    {
        std::vector<Token> tokenList = parseFormatString(formatString_);
        _parser.reset(new RealtimeTokenParser(tokenList));

        _constraint = constraint_;

        boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
        _workDir = dir_ + "/" + appName_ + "_" + boost::posix_time::to_iso_string(now);
        boost::filesystem::path dir(_workDir);
        if (!boost::filesystem::create_directory(dir))
        {
            return false;
        }

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
    std::vector<Token> Log::parseFormatString(std::string& fmtString_)
    {
        std::vector<Token> tokenList;
        while (true)
        {
            unsigned loc = fmtString_.find_first_of('%');
            if (loc == std::string::npos)
            {
                if(fmtString_.length() != 0)
                    tokenList.push_back(Token(fmtString_));
                break;
            }

            if (loc > 0)
            {
                tokenList.push_back(fmtString_.substr(0, loc));
            }

            if (loc < (fmtString_.length() - 1))
            {
                char fmtChar = fmtString_[loc + 1];
                tokenList.push_back(Token(std::string("%") + fmtChar));
                fmtString_.erase(0, loc + 2);
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
