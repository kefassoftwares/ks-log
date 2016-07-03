#include "StreamHandler.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

namespace KMergeLogs {

    StreamHandler::StreamHandler(const std::string & appDir_)
    {
        boost::filesystem::path path(appDir_);
        if (boost::filesystem::exists(path) && boost::filesystem::is_directory(path))
        {
            
            boost::filesystem::directory_iterator end_itr;
            for (boost::filesystem::directory_iterator it(path); it != end_itr; it++)
            {
                if (boost::filesystem::is_regular_file(it->status()))
                {
                    std::cout << appDir_ << "/" << it->path().filename().string() << " file " << std::endl;
                    _streams.push_back(std::shared_ptr<std::ifstream>(new std::ifstream(appDir_ + "/" + it->path().filename().string())));
                }
            }
        }
    }

}