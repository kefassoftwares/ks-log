#pragma once
#include <iostream>
#include <memory>
#include <array>
#include <fstream>
#include <vector>
#include <map>
#include <regex>

#include "LogToken.h"

namespace KMergeLogs {
    class Tokenizer {
    public:
        typedef std::shared_ptr<std::ifstream> IfstreamPtr;
        Tokenizer(const IfstreamPtr& stream_, const std::regex& logFormatRegex_);
        LogToken getNextToken();
    private:
        void refillBuffer();

        IfstreamPtr                 _stream;
        char                        _buffer[8193];
        std::streamsize             _nReadBytes;
        std::streamsize             _bufferConsumed = 0;
        std::regex                  _formatRegExp;
        bool                        _eofDetected = false;
    };

}
