#include <code_generator.h>


namespace LA{
    Item*
    convert_constant(Item* it){
        int64_t val = stoi(it->to_string());
        val <<= 1;
        val += 1;
        InstructionNumber* num = new InstructionNumber(std::to_string(val));
        return num;
    }

    Item*
    decode_name(std::vector<Instruction*>* instructions, int64_t ind, Item* it){
        Item* new_name = new Name(it->to_string() + "_new");
        Instruction_declaration* new_dec = new Instruction_declaration(new Type("int64"), new_name);
        Instruction_op* new_assing = new Instruction_op(new_name, it, new Op(">>"), new InstructionNumber("1"));
        (*instructions).insert((*instructions).begin() + ind, new_assing);
        (*instructions).insert((*instructions).begin(), new_dec);
        return new_name;
    }

    void
    generate_code(Program p){
        // std::ofstream outputFile;
        // outputFile.open("prog.IR");

        for(auto f : p.functions){
            std::cerr <<"\n orig functions is <<\n" << f->to_string();

            auto instructions = f->instructions;
            int64_t ctr = 0;
            std::set<std::string> int_names;
            while(ctr < instructions.size()){
                Instruction* in = instructions[ctr];
                std::string name = in->get_name();
                if(name == "Instruction_declaration"){
                    Instruction_declaration* instr = (Instruction_declaration*) in;
                    std::string type = instr->get_var_type()->to_string();
                    if(type == "int64"){
                        int_names.insert(instr->get_var()->to_string());
                        Instruction_assignment* i = new Instruction_assignment(instr->get_var(), new InstructionNumber("1"));
                        instructions.insert(instructions.begin() + ctr + 1, i);
                        ctr += 2;
                    }
                    else{
                        ctr++;
                    }
                }
                else if(name == "Instruction_assignment"){
                    Instruction_assignment* instr = (Instruction_assignment*) in;
                    Item* var = instr->get_var();
                    Item* s = instr->get_s();
                    if(s->get_name() == "InstructionNumber"){
                        s = convert_constant(instr->get_s());
                    }
                    Instruction_assignment* new_instr = new Instruction_assignment(var,s);
                    instructions[ctr] = new_instr;
                    ctr++;
                
                }
                else if(name == "Instruction_op"){
                    Instruction_op* instr = (Instruction_op*) in;
                    Item* var = instr->get_var();
                    Item* t1 = instr->get_t1();
                    Item* op = instr->get_op();
                    Item* t2 = instr->get_t2();
                    if(t1->get_name() == "Name" && (int_names.find(t1->to_string()) != int_names.end())){
                        t1 = decode_name(&instructions, ctr, t1);
                        int_names.insert(t1->to_string());
                        ctr += 2;
                    }
                    if(t2->get_name() == "Name" && (int_names.find(t2->to_string()) != int_names.end())){
                        t2 = decode_name(&instructions, ctr, t2);
                        int_names.insert(t2->to_string());
                        ctr += 2;
                    }

                    Instruction* new_instr = new Instruction_op(var, t1, op, t2);
                    instructions[ctr] = new_instr;

                    if(int_names.find(var->to_string()) != int_names.end()){
                        Instruction_op* i1 = new Instruction_op(var, var, new Op("<<"), new InstructionNumber("1"));
                        Instruction_op* i2 = new Instruction_op(var, var, new Op("+"), new InstructionNumber("1"));
                        instructions.insert(instructions.begin()+ctr+1,i1);
                        instructions.insert(instructions.begin() + ctr + 2, i2);
                        ctr += 2;
                    }
                    ctr ++;
                }
                else if(name == "Instruction_array_length"){
                    Instruction_array_length* instr = (Instruction_array_length*) in;
                    Item* d = instr->get_dst_var();
                    Item* s = instr->get_src_var();
                    Item* ind = instr->get_index();
                    if(ind->get_name() == "Name" && (int_names.find(ind->to_string()) != int_names.end())){
                        ind = decode_name(&instructions, ctr, ind);
                        int_names.insert(ind->to_string());
                        ctr += 2;
                    }

                    Instruction* new_instr = new Instruction_array_length(d,s,ind);
                    instructions[ctr] = new_instr;
                    ctr++;
                }
                else if(name == "Instruction_branch_t"){
                    Instruction_branch_t* instr = (Instruction_branch_t*) in;
                    Item* t = instr->get_t();
                    Item* label1 = instr->get_label1();
                    Item* label2 = instr->get_label2();
                    if(t->get_name() == "Name" && (int_names.find(t->to_string()) != int_names.end())){
                        t = decode_name(&instructions, ctr, t);
                        int_names.insert(t->to_string());
                        ctr += 2;
                    }

                    Instruction* new_instr = new Instruction_branch_t(t, label1, label2);
                    instructions[ctr] = new_instr;
                    ctr++;
                }
                else if(name == "Instruction_load"){
                    Instruction_load* instr = (Instruction_load*) in;
                    Item* var_dest = instr->get_var_dst();
                    Item* var_src = instr->get_var_src();
                    std::vector<Item*> indices = instr->get_indices();
                    // check_tensor_error(in, instr, var_dest, var_src, indices);
                    for(int64_t i = 0; i < indices.size(); i++){
                        if(indices[i]->get_name() == "Name" && (int_names.find(indices[i]->to_string()) != int_names.end())){
                            indices[i] = decode_name(&instructions, ctr, indices[i]);
                            int_names.insert(indices[i]->to_string());
                            ctr += 2;
                        }
                    }
                    
                    Instruction* new_instr = new Instruction_load(var_dest, var_src, indices, 0);
                    instructions[ctr] = new_instr;
                    ctr++;
                }
                else if(name == "Instruction_store"){
                    Instruction_store* instr = (Instruction_store*) in;
                    Item* var = instr->get_var();
                    Item* s = instr->get_s();
                    std::vector<Item*> indices = instr->get_indices();
                    for(int64_t i = 0; i < indices.size(); i++){
                        if(indices[i]->get_name() == "Name" && (int_names.find(indices[i]->to_string()) != int_names.end())){
                            indices[i] = decode_name(&instructions, ctr, indices[i]);
                            int_names.insert(indices[i]->to_string());
                            ctr += 2;
                        }
                    }

                    if(s->get_name() == "InstructionNumber"){
                        s = convert_constant(s);
                    }
                    
                    Instruction* new_instr = new Instruction_store(var, indices, s, 0);
                    instructions[ctr] = new_instr;
                    ctr++;
                    
                }
                else if(name == "Instruction_return_t"){
                    Instruction_return_t* instr = (Instruction_return_t*) in;
                    Item* t = instr->get_t();

                    if(t->get_name() == "InstructionNumber"){
                        t = convert_constant(t);
                    }

                    Instruction* new_instr = new Instruction_return_t(t);
                    instructions[ctr] = new_instr;
                    ctr++;
                }
                else if(name == "Instruction_call"){
                    Instruction_call* instr = (Instruction_call*) in;
                    Item* callee = instr->get_callee();
                    std::vector<Item*> args = instr->get_args();
                    for(int64_t i = 0; i < args.size(); i++){
                        if(args[i]->get_name() == "InstructionNumber"){
                            args[i] = convert_constant(args[i]);
                        }
                    }

                    Instruction* new_instr = new Instruction_call(callee, args);
                    instructions[ctr] = new_instr;
                    ctr++;
                }
                else if(name == "Instruction_call_assignment"){
                    Instruction_call_assignment* instr = (Instruction_call_assignment*) in;
                    Item* var = instr->get_var();
                    Item* callee = instr->get_callee();
                    std::vector<Item*> args = instr->get_args();
                    for(int64_t i = 0; i < args.size(); i++){
                        if(args[i]->get_name() == "InstructionNumber"){
                            args[i] = convert_constant(args[i]);
                        }
                    }

                    Instruction* new_instr = new Instruction_call_assignment(var, callee, args);
                    instructions[ctr] = new_instr;
                    ctr++;
                }
                else if(name == "Instruction_create_array"){
                    Instruction_create_array* instr = (Instruction_create_array*) in;
                    Item* var = instr->get_dst_var();
                    std::vector<Item*> args = instr->get_args();
                    for(int64_t i = 0; i < args.size(); i++){
                        if(args[i]->get_name() == "InstructionNumber"){
                            args[i] = convert_constant(args[i]);
                        }
                    }

                    Instruction* new_instr = new Instruction_create_array(var, args);
                    instructions[ctr] = new_instr;
                    ctr++;
                }
                else if(name == "Instruction_create_tuple"){
                    Instruction_create_tuple* instr = (Instruction_create_tuple*) in;
                    Item* dst_var = instr->get_dst_var();
                    Item* size = instr->get_size();
                    if(size->get_name() == "InstructionNumber"){
                        size = convert_constant(size);
                    }

                    Instruction* new_instr = new Instruction_create_tuple(dst_var,size);
                    instructions[ctr] = new_instr;
                    ctr++;
                }
                else{
                    ctr++;
                }
            }
            f->instructions = instructions;

            std::cerr << "new func is \n" << f->to_string();
        }


        // outputFile.close();
    }
}