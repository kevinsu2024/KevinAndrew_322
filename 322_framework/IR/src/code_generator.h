#pragma once

#include <IR.h>
#include <trace.h>
#include <iostream>
#include <fstream>
#include <multidim_array.h>
#include <unordered_map>
#include <algorithm>
// #include <tile.h>
#include <stack>
#include<set>
// #include <liveness_generator.h>
// #include <algorithm>

namespace IR{

    typedef std::unordered_map<std::string, Data_holder*> var_map;

    void generate_code(Program p, bool verbose);
    // void generate_call(Instruction* instr, std::ofstream& out);
    // void generate_call_assignment(Instruction* instr, std::ofstream& out);
    // Program convert_all_labels(Program p, std::string longest_label);
    // std::string generate_global_label(Program p);
}