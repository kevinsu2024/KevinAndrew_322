#include <interference_generator.h>

using namespace std;

namespace L2{
    std::unordered_map<std::string, std::set<std::string>> 
    generate_interference(Function* func, bool verbose){
        //Initialize nodes with liveness analysis generated
        std::unordered_map<std::string, std::set<std::string>> adj_list;
        std::vector<Node*> nodes = L2::generate_liveness(func, false); //don't want to print here CHANGE BACK
        std::vector<Instruction*> instructs = func->instructions;
        
        std::vector<std::string> gp_registers{"rdi", "rax", "rsi", "rdx", "rcx", "r8", "r9", "rbx", "rbp", "r10",
                                        "r11", "r12", "r13", "r14", "r15"};
        int64_t register_index = 0;
        int64_t instruction_index = 0;
        
        for(Node* node: nodes){

            //loop out set and connect each node with one another
            std::set<std::string> in_set = node->in;
            auto out_set = node->out;
            for(std::string val : in_set){
                if (adj_list.find(val) == adj_list.end()){
                    adj_list[val] = {}; //initialize key in map but don't store any vals in it
                }
                for(std::string val2 : in_set){
                    if(val == val2) continue;
                    adj_list[val].insert(val2);
                    adj_list[val2].insert(val);
                }
            }
            for(std::string val : out_set){
                if (adj_list.find(val) == adj_list.end()){
                    adj_list[val] = {}; //initialize key in map but don't store any vals in it
                }
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
        }

      
        for (Node* node: nodes){
            Instruction* instruction = instructs[instruction_index];
            if (instruction->get_name() == "Instruction_sop"){
                Instruction_sop* shift_instruction = (Instruction_sop*) instruction;
                Item* src_item = shift_instruction->get_src();
                if (src_item->get_name() == "Variable"){
                    Variable* src_var_item = (Variable*) src_item;
                    std::string src_var = src_var_item->to_string();
                    for (auto r: gp_registers){
                        if (r == "rcx"){
                            continue;
                        }
                        adj_list[src_var].insert(r);
                        adj_list[r].insert(src_var);
                    }
                    
                }

            }
            instruction_index++;
            
        }
        if(verbose){
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