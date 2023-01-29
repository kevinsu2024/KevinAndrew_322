#include <L2.h>

namespace L2{
    void dump_spill(Function* func);
    Function* generate_spill_code(Function* func, std::string var, std::string str);
    
}