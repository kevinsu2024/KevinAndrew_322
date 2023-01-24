#pragma once

#include <L2.h>

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
    }

    void generate_liveness(Program p);

}