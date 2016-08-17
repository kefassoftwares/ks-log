#pragma once
#include <iostream>
#include <memory>
#include <array>
#include <fstream>
#include "LogToken.h"
namespace KMergeLogs {
    class TokenExtractor {
    public:
        typedef std::shared_ptr<std::ifstream> IfstreamPtr;
        TokenExtractor(const IfstreamPtr& stream_);
        LogToken getNextToken();
    private:
        const IfstreamPtr&     _stream;
        char                   _buffer[8192];
        size_t                 _bufferLength;
    };

    TokenExtractor::TokenExtractor(const IfstreamPtr& stream_)
        :_stream(stream_)
    {
        _stream->read(_buffer, 8192);
        _bufferLength = _stream->gcount();
    }

    LogToken TokenExtractor::getNextToken()
    {
        return LogToken();
    }
}