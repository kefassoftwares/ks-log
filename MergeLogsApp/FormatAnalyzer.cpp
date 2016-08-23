#include "FormatAnalyzer.h"
namespace KMergeLogs {
    
    std::regex FormatAnalyzer::getRegex()
    {
        return std::regex(extractFormatTokens());
    }

    std::string FormatAnalyzer::extractFormatTokens()
    {
        std::ostringstream regexStream;
        regexStream << "(";

        while (true)
        {
            unsigned long long loc = _logFormat.find_first_of('%');
            if (loc == std::string::npos)
            {
                if (_logFormat.length() != 0)
                    regexStream << getToken(_logFormat);
                break;
            }

            if (loc > 0)
            {
                regexStream << getToken(_logFormat.substr(0, loc));
            }

            if (loc < (_logFormat.length() - 1))
            {
                char fmtChar = _logFormat[loc + 1];
                regexStream << getToken(std::string("%") + fmtChar);
                _logFormat.erase(0, loc + 2);
            }
            else
            {
                regexStream << getToken("%");
                break;
            }
        }
        
        regexStream << "?)(?=((\n" << _firstTokenRegex << ")|(\n)))";
        //std::cout << "Regular Expression:" << regexStream.str() << std::endl;
        return regexStream.str();
    }
    
    std::string FormatAnalyzer::getToken(const std::string& str_)
    {
        std::string ret = "";

        if (str_.length() == 2 && str_[0] == '%')
        {
            
            switch (str_[1])
            {
            case 'f':
                ret = "([A-Za-z0-9\\-\\_\\'\\;\\]\\[\\}\\{\\+\\=\\)\\(\\&\\^\\%\\$#@!\\~\\`]+\\.(cpp|cc|h|hpp|c))";
                break;
            case 'l':
                ret = "([0-9]+)";
                break;
            case 't':
                ret = "((0x[0-9A-Fa-f]+)|[0-9]+)";
                break;
            case 'd':
                ret = "([0-9]{4}\\-(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)\\-[0-3]?[0-9]\\s[0-2]?[0-9]\\:[0-5]?[0-9]\\:[0-5]?[0-9]\\.[0-9]{6})";
                break;
            case 's':
                ret = "(INFO|DEBUG|WARNING|ERROR|CRITICAL)";
                break;
            case 'm':
                ret = "(.|\n)*";
                break;
            }
        }

        if (ret == "")
        {
            ret = optimizeForRegex(str_);
        }

        if (_firstTokenRegex == "")
        {
            _firstTokenRegex = ret;
        }

        return ret;
    }

    std::string FormatAnalyzer::optimizeForRegex(const std::string& str_)
    {
        std::array<char,19> regexChars =
            {'$', '^', '*', '(', ')', '[', ']', '{', '}', '?', '+', '.', '-', '\\', '=', '|', '!', ',', ':'};
        std::string ret = "";

        for (int i = 0; i < str_.length(); i++)
        {
            bool flag = true;
            for(int j = 0; j < regexChars.size(); j++)
            {
                if(str_[i] == regexChars[j])
                {
                    flag = false;
                    ret += std::string("\\") + str_[i];
                    break;
                }
                else if (str_[i] == ' ')
                {
                    flag = false;
                    ret += std::string("\\s");
                    break;
                }
            }
            if(flag) ret += str_[i];
        }
        //std::cout << "Reporting optimization, was" << str_ << " converted to " << ret << std::endl; 
        return ret;
    }
    
}
