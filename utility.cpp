#include <cassert>
#include "utility.hpp"
#include "miniz.h"

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

std::vector<uint8_t> CompressString(const std::string& str)
{
    unsigned long outLength = compressBound(str.length()+1);
    std::vector<uint8_t> compressed;
    compressed.resize(outLength);
    
    if(compress(&compressed[0], &outLength, (const unsigned char*)str.c_str(), str.length()+1) != Z_OK)
    {
        // compression failed somehow
        assert(false);
    }
    else
    {
        compressed.resize(outLength);
    }
    
    return compressed;
}

std::string DecompressString(const std::vector<uint8_t>& bytes)
{
    std::string ret;
    unsigned long  deflateSize = bytes.size() * 10;
    ret.resize(deflateSize);
    uncompress((unsigned char*)&ret[0], &deflateSize, &bytes[0], bytes.size());
    
    return ret;
}
