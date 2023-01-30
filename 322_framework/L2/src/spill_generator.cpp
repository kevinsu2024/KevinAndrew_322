#include <spill_generator.h>

using namespace std;

namespace L2{
    Instruction_mem_load*
    load_instr(int64_t ctr, std::string var){
        Item* reg = new Register(RegisterID::rsp);
        Item* num = new InstructionNumber(std::to_string(ctr * 8));
        Item* dest = new Variable(var);
        Instruction_mem_load* instr = new Instruction_mem_load(dest,reg,num);
        return instr;
    }

    Function*
    generate_spill_code(Function* f, std::string v, std::string new_v){
        int64_t ctr = 0;
        Function new_func;
        new_func.name = f->name;
        new_func.arguments = f->arguments;

        for(Instruction* instr : f->instructions){
            if(instr->to_string().find(v) == std::string::npos) {
                new_func.instructions.push_back(instr);
                continue;
            }
            std::string instr_type = instr->get_name();
            if(instr_type == "Instruction_cjump"){
                Instruction_cjump* i = (Instruction_cjump*) instr;
                Item* first = i->get_first();
                Item* second = i->get_second();
                Item* label = i->get_label();
                if(i->get_first()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_first();
                    if(var->get_variable_name() == v){
                        Instruction_mem_load* load = load_instr(ctr,new_v);
                        new_func.instructions.push_back(load);
                        first = &Variable((new_v + std::to_string(ctr)) );
                    }
                }
                if(i->get_second()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_second();
                    if(var->get_variable_name() == v){
                        Instruction_mem_load* load = load_instr(ctr,new_v);
                        new_func.instructions.push_back(load);
                        second = &Variable((new_v+std::to_string(ctr)));
                    }
                }
                if(i->get_label()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_label();
                    if(var->get_variable_name() == v){
                        Instruction_mem_load* load = load_instr(ctr,new_v);
                        new_func.instructions.push_back(load);
                        label = &Variable((new_v+std::to_string(ctr)));
                    }
                }
                Instruction_cjump* new_i = new Instruction_cjump(first,i->get_op(), second,label);
                new_func.instructions.push_back(new_i);
                ctr++;

            }


        }
    }
}