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
    generate_call(Instruction* instr, std::ofstream& out, std::string call_label, int64_t num){
        Instruction_call* i = (Instruction_call*) instr;
        std::string label = call_label + std::to_string(num);
        std::vector<Item*> args = i->get_args();
        std::string callee = i->get_callee()->to_string();
        std::string return_label = "";
        
        //if current instruction is not standard library function, then need to store arguments, etc.
        if (callee.at(0) == '@' || callee.at(0) == '%'){ 
            out << "\t\tmem rsp -8 <- " <<  label << "\n"; // create ret label
        } 

        //store inputs
        // std::cout << args.size() << "\n";
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
        if (callee.at(0) == '@' || callee.at(0) == '%'){
            out << "\t\t" << label << "\n";
        }
        
    }

    void
    generate_call_assignment(Instruction* instr, std::ofstream& out, std::string call_label, int64_t num){
        Instruction_call_assignment* i = (Instruction_call_assignment*) instr;
        std::string label = call_label + std::to_string(num);
        std::vector<Item*> args = i->get_args();
        std::string callee = i->get_callee()->to_string();
        std::string return_label = "";
        std::string var = i->get_var()->to_string();
        
        //if current instruction is not standard library function, then need to store arguments, etc.
        if (callee.at(0) == '@' || callee.at(0) == '%'){ 
            out << "\t\tmem rsp -8 <- " <<  label << "\n"; 
        } 

        //store inputs
        // std::cout << args.size() << "\n";
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
        if (callee.at(0) == '@' || callee.at(0) == '%'){
            out << "\t\t" << label << "\n";
        }
        //extra step for assignment
        out << "\t\t" << var << " <- rax\n";
    }

    std::vector<Node*>
    get_leaves(Node* root){
        std::vector<Node*> leaves;
        std::stack<Node*> nodes;
        nodes.push(root);
        while(!nodes.empty()){
            Node* node = nodes.top();
            nodes.pop();
            if(node->neighbors.size() == 0) leaves.push_back(node);
            else{
                for(Node* neigh : node->neighbors){
                    nodes.push(neigh);
                }
            }
        }

        return leaves;
    }



    std::vector<Node*>
    check_conditions(std::vector<Node*> leaves, Node* tree, std::vector<Liveness_Node*> liveness_nodes, int64_t index_first, int64_t index_second){
        std::vector<Node*> matching;
        for(Node* leaf : leaves){
            if(leaf->node_type != tree->node_type || leaf->node_val != tree->node_val) continue;
            std::string var = tree->node_val;
            //check that not alive after second instr
            if(liveness_nodes[index_second]->out.find(var) != liveness_nodes[index_second]->out.end()) continue;

            //check that no use of v between two instructions
            bool works = true;
            for(int64_t i = index_first; i < index_second; i++){
                if(i == index_first){
                    if(liveness_nodes[i]->gen.find(var) != liveness_nodes[i]->gen.end()) works = false;
                }
                else{
                    if((liveness_nodes[i]->gen.find(var) != liveness_nodes[i]->gen.end()) || (liveness_nodes[i]->kill.find(var) != liveness_nodes[i]->kill.end())) works = false;
                }
            }

            if(!works) continue;

            // get list of vars in first isntruction and make sure no writes of others
            for(int64_t i = index_first + 1; i < index_second; i++){
                for(std::string var : liveness_nodes[index_first]->gen){
                    if(liveness_nodes[i]->kill.find(var) != liveness_nodes[i]->kill.end()) works = false;
                }
            }
            if(!works) continue;
            matching.push_back(leaf);
        }
        return matching;
    }

    Node* 
    find_node(Node* root, Node* node){
        if(root->node_type == node->node_type && root->node_val == node->node_val) return root;
        if(root->neighbors.size() == 0) return NULL;
        for(Node* neigh : root->neighbors){
            if(find_node(neigh, node) != NULL) {
                return find_node(neigh, node);
            }
        }
        return NULL;
    }

    void
    merge(Node* tree, Node* t){
        Node* node = find_node(tree,t);
        node->neighbors = t->neighbors;
        return;
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
            std::vector<Liveness_Node*> liveness_nodes = generate_liveness(f);
            std::string call_label_name = ":" + f->name.substr(1,f->name.size()-1);
            int label_num = 0;
            outputFile << "\t(" << f->name << "\n";
            //callee function start convention
            auto var_no = f->vars.size();
            outputFile << "\t\t" << std::to_string(var_no) << "\n";
            for (int i = 0; i < var_no; i++){
                std::string var_str = f->vars[i]->to_string();
                if (i < 6){
                    outputFile << "\t\t" << var_str << " <- " << arg_registers[i] << "\n";
                } else {
                    int stack_addr = 8 * (var_no - i - 1);
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
                    std::vector<int64_t> removed;
                    for(int64_t i = 1; i < trees.size(); i++){
                        //get list of leaves
                        //for every tree prior check conditions if so merge
                        //kee[ track of which indices to remove]
                        std::vector<Node*> leaves = get_leaves(trees[i]);

                        for(int64_t j = 0; j < i; j++){
                            if(std::find(removed.begin(), removed.end(),j) != removed.end()) continue;
                            std::vector<Node*> matching = check_conditions(leaves,trees[j],liveness_nodes, context->start_num + j, context->start_num + i);
                            for(Node* match : matching){
                                merge(match, trees[j]);
                            }
                            removed.push_back(j);
                        }
                    }

                    //remove merged trees
                    for(auto i = removed.size()-1; i > -1; i--){
                        trees.erase(trees.begin() + removed[i]);
                    }





                    //tiling starts here
                    for(auto tree : trees){
                        
                        auto tree_tile_tuples = maximal_munch(tree);
                        for (auto tuple : tree_tile_tuples){
                            auto subtree = tuple[0];
                            auto tile = tuple[1];
                            std::string converted = convert_to_instructions(subtree,tile);
                            // std::cout << "converted is : \n" << converted << "\n";
                            outputFile << converted;
                        }
                        
                    }
                }
                else{
                    auto i = context->instructions[0];
                    if (i->get_name() == "Instruction_call"){
                        generate_call(i, outputFile, call_label_name, label_num);
                        label_num++;
                    } else if (i->get_name() == "Instruction_call_assignment"){
                        generate_call_assignment(i, outputFile, call_label_name, label_num);
                        label_num++;
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