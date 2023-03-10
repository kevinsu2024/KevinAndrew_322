#include <code_generator.h>


namespace IR {
    std::string
    get_array_offset(std::vector<std::string> indexes, std::string var){
        std::string ans = "";
        auto indices = indexes;
        std::reverse(indices.begin(),indices.end());
        int64_t length = indices.size();
        
        for(int64_t i = 1; i < length; i++){
            ans += ("\t%addr_" + std::to_string(i) + " <- " + var + " + " + std::to_string((i+1)*8) + "\n");
            ans += ("\t%temp_size_" + std::to_string(i) + " <- load %addr_" + std::to_string(i) + "\n");
            ans += ("\t%size_" + std::to_string(i) + " <- %temp_size_" + std::to_string(i) + " >> 1\n");
        }
        ans += ("\t%array_offset_total <- " + std::to_string(8 * (length + 1)) + "\n");
        for(int64_t i = 0; i < length ; i ++){
            ans += ("\t%temp_offset <- " + indices[i] + "\n");
            for(int64_t j = i + 1; j < length; j++){
                ans += ("\t%temp_offset <- %temp_offset * %size_" + std::to_string(j) + "\n");
            }
            ans += ("\t%temp_offset <- %temp_offset * 8\n");
            ans += ("\t%array_offset_total <- %array_offset_total + %temp_offset\n");
        }
        ans += ("\t%temp_addr <- " + var + " + %array_offset_total\n");
        return ans;
    }


    std::string
    translate_instruction(Instruction* in, std::set<std::string>* array_vars, std::set<std::string>* tuple_vars){
        std::string s = "";
        std::cerr << "translating: " << in->to_string() << "\n";
        if(in->get_name() == "Instruction_load"){
            Instruction_load* instr = (Instruction_load*) in;
            std::string dst = instr->get_var_dst()->to_string();
            std::string src = instr->get_var_src()->to_string();
            std::vector<Item*> indices = instr->get_indices();
            std::string res = "";

            if((*array_vars).find(src) != (*array_vars).end()){
                //in array
                std::vector<std::string> indexes;
                for(Item* it : indices){
                    indexes.push_back(it->to_string());
                }

                res += get_array_offset(indexes, src);
                res += ("\t" + dst + " <- load %temp_addr\n");
            }
            else if((*tuple_vars).find(src) != (*tuple_vars).end()){
                //in tuple
                std::cerr<< "index is " << indices[0]->to_string() << "\n";
                res += ("\t%temp_index_var <- " + indices[0]->to_string() + "\n");
                res += ("\t%temp_index_var <- %temp_index_var * 8\n");
                res += ("\t%temp_index_var <- %temp_index_var + 8\n");
                res += ("\t%temp_var_0 <- " + src + " + %temp_index_var\n");
                res += ("\t" + dst + " <- load %temp_var_0\n");
            }
            
            return res;
        }
        else if(in->get_name() == "Instruction_store"){
            Instruction_store* instr = (Instruction_store*) in;
            std::string dst = instr->get_var()->to_string();
            std::string src = instr->get_s()->to_string();
            std::vector<Item*> indices = instr->get_indices();
            std::string res = "";

            if((*array_vars).find(dst) != (*array_vars).end()){
                //in array
                std::vector<std::string> indexes;
                for(Item* it : indices){
                    indexes.push_back(it->to_string());
                }

                res += get_array_offset(indexes, dst);
                res += ("\tstore %temp_addr <- " + src + "\n");
            }
            else if ((*tuple_vars).find(dst) != (*tuple_vars).end()){
                //in tuple
                res += ("\t%temp_index_var <- " + indices[0]->to_string() + "\n");
                res += ("\t%temp_index_var <- %temp_index_var * 8\n");
                res += ("\t%temp_index_var <- %temp_index_var + 8\n");
                res += ("\t%temp_var_1 <- " + dst + " + %temp_index_var\n");
                res += ("\tstore %temp_var_1 <- " + src + "\n");
            }
            return res;
        }
        else if (in->get_name() == "Instruction_declaration"){
            Instruction_declaration* instr = (Instruction_declaration*) in;
            std::string var_type = instr->get_var_type()->to_string();
            std::string var_name = instr->get_var()->to_string();
            if(var_type.substr(0,5) == "int64" && var_type.size() != 5){
                (*array_vars).insert(var_name);
            }
            else if (var_type == "tuple"){
                (*tuple_vars).insert(var_name);
            }
            
            return "";
        }
        else if(in->get_name() == "Instruction_array_length"){
            Instruction_array_length* instr = (Instruction_array_length*) in;
            std::string dst = instr->get_dst_var()->to_string();
            std::string src = instr->get_src_var()->to_string();
            std::string index = instr->get_index()->to_string();
            std::string src_offset = src + "_offset";
            std::string src_address = src + "_address";
            std::string res = ("\t" + src_offset + " <- 8\n");
            
            int64_t off = stoi(index) * 8;
            res += ("\t" + src_offset + " <- " + src_offset + " + " + std::to_string(off) + "\n");
            res += ("\t" + src_address + " <- " + src + " + " + src_offset + "\n");
            res += ("\t" + dst + " <- load " + src_address + "\n");
            return res;
        }
        else if(in->get_name() == "Instruction_tuple_length"){
            Instruction_tuple_length* instr = (Instruction_tuple_length*) in;
            std::string dst = instr->get_dst_var()->to_string();
            std::string src = instr->get_src_var()->to_string();
            std::string res = "";
            res += ("\t" + dst + " <- load " + src + "\n");
            res += ("\t" + dst + " <- " + dst + " << 1\n");
            res += ("\t" + dst + " <- " + dst + " + 1\n");
            return res;
        }
        else if(in->get_name() == "Instruction_create_array"){
            // std::cerr << "created array\n";
            Instruction_create_array* instr = (Instruction_create_array*) in;
            std::string res = "";
            std::string dst = instr->get_dst_var()->to_string();
            std::vector<Item*> args = instr->get_args();
            std::reverse(args.begin(), args.end());
            int64_t num_dimensions = args.size();
            std::vector<std::string> temp;
            for(int64_t i = 0; i < num_dimensions; i++){
                std::string arg = args[i]->to_string();
                std::string new_arg = arg;
                if(arg.substr(0,1) != "%"){
                    new_arg = "%num_" + arg + "_" + std::to_string(i);
                }
                res += ("\t" + new_arg + "D <- " + arg + " >> 1\n");
                temp.push_back(new_arg + "D");
            }
            for(int64_t i = 0; i < temp.size(); i++){
                if(i == 0) res += ("\t%temp <- " + temp[i]);

                else{
                    res += ("\t%temp <- %temp * " + temp[i]);
                }
                res += "\n";
            }
            res += ("\t%temp <- %temp + " + std::to_string(args.size()) + "\n");
            res += ("\t%temp <- %temp << 1\n");
            res += ("\t%temp <- %temp + 1\n");
            res += ("\t" + dst + " <- call allocate(%temp,1)\n");

            for(int64_t i = 0; i < num_dimensions; i++){
                std::string arg = args[i]->to_string();
                res += ("\t%temp" + std::to_string(i) + " <- " + dst + " + " + std::to_string(8*(i+1)) + "\n");
                res += ("\tstore %temp" + std::to_string(i) + " <- " + arg + "\n");
            }

            return res;
        }
        else if(in->get_name() == "Instruction_create_tuple"){
            Instruction_create_tuple* instr = (Instruction_create_tuple*) in;
            std::string dst = instr->get_dst_var()->to_string();
            std::string size = instr->get_size()->to_string();
            std::string res = "";
            res += ("\t" + dst + " <- call allocate (" + size + ",1)\n");
            return res;
        } 
        else if (in->get_name() == "Instruction_branch_t"){
            Instruction_branch_t* instr = (Instruction_branch_t*) in;
            std::string res = "";
            std::string t = instr->get_t()->to_string();
            std::string l1 = instr->get_label1()->to_string();
            std::string l2 = instr->get_label2()->to_string();
            res += ("\tbr " + t + " " + l1 + "\n");
            res += ("\t br " + l2 + "\n");


            return res;
        }
        else{
            return "\t" + in->to_string();
        }
    }

    

    void
    generate_code(Program p, bool verbose){
        std::ofstream outputFile;
        outputFile.open("prog.L3");

        if (verbose){
            std::cerr << "printing now; \n";
            std::cerr << "no of functions: " << p.functions.size() << "\n";
        }

        
        for (auto f : p.functions){
            outputFile << "define " << f->name << " (";
            for(int64_t i = 0; i < f->vars.size(); i++){
                outputFile << f->vars[i]->to_string();
                if(i != f->vars.size()-1) outputFile << ", ";
            }
            outputFile << "){\n";

            std::string func_string = "";
            std::set<std::string> array_mapping;
            std::set<std::string> tuple_mapping;
            for(int i = 0 ; i < f->vars.size(); i ++){
                if(f->types[i]->to_string() == "tuple") tuple_mapping.insert(f->vars[i]->to_string());
                else if(f->types[i]->to_string().substr(f->types[i]->to_string().size()-1,1) == "]") array_mapping.insert(f->vars[i]->to_string());
            }
            auto traces = get_traces(f->basic_blocks, verbose);


            if (verbose){
                std::cerr << "traces has length " << traces.size() <<"\n";
            }

            while(traces.size() > 0){
                auto trace = traces.back();
                traces.pop_back();
                std::vector<BasicBlock*> blocks =  trace->blocks;
                std::vector<Instruction*> instructions;
                for(int64_t i = blocks.size()-1; i > -1; i --){
                    BasicBlock* block = blocks[i];
                    Instruction_label* lb = new Instruction_label(block->label);
                    instructions.push_back(lb);
                    for(Instruction* in : block->instructions){
                        instructions.push_back(in);
                    }
                    //if(block->end->get_name() != "Instruction_branch") 
                    // if (block->end->get_name() != "Instruction_branch"){
                    instructions.push_back(block->end);
                    // }
                }
                // for(int i = 0; i < instructions.size() - 1; i++){
                //     if (instructions[i]->get_name() == "Instruction_branch" && instructions[i+1]->get_name() == "Instruction_label"){
                //         Instruction_branch* ins = (Instruction_branch*) instructions[i];
                //         Instruction_label* lab = (Instruction_label*) instructions[i+1];
                //         if (ins->get_label()->to_string() == lab->get_label()->to_string()){
                //             std::cerr << "erasing instruction: " << instructions[i]->to_string() << "\n";
                //             instructions.erase(instructions.begin() + i + 1);
                //         }
                //     }
                //     // func_string += (translate_instruction(in, &array_mapping, & tuple_mapping) + "\n");
                // }
                for(Instruction* in : instructions){
                    func_string += (translate_instruction(in, &array_mapping, & tuple_mapping) + "\n");
                }
            }
            outputFile<< func_string;
            
            outputFile << "}\n";
            std::cerr << "finished IR code gen.";
        }

        outputFile.close();
        return;
    }
}