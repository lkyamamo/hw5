#ifndef DICTENG_H
#define DICTENG_H
#include <set>
#include <string>
//extern const std::set<std::string> dictWords; 
const std::set<std::string>& readDictWords(std::string filename);

#endif