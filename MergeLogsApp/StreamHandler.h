#pragma once
#include <iostream>
#include <queue>
#include <memory>
#include "TokenExtractor.h"

namespace KMergeLogs{

    class StreamHandler{
        typedef std::vector<TokenExtractor> TokenExtrVector;
    public:
        StreamHandler(const std::string& appDir_);
        bool writeLogToken(const LogToken& token_) {   }
        const TokenExtrVector& getExtractorStreams() { return _streams;  }
        std::string readFormatString();
    private:
        TokenExtrVector     _streams;
        const std::string&  _appDir;
    };
}
