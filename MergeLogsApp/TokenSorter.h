#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "LogToken.h"
#include "Tokenizer.h"

namespace KMergeLogs {
    
    class TokenSorter {
    public:
        typedef std::vector<Tokenizer> TokenizerVector;
        TokenSorter(const TokenizerVector& streams_);
        LogToken getNextToken();
        operator bool() const
        {
            return !_tokenQueue.empty();
        }
    private:
        std::vector<LogToken>       _tokenQueue;
        TokenizerVector             _streams;
    };
    
}
