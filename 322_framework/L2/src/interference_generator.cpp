#include <interference_generator.h>

using namespace std;

namespace L2{
    std::unordered_map<std::string, std::set<std::string>> generate_interference(Function* func, bool print_std){
        //Initialize nodes with liveness analysis generated
        std::vector<Node*> nodes = L2::generate_liveness(func, false); //don't want to print here
        //get list of instructions(will use for the shift op case)
        // Function* func = p.functions.back();
        std::vector<Instruction*> instructs = func->instructions;
        //initialize interference graph
        std::unordered_map<std::string, std::set<std::string>> adj_list;
        //list of general purpose registers
        std::vector<std::string> gp_registers{"rdi", "rax", "rsi", "rdx", "rcx", "r8", "r9", "rbx", "rbp", "r10",
                                        "r11", "r12", "r13", "r14", "r15"};
        //current register id to connect to
        int64_t register_index = 0;
        //current line of instruction corresponding to node
        int64_t instruction_index = 0;
        //for each line of code
        
        for(Node* node: nodes){
           
            //current register string to connect to
            
            //loop in set and connect each node with one another
            // for (auto in_key: node->in){
            //     for (auto in_val: node->in){
            //         if (in_key == in_val){
            //             continue;
            //         }
            //         adj_list[in_key].insert(in_val);
            //         adj_list[in_val].insert(in_key);
            //         for (auto c: adj_list[in_val]){
            //             if (c == in_key){
            //                 continue;
            //             }
            //             adj_list[in_key].insert(c);
            //             adj_list[c].insert(in_key);
            //         } 
            //         for (auto c: adj_list[in_key]){
            //             if (c == in_val){
            //                 continue;
            //             }
            //             adj_list[in_val].insert(c);
            //             adj_list[c].insert(in_val);
            //         } 

            //     }
            // }
            

            //loop out set and connect each node with one another
            std::set<std::string> in_set = node->in;
            auto out_set = node->out;
            for(std::string val : in_set){
                for(std::string val2 : in_set){
                    if(val == val2) continue;
                    adj_list[val].insert(val2);
                    adj_list[val2].insert(val);
                }
            }
            for(std::string val : out_set){
                for(std::string val2 : out_set){
                    if(val == val2) continue;
                    adj_list[val].insert(val2);
                    adj_list[val2].insert(val);
                }
            }
            for(std::string val : gp_registers){
                for(std::string val2 : gp_registers){
                    if(val == val2) continue;
                    adj_list[val].insert(val2);
                    adj_list[val2].insert(val);
                }
            }
            for(std::string val : node->out){
                for(std::string val2 : node->kill){
                    if(val == val2) continue;
                    adj_list[val].insert(val2);
                    adj_list[val2].insert(val);
                }
            }
            // if(node->instr->get_name() == "Instruction_sop"){
            //     Instruction_sop* in = (Instruction_sop*) node;
            //     Item* second = in->get_src();
            //     std::string sname = second->get_name();
            //     if(second->get_name() == "Variable"){
            //         Variable* sec = (Variable*) second;
            //         for(std::string val : gp_registers){
            //             if(val == "rcx") continue;
            //             adj_list[val].insert(sec->get_variable_name());
            //             adj_list[sec->get_variable_name()].insert(val);
                        
            //          }
            //     }
            // }
            // std::cout << "herere";
            // for (auto out_key: node->out){
            //     for (auto out_val: node->out){
            //         if (out_key == out_val){
            //             continue;
            //         }
            //         adj_list[out_key].insert(out_val);
            //         adj_list[out_val].insert(out_key);
            //         for (auto c: adj_list[out_val]){
            //             if (c == out_key){
            //                 continue;
            //             }
            //             adj_list[out_key].insert(c);
            //             adj_list[c].insert(out_key);
            //         } 
            //         for (auto c: adj_list[out_key]){
            //             if (c == out_val){
            //                 continue;
            //             }
            //             adj_list[out_val].insert(c);
            //             adj_list[c].insert(out_val);
            //         } 
            //     }
            // }
        }

        //printing adj list here
        // std::cout << "\n\n\n\nprinting here\n\n\n\n";
        // if (true){
        //     for (auto key: adj_list){
        //         std::cout << key.first;
        //         for(std::string val: key.second){
        //             std::cout << " " << val;
        //         }
        //         std::cout << "\n";
        //     }
        // }
        // std::cout << "\n\n";

            //loop registers and connect cur_register to all other registers
        // std::string cur_register = "rdi";
        // for (auto r: gp_registers){
        //     if (adj_list.find(r) == adj_list.end()){
        //         cur_register = r;
        //     }
        // }
        // for (auto reg: gp_registers){
        //     if (reg == cur_register){
        //         continue;
        //     }
        //     adj_list[reg].insert(cur_register);
        //     adj_list[cur_register].insert(reg);
        //     for (auto c: adj_list[reg]){
        //         if (c == cur_register){
        //             continue;
        //         }
        //         adj_list[cur_register].insert(c);
        //         adj_list[c].insert(cur_register);
        //     } 
        //     for (auto c: adj_list[cur_register]){
        //         if (c == reg){
        //             continue;
        //         }
        //         adj_list[reg].insert(c);
        //         adj_list[c].insert(reg);
        //     } 
        // }
        // for (Node* node: nodes){
        //     //connect variables in kill with those in out
        //     for (auto kill_var: node->kill){
        //         for (auto out_var: node->out){
        //             if (kill_var == out_var){
        //                 continue;
        //             }
        //             adj_list[kill_var].insert(out_var);
        //             adj_list[out_var].insert(kill_var);
        //             for (auto c: adj_list[kill_var]){
        //                 if (c == out_var){
        //                     continue;
        //                 }
        //                 adj_list[out_var].insert(c);
        //                 adj_list[c].insert(out_var);
        //             } 
        //             for (auto c: adj_list[out_var]){
        //                 if (c == kill_var){
        //                     continue;
        //                 }
        //                 adj_list[kill_var].insert(c);
        //                 adj_list[c].insert(kill_var);
        //             } 
        //         }
        //     }
        // }
        for (Node* node: nodes){
            Instruction* instruction = instructs[instruction_index];
            //handle shift instruction case
            if (instruction->get_name() == "Instruction_sop"){
                //cast to shift op
                Instruction_sop* shift_instruction = (Instruction_sop*) instruction;
                //get src item and convert to get register/variable
                Item* src_item = shift_instruction->get_src();
                if (src_item->get_name() == "Variable"){
                    Variable* src_var_item = (Variable*) src_item;
                    std::string src_var = src_var_item->get_variable_name();
                    //connect src_var with all registers except rcx
                    for (auto r: gp_registers){
                        if (r == "rcx"){
                            continue;
                        }
                        adj_list[src_var].insert(r);
                        adj_list[r].insert(src_var);
                    }
                    
                }

            }
            //add to our indices
            // register_index++;
            instruction_index++;
            
        }

        // for (auto key: adj_list){
        //         std::cout << key.first;
        //         for(std::string val: key.second){
        //             std::cout << " " << val;
        //         }
        //         std::cout << "\n";
        //     }

        if (print_std){
            for (auto key: adj_list){
                std::cout << key.first;
                for(std::string val: key.second){
                    std::cout << " " << val;
                }
                std::cout << "\n";
            }
        }
        return adj_list;
    }
}