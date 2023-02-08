#include <code_generator.h>
#include <unordered_map>

namespace L3{

    std::vector<std::string> arg_registers {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
    std::string
    generate_global_label(Program p){
        //scan through entire program to find longest label
        std::string longest_label = "";
        for (auto f : p.functions){
            for (auto i : f->instructions){
                std::string i_str = i->to_string();
                //if string contains label
                auto pos = i_str.find(":");
                auto length = 0;
                if (pos != std::string::npos){
                    for (int i = pos; i < i_str.size(); i++){
                        if (i_str[i] == ' '){
                            break;
                        }
                        length = i;
                    }
                }
                //shorter; don't care
                if (length <= longest_label.size()){
                    continue;
                }
                //longer; replace
                else{
                    longest_label = i_str.substr(pos,length+1);
                }
            }
        }
        longest_label += "_global";
        return longest_label;
    }

    Program
    convert_all_labels(Program p, std::string longest_label){
        int64_t ctr = 0;
        //look through all instructions
        std::unordered_map<std::string, std::string> label_mapping;
        for (auto f : p.functions){
            label_mapping.clear();
            for (int i = 0; i < f->instructions.size(); i++){
                // only instructions with label or s rule
                std::string new_label = longest_label + std::to_string(ctr);
                if (f->instructions[i]->get_name() == "Instruction_assignment"){
                    Instruction_assignment* instr = (Instruction_assignment*) f->instructions[i];
                    if (instr->get_s()->get_name() == "InstructionLabel"){
                        std::string old_lab = instr->get_s()->to_string();
                        if (label_mapping.find(old_lab) == label_mapping.end()){
                            label_mapping[old_lab] = new_label;
                            ctr++;
                        }
                        f->instructions.erase(f->instructions.begin() + i);
                        InstructionLabel* lab = new InstructionLabel(label_mapping[old_lab]);
                        Instruction_assignment* new_instr = new Instruction_assignment(instr->get_var(), lab);
                        f->instructions.insert(f->instructions.begin() + i,new_instr);
                        
                    }
                } else if (f->instructions[i]->get_name() == "Instruction_store"){
                    Instruction_store* instr = (Instruction_store*) f->instructions[i];
                    if (instr->get_s()->get_name() == "InstructionLabel"){
                        std::string old_lab = instr->get_s()->to_string();
                        if (label_mapping.find(old_lab) == label_mapping.end()){
                            label_mapping[old_lab] = new_label;
                            ctr++;
                        }
                        f->instructions.erase(f->instructions.begin() + i);
                        InstructionLabel* lab = new InstructionLabel(label_mapping[old_lab]);
                        Instruction_store* new_instr = new Instruction_store(instr->get_var(), lab);
                        f->instructions.insert(f->instructions.begin() + i,new_instr);
                    }
                } else if (f->instructions[i]->get_name() == "Instruction_label"){
                    Instruction_label* instr = (Instruction_label*) f->instructions[i];
                    std::string old_lab = instr->get_label()->to_string();
                    if (label_mapping.find(old_lab) == label_mapping.end()){
                        label_mapping[old_lab] = new_label;
                        ctr++;
                    }
                    f->instructions.erase(f->instructions.begin() + i);
                    InstructionLabel* lab = new InstructionLabel(label_mapping[old_lab]);
                    Instruction_label* new_instr = new Instruction_label(lab);
                    f->instructions.insert(f->instructions.begin() + i, new_instr);
                } else if (f->instructions[i]->get_name() == "Instruction_branch"){
                    Instruction_branch* instr = (Instruction_branch*) f->instructions[i];
                    std::string old_lab = instr->get_label()->to_string();
                    if (label_mapping.find(old_lab) == label_mapping.end()){
                        label_mapping[old_lab] = new_label;
                        ctr++;
                    }
                    f->instructions.erase(f->instructions.begin() + i);
                    InstructionLabel* lab = new InstructionLabel(label_mapping[old_lab]);
                    Instruction_branch* new_instr = new Instruction_branch(lab);
                    f->instructions.insert(f->instructions.begin() + i, new_instr);
                } else if (f->instructions[i]->get_name() == "Instruction_branch_t"){
                    Instruction_branch_t* instr = (Instruction_branch_t*) f->instructions[i];
                    std::string old_lab = instr->get_label()->to_string();
                    if (label_mapping.find(old_lab) == label_mapping.end()){
                        label_mapping[old_lab] = new_label;
                        ctr++;
                    }
                    f->instructions.erase(f->instructions.begin() + i);
                    InstructionLabel* lab = new InstructionLabel(label_mapping[old_lab]);
                    Instruction_branch_t* new_instr = new Instruction_branch_t(instr->get_t(),lab);
                    f->instructions.insert(f->instructions.begin() + i, new_instr);
                }

                // ctr++;
            }   
        }
        return p;
    }

    void
    generate_call(Instruction* instr, std::ofstream& out){
        Instruction_call* i = (Instruction_call*) instr;
        std::vector<Item*> args = i->get_args();
        std::string callee = i->get_callee()->to_string();
        std::string return_label = "";
        
        //if current instruction is not standard library function, then need to store arguments, etc.
        if (callee.at(0) == '@' && callee.at(0) == '%'){ 
            return_label = ":" + callee + "_ret";
            out << "\t\tmem rsp -8 <- :" <<  return_label << "\n"; // create ret label
        } 

        //store inputs
        std::cout << args.size() << "\n";
        for (int i = 0; i < args.size(); i++){
            if (i < 6){
                out << arg_registers[i] << " <- " << args[i]->to_string() <<"\n";
            } else {
                //start at -16 because -8 is for ret addr
                out << "\t\tmem rsp -" << std::to_string((i-6)*8 + 16) << " <- " << args[i]->to_string() <<"\n";
            }
        }
        
        //the actual call line
        
        out << "\t\tcall " << callee << " " << args.size() << "\n";
        
        // for (int i = 0; i < args.size(); i++){
        //     out << args[i]->to_string();
        //     if (i < args.size() - 1){
        //         out << ", ";
        //     }
        // }
        //return label if we're not standard library
        if (callee.at(0) == '@' && callee.at(0) == '%'){
            out << "\t\t" << return_label << "\n";
        }
        
    }

    void
    generate_call_assignment(Instruction* instr, std::ofstream& out){
        Instruction_call_assignment* i = (Instruction_call_assignment*) instr;
        std::vector<Item*> args = i->get_args();
        std::string callee = i->get_callee()->to_string();
        std::string return_label = "";
        std::string var = i->get_var()->to_string();
        
        //if current instruction is not standard library function, then need to store arguments, etc.
        if (callee.at(0) == '@' && callee.at(0) == '%'){ 
            return_label = ":" + callee + "_ret";
            out << "\t\tmem rsp -8 <- :" <<  return_label << "\n"; // create ret label
        } 

        //store inputs
        std::cout << args.size() << "\n";
        for (int i = 0; i < args.size(); i++){
            if (i < 6){
                out << arg_registers[i] << " <- " << args[i]->to_string() <<"\n";
            } else {
                //start at -16 because -8 is for ret addr
                out << "\t\tmem rsp -" << std::to_string((i-6)*8 + 16) << " <- " << args[i]->to_string() <<"\n";
            }
        }
        
        //the actual call line
        
        out << "\t\tcall " << callee << " " << args.size() << "\n";
        
        // for (int i = 0; i < args.size(); i++){
        //     out << args[i]->to_string();
        //     if (i < args.size() - 1){
        //         out << ", ";
        //     }
        // }
        //return label if we're not standard library
        if (callee.at(0) == '@' && callee.at(0) == '%'){
            out << "\t\t" << return_label << "\n";
        }
        //extra step for assignment
        out << "\t\t" << var << " <- rax\n";
    }

    void
    generate_label(Instruction* instr, std::ofstream& out){

    }

    void
    generate_code(Program p){
        create_tiles();
        std::string global_label = generate_global_label(p);
        
        std::ofstream outputFile;
        p = convert_all_labels(p, global_label);
        auto contexts = generate_contexts(p);
        // std::cout << "\ncontexts has size " << contexts.size() << "\n";
        int64_t index = 0;
        
        outputFile.open("prog.L2");
        //maintained invariant that main is always first function
        outputFile << "(@main\n";
        //generates other code
        
        for (auto f : p.functions){
            outputFile << "\t(" << f->name << "\n";
            //callee function start convention
            auto var_no = f->vars.size();
            outputFile << "\t\t" << std::to_string(var_no) << "\n";
            for (int i = 0; i < var_no; i++){
                std::string var_str = f->vars[i]->to_string();
                if (i < 6){
                    outputFile << "\t\t" << var_str << " <- " << arg_registers[i] << "\n";
                } else {
                    int stack_addr = 8 * (var_no-i);
                    outputFile << "\t\t" << var_str << " <- " << "stack-arg " << std::to_string(stack_addr) << "\n";
                }
            }
            while(index < contexts.size() && contexts[index]->func_name == f->name){
                auto context = contexts[index];
                auto instructions = context->instructions;
                std::vector<Node*> trees;
                if(context->isContext){
                    for(auto inst : instructions){
                        Node* tree = instruction_to_graph(inst);
                        trees.push_back(tree);
                    }
                    //merging here
                    for(auto tree : trees){
                        auto tile = get_matching_tile(tree);
                        std::string converted = convert_to_instructions(tree,tile);
                        // std::cout << "converted is : \n" << converted << "\n";
                        outputFile << converted;
                    }
                }
                else{
                    auto i = context->instructions[0];
                    if (i->get_name() == "Instruction_call"){
                        generate_call(i, outputFile);
                    } else if (i->get_name() == "Instruction_call_assignment"){
                        generate_call_assignment(i, outputFile);
                    } else if (i->get_name() == "Instruction_label"){
                        Instruction_label* instr = (Instruction_label*) i;
                        outputFile << "\t" << instr->to_string() <<"\n";
                    }

                }
                index++;
            }
            outputFile << "\t)\n";

        }
        outputFile << "\n)";
        outputFile.close();   
    }

    
}