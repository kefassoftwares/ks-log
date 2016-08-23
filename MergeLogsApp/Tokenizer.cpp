#include "Tokenizer.h"
namespace KMergeLogs {
    Tokenizer::Tokenizer(const IfstreamPtr& stream_, const std::regex& logFormatRegex_)
        :_stream(stream_)
        ,_formatRegExp(logFormatRegex_)
    {
        _stream->read(_buffer, 8192);
        _nReadBytes = _stream->gcount();
        if (_stream->eof())
        {
            _eofDetected = true;
        }
    }
    /*
    * returns the next matching log entry from the stream
    */
    LogToken Tokenizer::getNextToken()
    {
        std::string searchString(&_buffer[_bufferConsumed], _nReadBytes - _bufferConsumed);
        std::smatch match;
        
        //std::cout << "Match string:" << searchString << std::endl;

        if(std::regex_search(searchString, match, _formatRegExp))
        {
            unsigned pos = match.position();
            std::string str = match[0].str();
            std::cout << pos << ":" << str << std::endl;
            _bufferConsumed += (pos + str.length());

            return LogToken(str);
        }
        
        //no match & eof reached
        if (_eofDetected) return LogToken();
        
        refillBuffer();
        return getNextToken();
    }
    /*
    * fills the buffer if fully consumed. (No log match found in current buffer)
    */
    void Tokenizer::refillBuffer()
    {
        //1 Log size expected to be smaller than 8KB, if it exeeds that size, the system won't recognize it.
        //move buffer contents to starting position:
        for (int i = _bufferConsumed, startPos = 0; i < 8193; i++, startPos++)
        {
            _buffer[startPos] = _buffer[i];
        }
        //append the read content
        _stream->read(&_buffer[8193 - _bufferConsumed], _bufferConsumed);
        _bufferConsumed = 0;
        _nReadBytes = _stream->gcount();
    }
}
