#pragma once
#include <iostream>
#include "StreamHandler.h"

namespace KMergeLogs{
    
    class Merge {
    public:
        Merge(const std::string& logDir_, const std::string& dateFormat_);
        void start();    
    private:
        const std::string& _logDir;
        const std::string& _dateFormat;
    };
    
}
