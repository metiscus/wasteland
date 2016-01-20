#include "utility.hpp"

std::vector<std::string> TokenizeString(const std::string& str, char delim)
{
    std::vector<std::string> ret;

    if(str.length() > 0)
    {
        std::string token;
        for(size_t itr=0; itr<str.length(); )
        {
            size_t nitr = str.find(delim, itr);
            token = str.substr(itr, nitr-itr);
            ret.push_back(token);
            
            if(nitr == str.npos)
                break;

            itr = nitr + 1;
        }
    }    
    return ret;
}
