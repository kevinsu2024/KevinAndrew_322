#pragma once

#include <L2.h>
#include <set>
#include <string>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <memory>
#include <algorithm>

namespace L2{
    class Node{
        public:
            Node(Instruction* in);
            Instruction* instr;
            std::set<int64_t> preds;
            std::set<int64_t> succs;
            std::set<std::string> gen;
            std::set<std::string> kill;
            std::set<std::string> in;
            std::set<std::string> out;
    };

    std::vector<Node*> generate_liveness(Program p, bool print_std);

}