#pragma once
#include <iostream>
#include <queue>
#include <memory>

namespace KMergeLogs{

    class StreamHandler{
    public:
        StreamHandler(const std::string& appDir_);
    private:
        std::vector<std::shared_ptr<std::ifstream>>  _streams;
    };
}
