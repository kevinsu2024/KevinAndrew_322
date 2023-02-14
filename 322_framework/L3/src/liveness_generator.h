#pragma once

#include <L3.h>
#include <set>
#include <string>
#include<iostream>



namespace L3{
    struct Liveness_Node{
        Instruction* instr;
        std::set<int64_t> preds;
        std::set<int64_t> succs;
        std::set<std::string> gen;
        std::set<std::string> kill;
        std::set<std::string> in;
        std::set<std::string> out;
    };

    std::vector<Liveness_Node*> generate_liveness(Function* func);
}