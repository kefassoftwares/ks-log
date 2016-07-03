#pragma once
#include<iostream>
#include "StreamHandler.h"
#include "LogToken.h"

namespace KMergeLogs {
    
    class TokenSorter {
    public:
        TokenSorter(const std::string& logDir_, const std::string& dateFormat_);
    private:
        StreamHandler           _streamHandler;
       // std::vector<LogToken>   _tokenQueue;
    };

}