#pragma once

#include <L2.h>
#include <unordered_map>
#include <set>
#include<tuple>

#include <string>


namespace L2{
    std::tuple<int64_t, std::unordered_map<std::string,std::string>, std::set<std::string>>
    assign_colors(std::unordered_map<std::string, std::set<std::string>> map, Function* f);

    std::string 
    translate_to_code(std::unordered_map<std::string, std::string> map, Function* f);
}