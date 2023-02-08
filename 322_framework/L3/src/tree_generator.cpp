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
        
        //make new contexts
        for (auto f : functions){
            Context* ctx = new Context();
            ctx->func_name = f->name;
            ctx->isContext = true;
            for (auto instr : f->instructions){
                if(instr == nullptr){
                    break;
                }
                if (!is_label_call(instr)){
                    ctx->instructions.push_back(instr);
                } else{
                    contexts.push_back(ctx);

                    Context* con = new Context();
                    con->isContext = false;
                    con->func_name = f->name;
                    con->instructions.push_back(instr);
                    contexts.push_back(con);

                    ctx = new Context();
                    ctx->func_name = f->name;
                    ctx->isContext = true;
                }
                if (is_branch_return(instr)){
                    contexts.push_back(ctx);
                    
                    ctx = new Context();
                    ctx->isContext = true;
                    ctx->func_name = f->name;
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
