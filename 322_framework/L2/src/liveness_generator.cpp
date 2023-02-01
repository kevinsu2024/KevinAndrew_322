#include <liveness_generator.h>


using namespace std;

namespace L2{

    Node::Node(Instruction* i){
        instr = i;
        std::set<int64_t> preds;
        std::set<int64_t> succs;
        std::set<std::string> gen;
        std::set<std::string> kill;
        std::set<std::string> in;
        std::set<std::string> out;

    }
    std::vector<std::string> argument_registers{"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
    std::string check_item(Item* it){
        if(it->get_name() == "Register"){
                return it->to_string();
            } else if (it->get_name() == "Variable"){
                return it->to_string();
            } 
            return "";
    }
    int64_t find_label(std::string target_label, std::vector<Instruction*>* instructions){
        for (int i = 0; i < instructions->size(); i++){
            if ((*instructions)[i]->get_name() == "Instruction_label"){
                Instruction_label* label_instr = (Instruction_label*) (*instructions)[i];
                InstructionLabel* label_item = (InstructionLabel*) label_instr->get_label();
                if (label_item->to_string() == target_label){
                    return i;
                }
            }
        }
        return -1;
    }

    void gen_kill(Node* n){
        Instruction* in = n->instr;
        // std::cout << "\nxdxd\n";
        // std::cout << in->get_name();
        if (in->get_name() == "Instruction_ret"){
            n->gen.insert("rax");
            n->gen.insert("r12");
            n->gen.insert("r13");
            n->gen.insert("r14");
            n->gen.insert("r15");
            n->gen.insert("rbx");
            n->gen.insert("rbp");
        }   else if (in->get_name() == "Instruction_cjump"){
            Instruction_cjump* i = (Instruction_cjump*) in;
            Item* first = i->get_first();
            Item* second = i->get_second();
            if (check_item(first) != "" && check_item(first) != "rsp"){
                n->gen.insert(check_item(first));
            }
            if (check_item(second) != "" && check_item(second) != "rsp"){
                n->gen.insert(check_item(second));
            }
        } else if (in->get_name() == "Instruction_call_u"){
            Instruction_call_u* i = (Instruction_call_u*) in;
            Item* item = i->get_u();
            InstructionNumber* num = (InstructionNumber*) i->get_num();
            int num_val = stoi(num->to_string());
            if (check_item(item) != ""){
                n->gen.insert(check_item(item));
            }
            for (int i = 0; i < std::min(num_val, (int)argument_registers.size()); i++){
                n->gen.insert(argument_registers[i]);
                // std::cout << "looping arg register\n";
            }
            n->kill.insert("rax");
            n->kill.insert("rcx");
            n->kill.insert("rdx");
            n->kill.insert("rdi");
            n->kill.insert("rsi");
            n->kill.insert("r8");
            n->kill.insert("r9");
            n->kill.insert("r10");
            n->kill.insert("r11");

        } else if (in->get_name() == "Instruction_pp"){
            Instruction_pp* i = (Instruction_pp*) in;
            Item* item = i->get_reg();
            if (check_item(item) != ""){
                n->gen.insert(check_item(item));
                n->kill.insert(check_item(item));
            }
        } else if (in->get_name() == "Instruction_mm"){
            Instruction_mm* i = (Instruction_mm*) in;
            Item* item = i->get_reg();
            if (check_item(item) != ""){
                n->gen.insert(check_item(item));
                n->kill.insert(check_item(item));
            }
        } else if (in->get_name() == "Instruction_at"){
            Instruction_at* i = (Instruction_at*) in;
            Item* item = i->get_reg1();
            Item* item2 = i->get_reg2();
            Item* item3 = i->get_reg3();
            if (check_item(item) != ""){
                n->kill.insert(check_item(item));
            }
            if (check_item(item2) != ""){
                n->gen.insert(check_item(item2));
            }
            if (check_item(item3) != ""){
                n->gen.insert(check_item(item3));
            }
        } else if (in->get_name() == "Instruction_cmp_assignment"){
            Instruction_cmp_assignment* i = (Instruction_cmp_assignment*) in;
            Item* item = i->get_dst();
            Item* item2 = i->get_first();
            Item* item3 = i->get_second();
            if (check_item(item) != ""){
                n->kill.insert(check_item(item));
            }
            if (check_item(item2) != "" && check_item(item2) != "rsp"){
                n->gen.insert(check_item(item2));
            }
            if (check_item(item3) != "" && check_item(item2) != "rsp"){
                n->gen.insert(check_item(item3));
            }
        }  else if (in->get_name() == "Instruction_function_assignment"){
            // std::cerr << "function assignment\n";
            Instruction_function_assignment* i = (Instruction_function_assignment*) in;
            Item* item = i->get_dst();
            
            if (check_item(item) != ""){
                n->kill.insert(check_item(item));
            }
        } else if (in->get_name() == "Instruction_stackarg_assignment"){
            Instruction_stackarg_assignment* i = (Instruction_stackarg_assignment*) in;
            Item* item = i->get_dst();
            if (check_item(item) != ""){
                n->kill.insert(check_item(item));
            }
        } else if (in->get_name() == "Instruction_assignment"){
            Instruction_assignment* i = (Instruction_assignment*) in;
            // std::cout << "herererer\n\n";
            

            Item* first = i->get_dst();
            // std::cout << check_item(first) << "\n";
            Item* second = i->get_src();
            if (check_item(first) != ""){
                n->kill.insert(check_item(first));
            }
            if (check_item(second) != "" && check_item(second) != "rsp"){
                n->gen.insert(check_item(second));
            }
        } else if (in->get_name() == "Instruction_mem_op_load"){
            Instruction_mem_op_load* i = (Instruction_mem_op_load*) in;
            Item* first = i->get_dst();
            Item* second = i->get_src();
            if (check_item(first) != ""){
                n->kill.insert(check_item(first));
                n->gen.insert(check_item(first));
            }
            if (check_item(second) != "" && check_item(second) != "rsp"){
                n->gen.insert(check_item(second));
            }
        } else if (in->get_name() == "Instruction_mem_load"){
            Instruction_mem_load* i = (Instruction_mem_load*) in;
            Item* first = i->get_dst();
            Item* second = i->get_src();
            if (check_item(first) != ""){
                n->kill.insert(check_item(first));
            }
            if (check_item(second) != "" && check_item(second) != "rsp"){
                n->gen.insert(check_item(second));
            }
        } else if (in->get_name() == "Instruction_mem_op_store"){
            Instruction_mem_op_store* i = (Instruction_mem_op_store*) in;
            Item* first = i->get_src();
            Item* second = i->get_x_reg();
            if (check_item(first) != ""){
                n->gen.insert(check_item(first));
            }
            if (check_item(second) != "" && check_item(second) != "rsp"){
                n->gen.insert(check_item(second));
            }
        } else if (in->get_name() == "Instruction_mem_store"){
            Instruction_mem_store* i = (Instruction_mem_store*) in;
            Item* first = i->get_src();
            
            Item* second = i->get_x_reg();
            if (check_item(first) != ""){
                n->gen.insert(check_item(first));
            }
            if (check_item(second) != "" && check_item(second) != "rsp"){
                n->gen.insert(check_item(second));
            }
        } else if (in->get_name() == "Instruction_aop"){
            Instruction_aop* i = (Instruction_aop*) in;
            Item* item = i->get_dst();
            Item* item2 = i->get_src();
            if (check_item(item) != ""){
                n->kill.insert(check_item(item));
                n->gen.insert(check_item(item));
            }
            if (check_item(item2) != "" && check_item(item2) != "rsp"){
                n->gen.insert(check_item(item2));
                
            }
        } else if (in->get_name() == "Instruction_sop"){
            Instruction_sop* i = (Instruction_sop*) in;
            Item* item = i->get_dst();
            Item* item2 = i->get_src();
            if (check_item(item) != ""){
                n->kill.insert(check_item(item));
                n->gen.insert(check_item(item));
            }
            if (check_item(item2) != "" && check_item(item2) != "rsp"){
                n->gen.insert(check_item(item2));
                
            }
        } else if (in->get_name() == "Instruction_call_function"){
            // std::cerr << "big func\n";
            Instruction_call_function* i = (Instruction_call_function*) in;
            InstructionNumber* num = (InstructionNumber*)i->get_number();
            int num_val = stoi(num->to_string());
            
            for (int i = 0; i < std::min(num_val, (int)argument_registers.size()); i++){
                n->gen.insert(argument_registers[i]);
                
            }
            
            n->kill.insert("rax");
            n->kill.insert("rcx");
            n->kill.insert("rdx");
            n->kill.insert("rdi");
            n->kill.insert("rsi");
            n->kill.insert("r8");
            n->kill.insert("r9");
            n->kill.insert("r10");
            n->kill.insert("r11");
            

        } else if (in->get_name() == "Instruction_call_print"){
            Instruction_call_print* i = (Instruction_call_print*) in;
            int num_val = 1;
            for (int i = 0; i < std::min(num_val, (int)argument_registers.size()); i++){
                n->gen.insert(argument_registers[i]);
            }
            n->kill.insert("rax");
            n->kill.insert("rcx");
            n->kill.insert("rdx");
            n->kill.insert("rdi");
            n->kill.insert("rsi");
            n->kill.insert("r8");
            n->kill.insert("r9");
            n->kill.insert("r10");
            n->kill.insert("r11");


        } else if (in->get_name() == "Instruction_call_allocate"){
            Instruction_call_allocate* i = (Instruction_call_allocate*) in;
            int num_val = 2;
            for (int i = 0; i < std::min(num_val, (int)argument_registers.size()); i++){
                n->gen.insert(argument_registers[i]);
            }
            n->kill.insert("rax");
            n->kill.insert("rcx");
            n->kill.insert("rdx");
            n->kill.insert("rdi");
            n->kill.insert("rsi");
            n->kill.insert("r8");
            n->kill.insert("r9");
            n->kill.insert("r10");
            n->kill.insert("r11");


        } else if (in->get_name() == "Instruction_call_input"){
            Instruction_call_input* i = (Instruction_call_input*) in;
            int num_val = 0;
            for (int i = 0; i < std::min(num_val, (int)argument_registers.size()); i++){
                n->gen.insert(argument_registers[i]);
            }
            n->kill.insert("rax");
            n->kill.insert("rcx");
            n->kill.insert("rdx");
            n->kill.insert("rdi");
            n->kill.insert("rsi");
            n->kill.insert("r8");
            n->kill.insert("r9");
            n->kill.insert("r10");
            n->kill.insert("r11");


        } else if (in->get_name() == "Instruction_call_tensor_error"){
            Instruction_call_tensor_error* i = (Instruction_call_tensor_error*) in;
            InstructionNumber* num = (InstructionNumber*)i->get_val();
            int num_val = stoi(num->to_string());
            for (int i = 0; i < std::min(num_val, (int)argument_registers.size()); i++){
                n->gen.insert(argument_registers[i]);
            }
            n->kill.insert("rax");
            n->kill.insert("rcx");
            n->kill.insert("rdx");
            n->kill.insert("rdi");
            n->kill.insert("rsi");
            n->kill.insert("r8");
            n->kill.insert("r9");
            n->kill.insert("r10");
            n->kill.insert("r11");


        }
        
    }
    bool 
    in_out(std::vector<Node*>* nodes){
        bool change = false;
        
        for (int i = nodes->size()-1; i > -1; i--){
            std::set<std::string> in = {};
            std::set<std::string> out = {};
            Node* cur_node = (*nodes)[i];
            for (int64_t succ: cur_node->succs){
                // std::cout << "successor for row " << i << "is " << succ << "\n";
                for (std::string ele_in_suc: (*nodes)[succ]->in){
                    out.insert(ele_in_suc);
                }
            }
            
            
            for (auto gen_item: cur_node->gen){
                in.insert(gen_item);
            }
            for (auto out_item : cur_node->out){
                // std::cout << out_item << "\n";
                if(cur_node->kill.find(out_item) == cur_node->kill.end()){
                    in.insert(out_item);
                }
            }
            if (out != cur_node->out){
                change = true;
                cur_node->out = out;
            }
            if (in != cur_node->in){
                change = true;
                cur_node->in = in;
            }
            
            
        }
        return change;
        
    }
    

    std::vector<Node*> 
    generate_liveness(Function* func, bool print_std){
        std::vector<Instruction*> instructs = func->instructions;

        std::vector<Node*> nodes (instructs.size());
        
        for(int64_t i = 0; i < instructs.size(); ++i){
            nodes[i] = new Node(instructs[i]);
        }
        for(int64_t i = 0; i < instructs.size(); ++i){
            Instruction* in = instructs[i];

            if(in->get_name() == "Instruction_goto"){
                Instruction_goto* goto_label = (Instruction_goto*) in;
                InstructionLabel* label = (InstructionLabel*) goto_label->get_label();
                std::string label_name = label->to_string();
                int64_t succ_position = find_label(label_name, &instructs);
                nodes[i]->succs.insert(succ_position);
                nodes[succ_position]->preds.insert(i);
            } else if (in->get_name() == "Instruction_ret" || in->get_name() == "Instruction_call_tensor_error"){

            } else if (in->get_name() == "Instruction_cjump"){
                Instruction_cjump* cjump_instr = (Instruction_cjump*) in;
                InstructionLabel* label = (InstructionLabel*) cjump_instr->get_label();
                std::string label_name = label->to_string();
                int64_t succ_position = find_label(label_name, &instructs);
                nodes[i]->succs.insert(succ_position);
                nodes[i]->succs.insert(i+1);
                nodes[succ_position]->preds.insert(i);
                nodes[i+1]->preds.insert(i);
                // std::cout << "succ position: " << succ_position << "\n";
                // std::cout << "pred position: " << i << "\n";
            } else {
                nodes[i]->succs.insert(i+1);
                nodes[i+1]->preds.insert(i);
                // std::cout << "succ position: " << i+1 << "\n";
                // std::cout << "pred position: " << i << "\n";
            }
        }
        
        // std::cout << "gen kill\n";
        //gen kill sets
        for (Node* n : nodes){
            gen_kill(n);
            // std::cerr << "loop gen kill\n";
        }
        // std::cerr << "before in out\n";
        //in out sets
        while (in_out(&nodes)){
            // std::cout << "looped once\n";
            continue;
        }
        
        for (int i = 0; i < instructs.size(); i++){
            // std::cout << "\n" << instructs[i]->get_name() << "\n";
            // std::cout << "pred for row " << i << "\n";
            // for (int64_t num: nodes[i]->preds){
            //     std::cout << num << ' ';
            // }
            // std::cout << "\n\n";
            // std::cout << "succs for row " << i << "\n";
            // for (int64_t num: nodes[i]->succs){
            //     std::cout << num << " " ;
            // }
            // std::cout << "\n\n";
        }
        // for (int i = 0; i < instructs.size(); i++){
        //     std::cout << "gen for row " << i << "\n";
        //     for (auto num: nodes[i]->gen){
        //         std::cout << num << ' ';
        //     }
        //     std::cout << "\n\n";
        //     std::cout << "kill for row " << i << "\n";
        //     for (auto num: nodes[i]->kill){
        //         std::cout << num << " " ;
        //     }
        //     std::cout << "\n\n";
        // }
        // std::cerr << "came here\n";
        if (print_std){
            std::cout << "(\n(in\n";
        for (int i = 0; i < instructs.size(); i++){
            // std::cout << "in for row " << i << "\n";
            
            int in_counter = 0;
            if(in_counter == nodes[i]->in.size()){
                std::cout << "()\n";
            } else {
                std::cout << "(";
            
                for (std::string num: nodes[i]->in){
                    std::cout << num;
                    if(in_counter < nodes[i]->in.size()-1){
                        std::cout << " ";
                    }
                    in_counter++;
                }
                std::cout << ")\n";
            }
        }
        std::cout << ")\n";
        std::cout << "\n(out\n";
        for (int i = 0; i < instructs.size(); i++){
            // std::cout << "in for row " << i << "\n";
            
            int out_counter = 0;
            if(out_counter == nodes[i]->out.size()){
                std::cout << "()\n";
            } else {
                std::cout << "(";
            
            
                for (std::string num: nodes[i]->out){
                    std::cout << num;
                    if(out_counter < nodes[i]->out.size()-1){
                        std::cout << " ";
                    }
                    out_counter++;
                }
                std::cout << ")\n";
            }
        }
        std::cout << ")\n";
        std::cout << "\n)";

        }
        
        return nodes;
    }

}