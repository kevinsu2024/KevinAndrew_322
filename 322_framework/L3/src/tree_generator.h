#pragma once

#include <L3.h>

namespace L3{
    class Context{
        public:
        bool isContext;
        std::string func_name;
        std::vector<Instruction*> instructions;
    };
    bool not_label_call(Instruction* i);
    std::vector<Context*> generate_contexts(Program p);
    void generate_tree(std::vector<Context*> contexts);

}