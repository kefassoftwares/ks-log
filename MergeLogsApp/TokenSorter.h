#pragma once
#include <iostream>
#include <list>
#include <memory>
#include "LogToken.h"
#include "Tokenizer.h"

namespace KMergeLogs {
    
    class TokenSorter {
    public:
        typedef std::vector<Tokenizer> TokenizerVector;
        typedef std::pair<LogToken, Tokenizer> LogTokenizerPair;
        TokenSorter(const TokenizerVector& streams_);
        LogToken getNextToken();
        operator bool() const
        {
            return !_sortedTokenQueue.empty();
        }
    private:
        void insertNextToken(Tokenizer& tokenizer_);

        std::list<LogTokenizerPair>       _sortedTokenQueue;
        TokenizerVector                   _streams;
    };
    
}
