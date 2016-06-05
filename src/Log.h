#pragma once

#include <fstream>
#include <map>
#include <thread>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>

#include "Token.h"
#include "RealtimeTokenParser.h"

namespace klog {

    struct Log {

        typedef std::shared_ptr<std::ofstream>       StreamPtr;
        typedef std::shared_ptr<RealtimeTokenParser> ParserPtr;

        bool init(const std::string& dir_, 
            const std::string& appName_, 
            std::string formatString_ = "%f(LineNo:%l) ThreadNo:%t [%d] %s:%m",
            Severity constraint_ = Severity::INFO);

        bool init(const std::string& dir_,
            const std::string& appName_,
            Severity constraint_ = Severity::INFO);

        void write(const std::string& logMessage_, Severity logType_, const std::string& sourceFile_, long int lineNum_);

        static Log& getInstance();
      
    private:
        std::vector<Token> parseFormatString(std::string& fmtString_);
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

#define LOG(statement, type) klog::Log::getInstance().write(statement, type, CPP_SOURCE_FILE, __LINE__)
#define LOG_INFO(statement) LOG(statement, klog::Severity::INFO)
#define LOG_DEBUG(statement) LOG(statement, klog::Severity::DEBUG)
#define LOG_WARNING(statement) LOG(statement, klog::Severity::WARNING)
#define LOG_ERROR(statement) LOG(statement, klog::Severity::ERROR)
#define LOG_CRITICAL(statement) LOG(statement, klog::Severity::CRITICAL)


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
