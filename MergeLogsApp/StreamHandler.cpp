#include "StreamHandler.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

namespace KMergeLogs {

    StreamHandler::StreamHandler(const std::string & appDir_)
        :_appDir(appDir_)
    {
        boost::filesystem::path path(appDir_);
        _oStreamPtr.reset(new std::ofstream(appDir_+"/Merged.txt"));

        FormatAnalyzer analyzer(readFormatString());
        std::regex formatRegex = analyzer.getRegex();
        
        if (boost::filesystem::exists(path) && boost::filesystem::is_directory(path))
        {
            
            boost::filesystem::directory_iterator end_itr;
            for (boost::filesystem::directory_iterator it(path); it != end_itr; it++)
            {
                if (boost::filesystem::is_regular_file(it->status()))
                {
                    if (it->path().filename().string() == ".loginfo")
                    {
                        continue;
                    }

                    _streams.push_back(Tokenizer(
                                           std::shared_ptr<std::ifstream>(
                                               new std::ifstream(appDir_ + "/" + it->path().
                                                                 filename().string())),
                                           formatRegex));
                }
            }
        }
    }

    std::string StreamHandler::readFormatString()
    {
        std::ifstream is(_appDir + "/.loginfo");
        if (!is)
        {
            throw std::runtime_error("Unable to determine log format");
        }

        std::stringstream sstr;
        sstr << is.rdbuf();
        std::string logFormat = sstr.str();
        
        return logFormat;
    }


}
