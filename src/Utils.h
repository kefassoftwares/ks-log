#pragma once
#include <map>
namespace klog {

    enum class Severity
    {
        INFO = 0,
        DEBUG = 1,
        WARNING = 2,
        ERROR = 3,
        CRITICAL = 4
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