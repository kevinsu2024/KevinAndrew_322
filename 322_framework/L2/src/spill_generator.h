#include <L2.h>
#include <set>
#include <string>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <memory>
#include <algorithm>

namespace L2{
    Function* generate_spill_code(Function* func, std::string var, std::string str);

    Instruction_mem_load* load_instr(int64_t n, std::string s);
    
}