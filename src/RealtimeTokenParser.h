#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Utils.h"
#include "Token.h"
namespace klog {

    class RealtimeTokenParser
    {
    public:
        RealtimeTokenParser(const std::vector<Token>& tokenList_);
        std::string parse(const std::string& logMessage_, Severity logType_, const std::string& sourceFile_, long int lineNum_) const;
    private:
        const std::vector<Token> _tokenList;
        std::string getSimpleTime() const;
    };

}