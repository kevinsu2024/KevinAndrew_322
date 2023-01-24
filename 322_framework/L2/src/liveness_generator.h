#pragma once

#include <L2.h>

namespace L2{
    class Node{
        public:
            void Node(Instruction* in);
            std::set<int64_t> preds;
            std::set<int64_t> succs;
            std::set<std::string> gen;
            std::set<std::string> kill;
    }

    void generate_liveness(Program p);

}