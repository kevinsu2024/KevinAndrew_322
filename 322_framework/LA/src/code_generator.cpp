#include <code_generator.h>


namesapce LA{
    void
    convert_constant(Item* it){
        int64_t val = stoi(it->to_string());
        val <<= 1;
        val += 1;
        it->set_string(std::to_string(val));
        return;
    }

    int64_t
    decode_name(std::vector<Instruction*>* instructions,  std::set<std::string>* int_names, int64_t ind, Item* it){
        if(it->get_name() != "Name") return 0;
        if((int_names*).find(it->to_string()) != (int_names*).end()) return 0;
        Item* new_name = new Name(it->to_string() + "_new");
        (int_names*).insert(new_name->to_string());
        Instruction_declaration* new_dec = new Instruction_declaration(new Type("int64"), new_name);
        Instruction_assignment* new_assing = new Instruction_op(new_name, it, new Op(">>"), new InstructionNumber("1"));
        (instructions*).insert((instructions*).begin() + ind, new_assing);
        (instructions*).insert((instructions*).begin(), new_dec);
        return 2;
    }

    void
    generate_code(Program p){
        std::ofstream outputFile;
        outputFile.open("prog.IR");

        for(auto f : p.functions){
            auto instructions = f->instructions;
            int64_t ctr = 0;
            std::set<std::string> int_names;
            while(ctr < instructions.size()){
                Instruction* in = instructions[ctr];
                std::string name = in->get_name();
                if(name == "Instruction_declaration"){
                    Instruction_declaration* instr = (Instruction_declaration*) in;
                    std::string type = instr->get_var_type();
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
                    if(instr->get_s()->get_name() == "InstructionNumber"){
                        convert_constant(instr->get_s());
                    }
                    ctr++;
                
                }
                else if(name == "Instruction_op"){
                    Instruction_op* instr = (Instruction_op*) in;
                    if(instr->get_t1()->get_name() == "Name"){
                        ctr += decode_name(&instructions, &int_names, ctr, instr_get_t1());
                    }
                    if(instr->get_t2()->get_name() == "Name"){
                        ctr += decode_name(&instructions, &int_names, ctr, instr->get_t2());
                    }
                    if(int_names.find(instr->get_var()->to_string()) != int_names.end()){
                        Instruction_op* i1 = new Instruction_op(instr->get_var(), instr->get_var(), new Op("<<"), new InstructionNumber("1"));
                        Instruction_op* i2 = new Instruction_op(instr->get_var(), instr->get_var(), new Op("+"), new InstructionNumber("1"));
                        instructions.insert(instructions.begin()+ctr+1,i1);
                        instructions.insert(instructions.begin() + ctr + 2, i2);
                        ctr += 2;
                    }
                    ctr ++;
                }
                else if(name == "Instruction_array_length"){
                    Instruction_array_length* instr = (Instruction_array_length*) in;
                    if(instr->get_index()->get_name() == "Name") ctr += decode_name(&instructions, &int_names, ctr, instr->get_index());
                    ctr++;
                }
                else if(name == "Instruction_branch_t"){
                    Instruction_branch_t* instr = (Instruction_branch_t*) in;
                    if(instr->get_t()->get_name() == "Name") ctr += decode_name(&instructions, &int_names, ctr, instr->get_t());
                    ctr++;
                }
                else if(name == "Instruction_load"){
                    Instruction_load* instr = (Instruction_load*) in;
                    for(Item* it : instr->get_indices()){
                        ctr += decode_name(&instructions, &int_names, ctr, it);
                    }
                    ctr++;
                }
                else if(name == "Instruction_store"){
                    Instruction_store* instr = (Instruction_store*) in;
                    for(Item* it : instr->get_indices()){
                        ctr += decode_name(&instructions, &int_names, ctr, it);
                    }
                    ctr++;
                }
                else{
                    ctr++;
                }
            }
            f->instructions = instructions;
        }


        outputFile.close();
    }
}