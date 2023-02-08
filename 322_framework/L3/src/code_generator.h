#pragma once

#include <L3.h>
#include <tree_generator.h>
#include <iostream>
#include <fstream>
#include <tile.h>

namespace L3{
    void generate_code(Program p);
    void generate_call(Instruction* instr, std::ofstream& out);
    void generate_call_assignment(Instruction* instr, std::ofstream& out);
    Program convert_all_labels(Program p, std::string longest_label);
    std::string generate_global_label(Program p);
}