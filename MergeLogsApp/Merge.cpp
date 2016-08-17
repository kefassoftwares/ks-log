#include "Merge.h"
#include "TokenSorter.h"
#include <iostream>

namespace KMergeLogs {
    
    Merge::Merge(const std::string& logDir_, const std::string& logFormat_)
        :_logDir(logDir_),
        _logFormat(logFormat_)
    {}

    void Merge::start()
    {
        StreamHandler handler(_logDir);
        TokenSorter ts(handler.getExtractorStreams(), _logFormat);
        
        while (ts)
        {
            LogToken token = ts.getNextToken();

        }
    }
    
}

/*Algo:
1 open all streams
2 create a queue -> init it
3 while getnexttoken from queue, add next token from that stream
4 queue empty? stop.
 */
