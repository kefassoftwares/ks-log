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
        insertNextToken(_sortedTokenQueue.front().second);
        _sortedTokenQueue.erase(_sortedTokenQueue.begin());
        return retToken;
    }

    void TokenSorter::insertNextToken(Tokenizer& tokenizer_)
    {
        

        LogToken token = tokenizer_.getNextToken();
        if (token.getType() == LogToken::TokenType::BAD_TOKEN) {
            return;
        }
        
        for (auto it = _sortedTokenQueue.begin(); it != _sortedTokenQueue.end(); it++)
        {
            if (it->first.getTime() > token.getTime())
            {
                _sortedTokenQueue.insert(it, std::make_pair(token, tokenizer_));
                return;
            }
            else
            {
                _sortedTokenQueue.insert(++it, std::make_pair(token, tokenizer_));
                return;
            }
        }

        if (_sortedTokenQueue.empty())
        {
            _sortedTokenQueue.insert(_sortedTokenQueue.begin(), std::make_pair(token, tokenizer_));
        }
    }

}
