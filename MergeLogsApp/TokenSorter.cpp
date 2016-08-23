#include "TokenSorter.h"

namespace KMergeLogs {

    

    TokenSorter::TokenSorter(const TokenizerVector& streams_)
        :_streams(streams_)
    {
        _tokenQueue.push_back(LogToken(""));
    }

    LogToken TokenSorter::getNextToken()
    {
        _tokenQueue.erase(_tokenQueue.begin());
        return _streams[0].getNextToken();
    }

}
