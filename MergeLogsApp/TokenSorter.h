#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "LogToken.h"
#include "TokenExtractor.h"

namespace KMergeLogs {
    
    class TokenSorter {
    public:
        typedef std::vector<TokenExtractor> TokenExtrVector;
        TokenSorter(const TokenExtrVector& streams_, const std::string& dateFormat_);
        LogToken getNextToken();
        operator bool() const
        {
            return !_tokenQueue.empty();
        }
    private:
        std::vector<LogToken>       _tokenQueue;
        const TokenExtrVector&      _streams;
    };
    
}
