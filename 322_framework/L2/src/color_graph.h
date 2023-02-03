#pragma once

#include <L2.h>
#include <unordered_map>
#include <set>

#include <string>


namespace L2{
    std::pair<std::unordered_map<std::string,std::string>, Function*>
    generate_mapping(Function* f);

    std::string 
    translate_to_code(std::unordered_map<std::string, std::string> map, Function* f);
}