#include <tree_generator.h>


namespace L3{

    bool
    is_label_call(Instruction* i){
        return  (   i->get_name() == "Instruction_label"            ||
                    i->get_name() == "Instruction_call_assignment"  ||
                    i->get_name() == "Instruction_call"             
                );
    }
    bool
    is_branch_return(Instruction* i ){
        return  (   i->get_name() == "Instruction_return"           ||
                    i->get_name() == "Instruction_return_t"         ||
                    i->get_name() == "Instruction_branch"           ||
                    i->get_name() == "Instruction_branch_t"         
                );
    }

    std::vector<Context*>
    generate_contexts(Program p){
        std::vector<Function*> functions = p.functions;
        std::vector<Context*> contexts;
        Context* ctx = new Context();
        //make new contexts
        for (auto f : functions){
            for (auto instr : f->instructions){
                if(instr == nullptr){
                    break;
                }
                if (!is_label_call(instr)){
                    ctx->instructions.push_back(instr);
                }
                if (is_label_call(instr) && is_branch_return(instr)){
                    contexts.push_back(ctx);
                    ctx = new Context();
                }
            }
        }
        //delete empty contexts
        for (int64_t i = 0; i < contexts.size(); i++){
            if (contexts[i]->instructions.size() == 0){
                contexts.erase(contexts.begin() + i);
            }
        }
        return contexts;
    }
}
