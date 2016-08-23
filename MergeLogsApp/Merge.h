#pragma once
#include <iostream>
#include "StreamHandler.h"

namespace KMergeLogs{
    
    class Merge {
    public:
        Merge(const std::string& logDir_);
        void start();    
    private:
        const std::string& _logDir;
    };
    
}
