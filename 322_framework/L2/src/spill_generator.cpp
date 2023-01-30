#include <spill_generator.h>

using namespace std;

namespace L2{
    Instruction_mem_load*
    load_instr(int64_t ctr, std::string var){
        Item* reg = new Register(RegisterID::rsp);
        reg->set_name("Register");
        Item* num = new InstructionNumber(std::to_string(ctr * 8));
        num->set_name("InstructionNumber");
        Item* dest = new Variable(var + std::to_string(ctr));
        dest->set_name("Variable");
        Instruction_mem_load* instr = new Instruction_mem_load(dest,reg,num);
        instr->set_name("Instruction_mem_load");
        return instr;
    }

    Instruction_mem_store*
    store_instr(int64_t ctr, std::string var){
        Item* x_reg = new Register(RegisterID::rsp);
        x_reg->set_name("Register");
        Item* num = new InstructionNumber(std::to_string(ctr * 8));
        num->set_name("InstructionNumber");
        Item* src = new Variable(var + std::to_string(ctr));
        src->set_name("Variable");
        Instruction_mem_store* instr = new Instruction_mem_store(src,num,x_reg);
        instr->set_name("Instruction_mem_store");
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
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        first = temp_var;
                        first->set_name("Variable");
                    }
                }
                if(i->get_second()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_second();
                    if(var->get_variable_name() == v){
                        Instruction_mem_load* load = load_instr(ctr,new_v);
                        new_func.instructions.push_back(load);
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        second = temp_var;
                        second->set_name("Variable");
                    }
                }
                if(i->get_label()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_label();
                    if(var->get_variable_name() == v){
                        Instruction_mem_load* load = load_instr(ctr,new_v);
                        new_func.instructions.push_back(load);
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        label = temp_var;
                        label->set_name("Variable");
                    }
                }
                Instruction_cjump* new_i = new Instruction_cjump(first,i->get_op(), second,label);
                new_i->set_name("Instruction_cjump");
                new_func.instructions.push_back(new_i);
                ctr++;

            }
            else if(instr_type == "Instruction_cmp_assignment"){
                Instruction_cmp_assignment* i = (Instruction_cmp_assignment*) instr;
                Item* first = i->get_first();
                Item* second = i->get_second();
                Item* dst = i->get_dst();
                if(i->get_first()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_first();
                    if(var->get_variable_name() == v){
                        Instruction_mem_load* load = load_instr(ctr,new_v);
                        new_func.instructions.push_back(load);
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        first = temp_var;
                        first->set_name("Variable");
                    }
                }
                if(i->get_second()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_second();
                    if(var->get_variable_name() == v){
                        Instruction_mem_load* load = load_instr(ctr,new_v);
                        new_func.instructions.push_back(load);
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        second = temp_var;
                        second->set_name("Variable");
                    }
                }
                if(i->get_dst()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_dst();
                    if(var->get_variable_name() == v){
                        // Instruction_mem_load* load = load_instr(ctr,new_v);
                        // new_func.instructions.push_back(load);
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        dst = temp_var;
                        dst->set_name("Variable");
                    }
                }
                Instruction_cmp_assignment* new_i = new Instruction_cmp_assignment(dst,first,second,i->get_op());
                new_i->set_name("Instruction_cmp_assignment");
                new_func.instructions.push_back(new_i);
                if(i->get_dst()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_dst();
                    if(var->get_variable_name() == v){
                        Instruction_mem_store* store = store_instr(ctr,new_v);
                        new_func.instructions.push_back(store);
                        // Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        // dst = temp_var;
                    }
                }
                
                ctr++;

            }
            else if(instr_type == "Instruction_assignment"){
                Instruction_assignment* i = (Instruction_assignment*) instr;
                Item* src = i->get_src();
                Item* dst = i->get_dst();
                if(i->get_src()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_src();
                    if(var->get_variable_name() == v){
                        Instruction_mem_load* load = load_instr(ctr,new_v);
                        new_func.instructions.push_back(load);
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        src = temp_var;
                        src->set_name("Variable");
                    }
                }
                if(i->get_dst()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_dst();
                    if(var->get_variable_name() == v){
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        dst = temp_var;
                        dst->set_name("Variable");
                    }
                }
                Instruction_assignment* new_i = new Instruction_assignment(dst,src);
                new_i->set_name("Instruction_assignment");
                new_func.instructions.push_back(new_i);
                if(i->get_dst()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_dst();
                    if(var->get_variable_name() == v){
                        Instruction_mem_store* store = store_instr(ctr,new_v);
                        new_func.instructions.push_back(store);
                        // Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        // dst = temp_var;
                    }
                }
                ctr++;

            }
            else if(instr_type == "Instruction_stackarg_assignment"){
                Instruction_stackarg_assignment* i = (Instruction_stackarg_assignment*) instr;
                Item* dst = i->get_dst();
                Item* num = i->get_num();
                if(i->get_dst()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_dst();
                    if(var->get_variable_name() == v){
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        dst = temp_var;
                        dst->set_name("Variable");
                    }
                }
                Instruction_stackarg_assignment* new_i = new Instruction_stackarg_assignment(dst, num);
                new_i->set_name("Instruction_stackarg_assignment");
                new_func.instructions.push_back(new_i);
                if(i->get_dst()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_dst();
                    if(var->get_variable_name() == v){
                        Instruction_mem_store* store = store_instr(ctr,new_v);
                        new_func.instructions.push_back(store);
                        // Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        // dst = temp_var;
                    }
                }
                ctr++;
            
            }
            else if(instr_type == "Instruction_function_assignment"){
                Instruction_function_assignment* i = (Instruction_function_assignment*) instr;
                // Item* src = i->get_src();
                Item* dst = i->get_dst();
                Item* fname = i->get_fname();
                if(i->get_dst()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_dst();
                    if(var->get_variable_name() == v){
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        dst = temp_var;
                        dst->set_name("Variable");
                    }
                }
                Instruction_function_assignment* new_i = new Instruction_function_assignment(dst,fname);
                new_i->set_name("Instruction_function_assignment");
                new_func.instructions.push_back(new_i);
                if(i->get_dst()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_dst();
                    if(var->get_variable_name() == v){
                        Instruction_mem_store* store = store_instr(ctr,new_v);
                        new_func.instructions.push_back(store);
                        // Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        // dst = temp_var;
                    }
                }
                ctr++;

            }
            else if(instr_type == "Instruction_aop"){
                Instruction_aop* i = (Instruction_aop*) instr;
                // Item* src = i->get_src();
                Item* src = i->get_src();
                Item* dst = i->get_dst();
                if(i->get_src()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_src();
                    if(var->get_variable_name() == v){
                        Instruction_mem_load* load = load_instr(ctr,new_v);
                        new_func.instructions.push_back(load);
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        src = temp_var;
                        src->set_name("Variable");
                    }
                }
                if(i->get_dst()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_dst();
                    if(var->get_variable_name() == v){
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        dst = temp_var;
                        dst->set_name("Variable");
                    }
                }
                Instruction_aop* new_i = new Instruction_aop(src,i->get_op(),dst);
                dst->set_name("Instruction_aop");
                new_func.instructions.push_back(new_i);
                if(i->get_dst()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_dst();
                    if(var->get_variable_name() == v){
                        Instruction_mem_store* store = store_instr(ctr,new_v);
                        new_func.instructions.push_back(store);
                        // Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        // dst = temp_var;
                    }
                }
                ctr++;

            }
            else if(instr_type == "Instruction_sop"){
                Instruction_sop* i = (Instruction_sop*) instr;
                // Item* src = i->get_src();
                Item* src = i->get_src();
                Item* dst = i->get_dst();
                if(i->get_src()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_src();
                    if(var->get_variable_name() == v){
                        Instruction_mem_load* load = load_instr(ctr,new_v);
                        new_func.instructions.push_back(load);
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        src = temp_var;
                        src->set_name("Variable");
                    }
                }
                if(i->get_dst()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_dst();
                    if(var->get_variable_name() == v){
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        dst = temp_var;
                        dst->set_name("Variable");
                    }
                }
                Instruction_sop* new_i = new Instruction_sop(dst,i->get_op(),src);
                new_i->set_name("Instruction_sop");
                new_func.instructions.push_back(new_i);
                if(i->get_dst()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_dst();
                    if(var->get_variable_name() == v){
                        Instruction_mem_store* store = store_instr(ctr,new_v);
                        new_func.instructions.push_back(store);
                        // Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        // dst = temp_var;
                    }
                }
                ctr++;

            }

            else if(instr_type == "Instruction_pp"){
                Instruction_pp* i = (Instruction_pp*) instr;
                // Item* src = i->get_src();
                Item* reg = i->get_reg();
                if(i->get_reg()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_reg();
                    if(var->get_variable_name() == v){
                        Instruction_mem_load* load = load_instr(ctr,new_v);
                        new_func.instructions.push_back(load);
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        reg = temp_var;
                        reg->set_name("Variable");
                    }
                }
                
                Instruction_pp* new_i = new Instruction_pp(reg);
                new_i->set_name("Instruction_pp");
                new_func.instructions.push_back(new_i);
                if(i->get_reg()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_reg();
                    if(var->get_variable_name() == v){
                        Instruction_mem_store* store = store_instr(ctr,new_v);
                        new_func.instructions.push_back(store);
                    }
                }
                ctr++;

            }
            else if(instr_type == "Instruction_mm"){
                Instruction_mm* i = (Instruction_mm*) instr;
                // Item* src = i->get_src();
                Item* reg = i->get_reg();
                if(i->get_reg()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_reg();
                    if(var->get_variable_name() == v){
                        Instruction_mem_load* load = load_instr(ctr,new_v);
                        new_func.instructions.push_back(load);
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        reg = temp_var;
                        reg->set_name("Variable");
                    }
                }
                
                Instruction_mm* new_i = new Instruction_mm(reg);
                new_i->set_name("Instruction_mm");
                new_func.instructions.push_back(new_i);
                if(i->get_reg()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_reg();
                    if(var->get_variable_name() == v){
                        Instruction_mem_store* store = store_instr(ctr,new_v);
                        new_func.instructions.push_back(store);
                    }
                }
                ctr++;

            }
            else if(instr_type == "Instruction_call_u"){
                Instruction_call_u* i = (Instruction_call_u*) instr;
                // Item* src = i->get_src();
                Item* reg = i->get_u();
                if(i->get_u()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_u();
                    if(var->get_variable_name() == v){
                        Instruction_mem_load* load = load_instr(ctr,new_v);
                        new_func.instructions.push_back(load);
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        reg = temp_var;
                        reg->set_name("Variable");
                    }
                }
                
                Instruction_call_u* new_i = new Instruction_call_u(reg, i->get_num());
                new_i->set_name("Instruction_call_u");
                new_func.instructions.push_back(new_i);
                ctr++;

            }
            else if(instr_type == "Instruction_at"){
                Instruction_at* i = (Instruction_at*) instr;
                // Item* src = i->get_src();
                Item* dst = i->get_reg1();
                Item* reg2 = i->get_reg2();
                Item* reg3 = i->get_reg3();
                if(i->get_reg2()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_reg2();
                    if(var->get_variable_name() == v){
                        Instruction_mem_load* load = load_instr(ctr,new_v);
                        new_func.instructions.push_back(load);
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        reg2 = temp_var;
                        reg2->set_name("Variable");
                    }
                }
                if(i->get_reg3()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_reg3();
                    if(var->get_variable_name() == v){
                        Instruction_mem_load* load = load_instr(ctr,new_v);
                        new_func.instructions.push_back(load);
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        reg3 = temp_var;
                        reg3->set_name("Variable");
                    }
                }
                if(i->get_reg1()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_reg1();
                    if(var->get_variable_name() == v){
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        dst = temp_var;
                        dst->set_name("Variable");
                    }
                }
                Instruction_at* new_i = new Instruction_at(dst, reg2, reg3, i->get_num());
                new_i->set_name("Instruction_at");
                new_func.instructions.push_back(new_i);
                if(i->get_reg1()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_reg1();
                    if(var->get_variable_name() == v){
                        Instruction_mem_store* store = store_instr(ctr,new_v);
                        new_func.instructions.push_back(store);
                    }
                }
                ctr++;


            }
            else if(instr_type == "Instruction_mem_load"){
                Instruction_mem_load* i = (Instruction_mem_load*) instr;
                // Item* src = i->get_src();
                Item* dst = i->get_dst();
                Item* src = i->get_src();
                if(i->get_src()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_src();
                    if(var->get_variable_name() == v){
                        Instruction_mem_load* load = load_instr(ctr,new_v);
                        new_func.instructions.push_back(load);
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        src = temp_var;
                        src->set_name("Variable");
                    }
                }
                if(i->get_dst()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_dst();
                    if(var->get_variable_name() == v){
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        dst = temp_var;
                        dst->set_name("Variable");
                    }
                }
                Instruction_mem_load* new_i = new Instruction_mem_load(dst, src, i->get_num());
                new_i->set_name("Instruction_mem_load");
                new_func.instructions.push_back(new_i);
                if(i->get_dst()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_dst();
                    if(var->get_variable_name() == v){
                        Instruction_mem_store* store = store_instr(ctr,new_v);
                        new_func.instructions.push_back(store);
                    }
                }
                ctr++;
            }
            else if(instr_type == "Instruction_mem_store"){
                Instruction_mem_store* i = (Instruction_mem_store*) instr;
                // Item* src = i->get_src();
                Item* x_reg = i->get_x_reg();
                Item* src = i->get_src();
                if(i->get_src()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_src();
                    if(var->get_variable_name() == v){
                        Instruction_mem_load* load = load_instr(ctr,new_v);
                        new_func.instructions.push_back(load);
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        src = temp_var;
                        src->set_name("Variable");
                    }
                }
                if(i->get_x_reg()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_x_reg();
                    if(var->get_variable_name() == v){
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        x_reg = temp_var;
                        x_reg->set_name("Variable");
                    }
                }
                Instruction_mem_store* new_i = new Instruction_mem_store(src, i->get_num(), x_reg);
                new_i->set_name("Instruction_mem_load");
                new_func.instructions.push_back(new_i);
                if(i->get_x_reg()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_x_reg();
                    if(var->get_variable_name() == v){
                        Instruction_mem_store* store = store_instr(ctr,new_v);
                        new_func.instructions.push_back(store);
                    }
                }
                ctr++;
            }
            else if(instr_type == "Instruction_mem_op_store"){
                Instruction_mem_op_store* i = (Instruction_mem_op_store*) instr;
                // Item* src = i->get_src();
                Item* x_reg = i->get_x_reg();
                Item* src = i->get_src();
                if(i->get_src()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_src();
                    if(var->get_variable_name() == v){
                        Instruction_mem_load* load = load_instr(ctr,new_v);
                        new_func.instructions.push_back(load);
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        src = temp_var;
                        src->set_name("Variable");
                    }
                }
                if(i->get_x_reg()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_x_reg();
                    if(var->get_variable_name() == v){
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        x_reg = temp_var;
                        x_reg->set_name("Variable");
                    }
                }
                Instruction_mem_op_store* new_i = new Instruction_mem_op_store(src, i->get_num(), x_reg, i->get_op());
                new_i->set_name("Instruction_mem_op_load");
                new_func.instructions.push_back(new_i);
                if(i->get_x_reg()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_x_reg();
                    if(var->get_variable_name() == v){
                        Instruction_mem_store* store = store_instr(ctr,new_v);
                        new_func.instructions.push_back(store);
                    }
                }
                ctr++;
            }
            else if(instr_type == "Instruction_mem_op_load"){
                Instruction_mem_op_load* i = (Instruction_mem_op_load*) instr;
                // Item* src = i->get_src();
                Item* dst = i->get_dst();
                Item* src = i->get_src();
                if(i->get_src()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_src();
                    if(var->get_variable_name() == v){
                        Instruction_mem_load* load = load_instr(ctr,new_v);
                        new_func.instructions.push_back(load);
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        src = temp_var;
                        src->set_name("Variable");
                    }
                }
                if(i->get_dst()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_dst();
                    if(var->get_variable_name() == v){
                        Variable* temp_var = new Variable((new_v + std::to_string(ctr)));
                        dst = temp_var;
                        dst->set_name("Variable");
                    }
                }
                Instruction_mem_op_load* new_i = new Instruction_mem_op_load(dst, src, i->get_num(), i->get_op());
                new_i->set_name("Instruction_mem_op_load");
                new_func.instructions.push_back(new_i);
                if(i->get_dst()->get_name() == "Variable"){
                    Variable* var = (Variable*) i->get_dst();
                    if(var->get_variable_name() == v){
                        Instruction_mem_store* store = store_instr(ctr,new_v);
                        new_func.instructions.push_back(store);
                    }
                }
                ctr++;
            }   
        }
        return &new_func;
    }
}
