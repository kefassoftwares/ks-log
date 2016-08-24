#include "TokenSorter.h"

namespace KMergeLogs {

    

    TokenSorter::TokenSorter(const TokenizerVector& streams_)
        :_streams(streams_)
    {
        for (auto stream : streams_)
        {
            insertNextToken(stream);
        }
    }

    LogToken TokenSorter::getNextToken()
    {
        if (_sortedTokenQueue.empty()) return LogToken();

        LogToken retToken = _sortedTokenQueue.front().first;
        Tokenizer temp = _sortedTokenQueue.front().second;
        _sortedTokenQueue.erase(_sortedTokenQueue.begin());
        insertNextToken(temp);
        return retToken;
    }

    void TokenSorter::insertNextToken(Tokenizer& tokenizer_)
    {
        LogToken token = tokenizer_.getNextToken();
        if (token.getType() == LogToken::TokenType::BAD_TOKEN) {
            return;
        }
        
        bool isInserted = false;
        for (auto it = _sortedTokenQueue.begin(); it != _sortedTokenQueue.end(); it++)
        {
            if (it->first.getTime() > token.getTime())
            {
                isInserted = true;
                _sortedTokenQueue.insert(it, std::make_pair(token, tokenizer_));
                return;
            }
        }

        if (!isInserted)
        {
            _sortedTokenQueue.insert(_sortedTokenQueue.end(), std::make_pair(token, tokenizer_));
        }
    }

}
