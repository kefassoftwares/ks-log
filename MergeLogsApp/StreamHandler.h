#pragma once
#include <iostream>
#include <queue>
#include <memory>
#include "Tokenizer.h"
#include "FormatAnalyzer.h"

namespace KMergeLogs{

    class StreamHandler{
        typedef std::vector<Tokenizer> TokenizerVector;
    public:
        StreamHandler(const std::string& appDir_);
        bool writeLogToken(const LogToken& token_) { std::cout << token_.getContent() << std::endl; (*_oStreamPtr) << token_.getContent() << std::endl; return true; }
        TokenizerVector& getExtractorStreams() { return _streams;  }
    private:
        TokenizerVector                 _streams;
        const std::string&              _appDir;
        std::shared_ptr<std::ofstream>  _oStreamPtr;
        std::string readFormatString();
    };
}
