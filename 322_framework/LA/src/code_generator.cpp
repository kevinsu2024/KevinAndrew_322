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
    decode_name(std::vector<Instruction*>* instructions, int64_t ind, Item* it, std::string longest){
        std::string nn = longest + it->to_string() + "_new";
        nn = nn.substr(1, nn.size() - 1);
        Item* new_name = new Name(nn);
        Instruction_declaration* new_dec = new Instruction_declaration(new Type("int64"), new_name);
        Instruction_op* new_assing = new Instruction_op(new_name, it, new Op(">>"), new InstructionNumber("1"));
        (*instructions).insert((*instructions).begin() + ind, new_assing);
        (*instructions).insert((*instructions).begin(), new_dec);
        return new_name;
    }

    int64_t
    insert_ins(std::vector<Instruction*>* instructions, Instruction* ins, int64_t ctr){
        if (ins->get_name() == "Instruction_declaration"){
            (*instructions).insert((*instructions).begin(), ins);
        } else {
            (*instructions).insert((*instructions).begin() + ctr, ins);
        }
        return ctr + 1;
        
    }
    int64_t
    check_tensor_error( std::vector<Instruction*>* instructions, int64_t ins_ind, Instruction* ins, Item* array,
                        std::vector<Item*> indices, int64_t line_no, std::string ll, std::string ln){
        //check allocation
        Item* line_number_name = new Name(ln + std::to_string(line_no)+ "_new");
        Item* line_number = new InstructionNumber(std::to_string(line_no));
        Instruction_declaration* line_number_dec = new Instruction_declaration(new Type("int64"), line_number_name);
        Instruction_assignment* line_number_ins = new Instruction_assignment(line_number_name, line_number);
        ins_ind = insert_ins(instructions, line_number_dec, ins_ind);
        ins_ind = insert_ins(instructions, line_number_ins, ins_ind);

        Item* new_v_name = new Name(ln + std::to_string(line_no) + "_new_new");
        Instruction_declaration* new_v_dec = new Instruction_declaration(new Type("int64"), new_v_name);
        ins_ind = insert_ins(instructions, new_v_dec, ins_ind);
        Item* array_name = new Name(array->to_string().substr(1,array->to_string().size()));
        Item* zero_name = new InstructionNumber(std::to_string(0));
        Item* equal_op = new Op("=");
        Instruction_op* new_v_assign = new Instruction_op(new_v_name, array_name, equal_op, zero_name);
        ins_ind = insert_ins(instructions, new_v_assign, ins_ind);
        
        Item* false_label = new InstructionLabel(ll + std::to_string(line_no) + "_new_false");
        Item* true_label = new InstructionLabel(ll + std::to_string(line_no) + "_new_true");
        Instruction_branch_t* branch_false_true = new Instruction_branch_t(new_v_name, false_label, true_label);
        ins_ind = insert_ins(instructions, branch_false_true, ins_ind);
        
        Instruction_label* ins_false_label = new Instruction_label(false_label);
        ins_ind = insert_ins(instructions, ins_false_label, ins_ind);

        Item* tensor_error_name = new Name("tensor-error");
        tensor_error_name->set_string("tensor-error");
        Instruction_call* tensor_error_call = new Instruction_call(tensor_error_name, std::vector<Item*>{line_number});
        ins_ind = insert_ins(instructions, tensor_error_call, ins_ind);

        Instruction_label* ins_true_label = new Instruction_label(true_label);
        ins_ind = insert_ins(instructions, ins_true_label, ins_ind);

        //checking memory access
        for (int i = 0; i < indices.size(); i++){
            Item* length_name = new Name(ln + std::to_string(line_no) + indices[i]->to_string() + "_new");
            Item* dim_name = new InstructionNumber(std::to_string(i));
            Instruction_array_length* arr_len_ins = new Instruction_array_length(length_name, array_name, dim_name);
            ins_ind = insert_ins(instructions, arr_len_ins, ins_ind);

            Item* length_check_name = new Name(ln + std::to_string(line_no) + indices[i]->to_string() + "_check");
            Item* check_op = new Op("<");
            Item* dim_value_name;
            if (indices[i]->get_name() == "Name"){
                dim_value_name = new Name(indices[i]->to_string());
            } else {
                dim_value_name = new InstructionNumber(indices[i]->to_string());
            }
            
            Instruction_op* length_check_ins = new Instruction_op(length_check_name, dim_value_name, check_op, length_name);
            ins_ind = insert_ins(instructions, length_check_ins, ins_ind);

            Item* false_check_label = new InstructionLabel(ll + std::to_string(line_no) + "_new_check_" + std::to_string(i) + indices[i]->to_string() + "false");
            Item* true_check_label = new InstructionLabel(ll + std::to_string(line_no) + "_new_check_" + std::to_string(i) + indices[i]->to_string() + "true");
            Instruction_branch_t* length_check_branch_ins = new Instruction_branch_t(length_check_name, false_check_label, true_check_label);
            ins_ind = insert_ins(instructions, length_check_branch_ins, ins_ind);

            Instruction_label* false_check_label_ins = new Instruction_label(false_check_label);
            ins_ind = insert_ins(instructions, false_check_label_ins, ins_ind);

            if (indices.size() == 1){
                Instruction_call* tensor_error_check_call = new Instruction_call(tensor_error_name, std::vector<Item*>{line_number, length_name, indices[i]});
                ins_ind = insert_ins(instructions, tensor_error_check_call, ins_ind);
            } else {
                Instruction_call* tensor_error_check_call = new Instruction_call(tensor_error_name, std::vector<Item*>{line_number, dim_name, length_name, indices[i]});
                ins_ind = insert_ins(instructions, tensor_error_check_call, ins_ind);
            } 
            Instruction_label* true_check_label_ins = new Instruction_label(true_check_label);
            ins_ind = insert_ins(instructions, true_check_label_ins, ins_ind);
        }
        return ins_ind;
    }

    void
    generate_code(Program p){
        std::ofstream outputFile;
        outputFile.open("prog.IR");

        for(auto f : p.functions){
            std::cerr <<"\n orig functions is <<\n" << f->to_string();
            auto longest_label = f->longest_label + "_global";
            if(f->longest_label.size() == 0) longest_label = ":" + longest_label;
            std::string longest_name = f->longest_name;
            std::cerr<< "\nlongest name is " << longest_name<< "\n\n";
            int label_count = 0;
            auto instructions = f->instructions;
            std::string return_type = f->return_type;
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
                    } else if ((type.find("int64") != std::string::npos && type.size() > 5) || type == "tuple")
                    {
                        Instruction_assignment* i = new Instruction_assignment(instr->get_var(), new InstructionNumber("0"));
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
                        t1 = decode_name(&instructions, ctr, t1, longest_name);
                        int_names.insert(t1->to_string());
                        ctr += 2;
                    }
                    if(t2->get_name() == "Name" && (int_names.find(t2->to_string()) != int_names.end())){
                        t2 = decode_name(&instructions, ctr, t2, longest_name);
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
                        ind = decode_name(&instructions, ctr, ind, longest_name);
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
                        t = decode_name(&instructions, ctr, t, longest_name);
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
                    int64_t line_no = instr->get_line_no();
                    
                    ctr = check_tensor_error(&instructions, ctr, instr, var_src, indices, line_no, longest_label, longest_name);
                    for(int64_t i = 0; i < indices.size(); i++){
                        if(indices[i]->get_name() == "Name" && (int_names.find(indices[i]->to_string()) != int_names.end())){
                            indices[i] = decode_name(&instructions, ctr, indices[i], longest_name);
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
                    int64_t line_no = instr->get_line_no();
                    ctr = check_tensor_error(&instructions, ctr, instr, var, indices, line_no, longest_label, longest_name);
                    for(int64_t i = 0; i < indices.size(); i++){
                        if(indices[i]->get_name() == "Name" && (int_names.find(indices[i]->to_string()) != int_names.end())){
                            indices[i] = decode_name(&instructions, ctr, indices[i], longest_name);
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
            instructions = LA::get_basic_blocks(instructions, longest_label, return_type);

            f->instructions = instructions;
            std::cerr << "new func is \n" << f->to_string();
            outputFile << "define " << f->return_type << " @" << f->name << "(";
            for(int64_t i = 0; i < f->types.size(); i++){
                outputFile << f->types[i]->to_string() << " "<< f->vars[i]->to_string();
                if(i != f->types.size()-1) outputFile << ", ";
            }
            outputFile << "){\n";
            for(auto in : f->instructions){
                outputFile << "\t" << in->to_string() << "\n";
            }
            outputFile << "}\n";
        }


        outputFile.close();
    }
}