#pragma once

#include <L3.h>
#include <iostream>

namespace L3{
    class Context{
        public:
        bool isContext;
        std::string func_name;
        std::vector<Instruction*> instructions;
        int64_t start_num;
    };
    bool not_label_call(Instruction* i);
    std::vector<Context*> generate_contexts(Program p);
    void generate_tree(std::vector<Context*> contexts);

}