#include <spill_generator.h>

using namespace std;

namespace L2{

    std::string
    convert_string(std::string s, std::string old_var, std::string new_var){
        if(s.find(old_var) == std::string::npos) return s;
        s.replace(s.find(old_var), old_var.size(), new_var);
        return s;
    }

    Instruction_mem_load*
    load_instr(int64_t ctr, int64_t locals, std::string var){
        Item* reg = new Register(RegisterID::rsp);
        Item* num = new InstructionNumber(std::to_string(locals * 8));
        Item* dst = new Variable(var + std::to_string(ctr));
        Instruction_mem_load* instr = new Instruction_mem_load(dst,reg,num);
        return instr;
    }

    Instruction_mem_store*
    store_instr(int64_t ctr, int64_t locals, std::string var){
        Item* x_reg = new Register(RegisterID::rsp);
        Item* num = new InstructionNumber(std::to_string(locals * 8));
        Item* src = new Variable(var + std::to_string(ctr));
        Instruction_mem_store* instr = new Instruction_mem_store(src,num,x_reg);
        return instr;
    }

    Function*
    generate_spill_code(Function* f, std::string v, std::string new_v){
        int64_t ctr = 0;
        Function* new_func = new Function();
        new_func->name = f->name;
        new_func->arguments = f->arguments;
        int64_t local_num = f->locals;
        bool changed = false;


        for(Instruction* instr : f->instructions){
            if(instr->to_string().find(v) == std::string::npos) {
                new_func->instructions.push_back(instr);
                continue;
            }
            changed = true;
            std::string instr_type = instr->get_name();
            if(instr_type == "Instruction_cjump"){
                Instruction_cjump* i = (Instruction_cjump*) instr;
                Item* first = i->get_first();
                Item* second = i->get_second();
                Item* label = i->get_label();
                Item* op = i->get_op();
                bool loaded = false;
                if(first->get_name() == "Variable" && first->to_string() == v){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    first = temp_var;
                    loaded = true;
                }
                if(second->get_name() == "Variable" && second->to_string() == v){
                    if(!loaded){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    loaded = true;
                    }
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    second = temp_var;
                }
                if(label->get_name() == "Variable" && label->to_string() == v){
                    if(!loaded){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    }
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    label = temp_var;
                }
                Instruction_cjump* new_i = new Instruction_cjump(first, op, second,label);
                new_func->instructions.push_back(new_i);
                ctr++;

            }
            else if(instr_type == "Instruction_cmp_assignment"){
                Instruction_cmp_assignment* i = (Instruction_cmp_assignment*) instr;
                Item* first = first;
                Item* second = i->get_second();
                Item* dst = i->get_dst();
                bool loaded = false;

                if(first->get_name() == "Variable" && first->to_string() == v){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    first = temp_var;
                    loaded = true;
                }
                if(second->get_name() == "Variable" && second->to_string() == v){
                    if(!loaded){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    }
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    second = temp_var;
                }
                if(dst->get_name() == "Variable" && dst->to_string() == v){
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    dst = temp_var;
                }
                Instruction_cmp_assignment* new_i = new Instruction_cmp_assignment(dst,first,second,i->get_op());
                new_func->instructions.push_back(new_i);
                if(dst->get_name() == "Variable" && dst->to_string() == (new_v + std::to_string(ctr))){
                    Instruction_mem_store* store = store_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(store);
                }
                
                ctr++;

            }
            else if(instr_type == "Instruction_assignment"){
                Instruction_assignment* i = (Instruction_assignment*) instr;
                Item* src = i->get_src();
                Item* dst = i->get_dst();
                if(src->get_name() == "Variable" && src->to_string() == v){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    src = temp_var;
                }
                if(dst->get_name() == "Variable" && dst->to_string() == v){
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    dst = temp_var;
                }

                Instruction_assignment* new_i = new Instruction_assignment(dst,src);
                new_func->instructions.push_back(new_i);

                if(dst->get_name() == "Variable" && dst->to_string() == (new_v + std::to_string(ctr))){
                    Instruction_mem_store* store = store_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(store);
                }
                ctr++;

            }
            else if(instr_type == "Instruction_stackarg_assignment"){
                Instruction_stackarg_assignment* i = (Instruction_stackarg_assignment*) instr;
                Item* dst = i->get_dst();
                Item* num = i->get_num();
                if(dst->get_name() == "Variable" && dst->to_string() == v){
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    dst = temp_var;
                }
                Instruction_stackarg_assignment* new_i = new Instruction_stackarg_assignment(dst, num);
                new_func->instructions.push_back(new_i);
                if(dst->get_name() == "Variable" && dst->to_string() == (new_v + std::to_string(ctr))){
                    Instruction_mem_store* store = store_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(store);
                }
                ctr++;
            
            }
            else if(instr_type == "Instruction_function_assignment"){
                Instruction_function_assignment* i = (Instruction_function_assignment*) instr;
                Item* dst = i->get_dst();
                Item* fname = i->get_fname();
                if(dst->get_name() == "Variable"  && dst->to_string() == v){
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    dst = temp_var;
                }
                Instruction_function_assignment* new_i = new Instruction_function_assignment(dst,fname);
                new_func->instructions.push_back(new_i);
                if(dst->get_name() == "Variable" && dst->to_string() == (new_v + std::to_string(ctr))){
                    Instruction_mem_store* store = store_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(store);
                }
                ctr++;

            }
            else if(instr_type == "Instruction_aop"){
                Instruction_aop* i = (Instruction_aop*) instr;
                Item* src = i->get_src();
                Item* dst = i->get_dst();
                Item* op = i->get_op();
                bool loaded = false;
                if(src->get_name() == "Variable" && src->to_string() == v){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    src = temp_var;
                    loaded = true;
                }
                if(dst->get_name() == "Variable"  && dst->to_string() == v){
                    if(!loaded){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    }
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    dst = temp_var;
                }

                Instruction_aop* new_i = new Instruction_aop(src,op,dst);
                new_func->instructions.push_back(new_i);

                if(dst->get_name() == "Variable" && dst->to_string() == (new_v + std::to_string(ctr))){
                    Instruction_mem_store* store = store_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(store);
                }
                ctr++;

            }
            else if(instr_type == "Instruction_sop"){
                Instruction_sop* i = (Instruction_sop*) instr;
                Item* src = i->get_src();
                Item* dst = i->get_dst();
                Item* op = i->get_op();
                bool loaded = false;
                if(src->get_name() == "Variable" && src->to_string() == v){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    src = temp_var;
                    loaded = true;
                }
                if(dst->get_name() == "Variable"  && dst->to_string() == v){
                    if(!loaded){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    }
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    dst = temp_var;
                }

                Instruction_sop* new_i = new Instruction_sop(dst,i->get_op(),src);
                new_func->instructions.push_back(new_i);

                if(dst->get_name() == "Variable" && dst->to_string() == (new_v + std::to_string(ctr))){
                    Instruction_mem_store* store = store_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(store);
                }
                ctr++;

            }

            else if(instr_type == "Instruction_pp"){
                Instruction_pp* i = (Instruction_pp*) instr;
                Item* reg = i->get_reg();
                if(reg->get_name() == "Variable" && reg->to_string() == v){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    reg = temp_var;
                }
            
                Instruction_pp* new_i = new Instruction_pp(reg);
                new_func->instructions.push_back(new_i);

                if(reg->get_name() == "Variable" && reg->to_string() == (new_v + std::to_string(ctr))){
                    Instruction_mem_store* store = store_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(store);
                }
                ctr++;

            }

            else if(instr_type == "Instruction_mm"){
                Instruction_mm* i = (Instruction_mm*) instr;
                Item* reg = i->get_reg();
                if(reg->get_name() == "Variable" && reg->to_string() == v){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    reg = temp_var;
                }
                
                Instruction_mm* new_i = new Instruction_mm(reg);
                new_func->instructions.push_back(new_i);

                if(reg->get_name() == "Variable" && reg->to_string() == (new_v + std::to_string(ctr))){
                    Instruction_mem_store* store = store_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(store);
                }
                ctr++;
            }

            else if(instr_type == "Instruction_call_u"){
                Instruction_call_u* i = (Instruction_call_u*) instr;
                Item* reg = i->get_u();
                Item* num = i->get_num();
                if(reg->get_name() == "Variable" && reg->to_string() == v){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    reg = temp_var;
                }
                
                Instruction_call_u* new_i = new Instruction_call_u(reg, num);
                new_func->instructions.push_back(new_i);
                ctr++;

            }

            else if(instr_type == "Instruction_at"){
                Instruction_at* i = (Instruction_at*) instr;
                Item* dst = i->get_reg1();
                Item* reg2 = i->get_reg2();
                Item* reg3 = i->get_reg3();
                Item* num = i->get_num();
                bool loaded = false;
                if(reg2->get_name() == "Variable" && reg2->to_string() == v){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    reg2 = temp_var;
                    loaded = true;
                }
                if(reg3->get_name() == "Variable" && reg3->to_string() == v){
                    if(!loaded){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    }
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    reg3 = temp_var;
                }
                if(dst->get_name() == "Variable"  && dst->to_string() == v){
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    dst = temp_var;
                }

                Instruction_at* new_i = new Instruction_at(dst, reg2, reg3, num);
                new_func->instructions.push_back(new_i);

                if(dst->get_name() == "Variable" && dst->to_string() == (new_v + std::to_string(ctr))){
                    Instruction_mem_store* store = store_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(store);
                }
                ctr++;


            }
            else if(instr_type == "Instruction_mem_load"){
                Instruction_mem_load* i = (Instruction_mem_load*) instr;
                Item* dst = i->get_dst();
                Item* src = i->get_src();
                Item* num = i->get_num();
                if(src->get_name() == "Variable" && src->to_string() == v){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    src = temp_var;
                }
                if(dst->get_name() == "Variable"  && dst->to_string() == v){
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    dst = temp_var;
                }

                Instruction_mem_load* new_i = new Instruction_mem_load(dst, src, num);
                new_func->instructions.push_back(new_i);

                if(dst->get_name() == "Variable" && dst->to_string() == (new_v + std::to_string(ctr))){
                    Instruction_mem_store* store = store_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(store);
                }
                ctr++;
            }
            else if(instr_type == "Instruction_mem_store"){
                Instruction_mem_store* i = (Instruction_mem_store*) instr;
                Item* x_reg = i->get_x_reg();
                Item* src = i->get_src();
                Item* num = i->get_num();
                bool loaded = false;
                if(src->get_name() == "Variable" && src->to_string() == v){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    src = temp_var;
                    loaded = true;
                }
                if(x_reg->get_name() == "Variable" && x_reg->to_string() == v){
                    if(!loaded){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    }
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    x_reg = temp_var;
                }
                Instruction_mem_store* new_i = new Instruction_mem_store(src, i->get_num(), x_reg);
                new_func->instructions.push_back(new_i);
                ctr++;
            }
            else if(instr_type == "Instruction_mem_op_store"){
                Instruction_mem_op_store* i = (Instruction_mem_op_store*) instr;
                Item* x_reg = i->get_x_reg();
                Item* src = i->get_src();
                Item* op = i->get_op();
                Item* num = i->get_num();
                bool loaded = false;
                if(src->get_name() == "Variable" && src->to_string() == v){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    src = temp_var;
                    loaded = true;
                }
                if(x_reg->get_name() == "Variable" && x_reg->to_string() == v){
                    if(!loaded){
                        Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                        new_func->instructions.push_back(load);
                    }
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    x_reg = temp_var;
                }
                Instruction_mem_op_store* new_i = new Instruction_mem_op_store(src, num, x_reg, op);
                new_func->instructions.push_back(new_i);
                ctr++;
            }
            else if(instr_type == "Instruction_mem_op_load"){
                Instruction_mem_op_load* i = (Instruction_mem_op_load*) instr;
                Item* dst = i->get_dst();
                Item* src = i->get_src();
                Item* num = i->get_num();
                Item* op = i->get_op();

                if(src->get_name() == "Variable" && src->to_string() == v){
                    Instruction_mem_load* load = load_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(load);
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    src = temp_var;
                }
                if(dst->get_name() == "Variable"  && dst->to_string() == v){
                    Item* temp_var = new Variable((new_v + std::to_string(ctr)));
                    dst = temp_var;
                }

                Instruction_mem_op_load* new_i = new Instruction_mem_op_load(dst, src, num, op);
                new_func->instructions.push_back(new_i);

                if(dst->get_name() == "Variable" && dst->to_string() == (new_v + std::to_string(ctr))){
                    Instruction_mem_store* store = store_instr(ctr, local_num,new_v);
                    new_func->instructions.push_back(store);
                }
                ctr++;
            }  
            
        }
        new_func->arguments = f->arguments;
        new_func->locals = f->locals;
        if(changed) new_func->locals = f->locals + 1;
        return new_func;
    }
}
