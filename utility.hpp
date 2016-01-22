#ifndef UTILITY_HPP_INCLUDED
#define UTILITY_HPP_INCLUDED

#include <string>
#include <vector>
#include <cstdint>

std::vector<std::string> TokenizeString(const std::string& str, char delim = ',');
std::vector<uint8_t> CompressString(const std::string& str);
std::string DecompressString(const std::vector<uint8_t>& bytes);

#endif
