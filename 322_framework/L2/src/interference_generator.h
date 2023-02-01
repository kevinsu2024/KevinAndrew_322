#include <liveness_generator.h>
#include <unordered_map>

namespace L2{
    std::unordered_map<std::string, std::set<std::string>> 
    generate_interference(Function* func, bool verbose);

}