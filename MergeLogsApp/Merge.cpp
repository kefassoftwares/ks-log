#include "Merge.h"
#include "TokenSorter.h"
#include <iostream>

namespace KMergeLogs {
    
    Merge::Merge(const std::string& logDir_, const std::string& dateFormat_)
        :_logDir(logDir_),
        _dateFormat(dateFormat_)
    {}

    void Merge::start()
    {
        TokenSorter ts(_logDir, _dateFormat);
        
        std::cout << "Hello" << std::endl;
    }
    
}

/*Algo:
1 open all streams
2 create a queue -> init it
3 while getnexttoken from queue, add next token from that stream
4 queue empty? stop.
 */
