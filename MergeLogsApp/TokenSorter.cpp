#include "TokenSorter.h"

namespace KMergeLogs {

    

    TokenSorter::TokenSorter(const TokenExtrVector& streams_, const std::string& dateFormat_)
        :_streams(streams_)
    {
        
    }

    LogToken TokenSorter::getNextToken()
    {

        return LogToken();
    }

}