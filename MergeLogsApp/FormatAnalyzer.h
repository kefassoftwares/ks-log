#pragma once
#include <iostream>
#include <vector>
#include <regex>
#include <map>
#include <array>
#include <sstream>

namespace KMergeLogs {
    class FormatAnalyzer {
    public:
        FormatAnalyzer(const std::string& fmtString_):_logFormat(fmtString_) {}
        std::regex getRegex();
    private:
        std::string extractFormatTokens();
        std::string getToken(const std::string& str_);
        std::string optimizeForRegex(const std::string& str_);
        
        std::string _logFormat;
        std::string _firstTokenRegex = "";
    };
}
