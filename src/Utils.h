#pragma once
#include <map>
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
}