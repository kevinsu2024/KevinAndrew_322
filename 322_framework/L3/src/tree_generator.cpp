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
            ctx->start_num = 0;
            contexts.push_back(ctx);
            int64_t ctr = 0;
            // std::cerr << "\nhererere\n";
            for (auto instr : f->instructions){
                // std::cerr << "\nisntruction is: " << instr->to_string() << "\n";
                // std::cerr << "ctr is " << ctr << "\n";
                if(instr == nullptr){
                    break;
                }
                if (!is_label_call(instr)){
                    // std::cerr <<"herere 1 \n";
                    contexts.back()->instructions.push_back(instr);
                    if (is_branch_return(instr)){
                        // std::cerr <<"herere 2 \n";
                        contexts.push_back(new Context());
                        contexts.back()->start_num = ctr+1;
                        contexts.back()->isContext = true;
                        contexts.back()->func_name = f->name;
                }
                } else{
                    // std::cerr <<"herere 3 \n";
                    contexts.push_back(new Context());
                    contexts.back()->start_num = ctr;
                    contexts.back()->isContext = false;
                    contexts.back()->func_name = f->name;
                    contexts.back()->instructions.push_back(instr);

                    contexts.push_back(new Context());
                    contexts.back()->start_num = ctr+1;
                    contexts.back()->isContext = true;
                    contexts.back()->func_name = f->name;

                    

                    
                }
                
                ctr++;
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
