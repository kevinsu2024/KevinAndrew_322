#include <color_graph.h>
#include <iostream>

using namespace std;

namespace L2{
    std::tuple<int64_t, std::unordered_map<std::string,std::string>, std::set<std::string>>
    assign_colors(std::unordered_map<std::string, std::set<std::string>> m, Function* f){
        std::vector<std::string> gp_registers{"rdi", "rax", "rsi", "rdx", "rcx", "r8", "r9", "r10",
                                        "r11", "r12", "r13", "r14", "r15", "rbx", "rbp"};
        std::set<std::string> reg_set(gp_registers.begin(),gp_registers.end());
        std::unordered_map<std::string, std::string> mapping;
        std::set<std::string> keys(gp_registers.begin(),gp_registers.end());
        std::vector<std::string> node_stack;
        std::set<std::string> uncolored;
        // std::cout << m.size() << "\n";
        for(auto ele : m){
            if(reg_set.find(ele.first) != reg_set.end()) continue;
            node_stack.push_back(ele.first);
        }
        // std::cout << m.size() << " " << node_stack.size() << "\n";


        //color registers as their own
        for(std::string reg : gp_registers){
            mapping[reg] = reg;
        }
        // std::cout << "\n" << mapping.size() << "\n";
        while (node_stack.size() > 0){
            std::string node = node_stack.back();
            node_stack.pop_back();
            std::set<std::string> unseen(gp_registers.begin(),gp_registers.end());
            for(std::string neigh : m[node]){
                if(mapping.find(neigh) != mapping.end() && unseen.find(mapping[neigh]) != unseen.end()){
                    unseen.erase(mapping[neigh]);
                }
            }
            if(unseen.size() == 0){
                uncolored.insert(node);
                continue;
            }
            std::string chosen_reg = *unseen.begin();
            mapping[node] = chosen_reg;

        }
        // std::cout << "\ndonedone\n";
        // std::cout << "num failed is " << uncolored.size() << "\n";
        // for(auto p : mapping){
        //     std::cout << p.first << " mapped with " << p.second << "\n";
        // }

        std::tuple<int64_t, std::unordered_map<std::string, std::string>, std::set<std::string>> ans = make_tuple(uncolored.size(), mapping, uncolored);
        return ans;
    }


    std::string
    translate_to_code(std::unordered_map<std::string, std::string> map, Function* f){
        //change stackarg instructions first
        auto instructions = f->instructions;
        for(int64_t i = 0; i < instructions.size(); i++){
            if(instructions[i]->get_name() == ("Instruction_stackarg_assignment")){
                Instruction_stackarg_assignment* in = (Instruction_stackarg_assignment*) instructions[i];
                int64_t arg_num = std::stoi(in->get_num()->to_string());
                Item* src = new Register(RegisterID::rsp);
                Item* num = new InstructionNumber(std::to_string(arg_num+(f->locals * 8)));
                Item* dest = in->get_dst();
                Instruction_mem_load* new_in = new Instruction_mem_load(dest,src,num);
                instructions.at(i) = new_in;
            }
        }
        std::string func = f->to_string();
        // std::cout << "\n f for this func is \n";
        // std::cout << func << "\n";
        for(auto p : map){
            if(p.first == p.second) continue;
            while(func.find(p.first) != std::string::npos){
                func.replace(func.find(p.first), p.first.size(), p.second);
            }
        }

        

        // std::cout << "\n f for this new func is \n";
        // std::cout << func << "\n";
        return func;
    }
}