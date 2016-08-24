#include "Merge.h"
#include "TokenSorter.h"
#include <iostream>
#include <boost/filesystem.hpp>

namespace KMergeLogs {
    
    Merge::Merge(const std::string& logDir_)
        :_logDir(logDir_)
    {
        if (!boost::filesystem::exists(logDir_)) 
        {
            throw std::runtime_error("No such folder: " + logDir_);
        }
    }

    void Merge::start()
    {
        StreamHandler handler(_logDir);
        TokenSorter ts(handler.getExtractorStreams());
        
        while (ts)
        {
            LogToken token = ts.getNextToken();
            handler.writeLogToken(token);
        }
    }
    
}

/*Algo:
1 open all streams
2 create a queue -> init it
3 while getnexttoken from queue, add next token from that stream
4 queue empty? stop.
 */
