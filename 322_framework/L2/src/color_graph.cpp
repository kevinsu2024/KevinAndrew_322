#include <color_graph.h>
#include <interference_generator.h>
#include <spill_generator.h>
#include <iostream>

using namespace std;

namespace L2{
    std::vector<std::string> ordered_gp_registers{"rdi", "rsi", "rax", "rdx", "rcx", "r8", "r9", "r10",
                                        "r11", "r12", "r13", "r14", "r15", "rbx", "rbp"};

    std::set<std::string> unordered_gp_registers{"rdi", "rsi", "rax", "rdx", "rcx", "r8", "r9", "r10",
                                        "r11", "r12", "r13", "r14", "r15", "rbx", "rbp"};


    int64_t
    check_degree(std::set<std::string> neighbors, std::set<std::string> nodes_in_graph){
        int64_t ctr = 0;
        for(auto neigh : neighbors){
            if(unordered_gp_registers.find(neigh) != unordered_gp_registers.end()){
                ctr++;
            } else if (nodes_in_graph.find(neigh) != nodes_in_graph.end()){
                ctr++;
            }
        }
        return ctr;
    }
    
    std::string
    get_largest(std::set<std::string> nodes_in_graph, int64_t thresh, std::unordered_map<std::string, std::set<std::string>> int_graph){
        
        std::string largest = "";
        int64_t max_degree = -1;
        for(auto e : nodes_in_graph){
            int64_t degree = check_degree(int_graph[e], nodes_in_graph);
            if(degree > max_degree){
                if(thresh == -1 || degree < thresh){
                    max_degree = degree;
                    largest = e;
                }
            }
        }
        return largest;
    }


    std::string
    get_color(std::set<std::string> neighbors, std::set<std::string> nodes_in_graph, std::unordered_map<std::string, std::string> mapping){
        // std::cout << "neighbors\n";
        // for(auto n : neighbors){
        //     std::cout << " " << n << " ";
        // }
        // std::cout << "\n";

        // std::cout << "nodes in graph\n";
        // for(auto n : nodes_in_graph){
        //     std::cout << " " << n << " ";
        // }
        // std::cout << "\n";
        
        
        std::set<std::string> taken;
        for(auto neigh : neighbors){
            // std::cout << "\n" << neigh << " is found in graph " << (nodes_in_graph.find(neigh) != nodes_in_graph.end()) << "\n";
            if(nodes_in_graph.find(neigh) != nodes_in_graph.end()) {
                // std::cout << mapping[neigh] << " is bein gput into taken \n";
                taken.insert(mapping[neigh]);
            }
        }

        // std::cout << "nodes in taken\n";
        // for(auto n : taken){
        //     std::cout << " " << n << " ";
        // }
        // std::cout << "\n";

        
        for(int64_t i = 0; i < ordered_gp_registers.size(); i++){
            std::string reg = ordered_gp_registers[i];
            if(taken.find(reg) == taken.end())return reg;
        }
        return "";
    }

    Function*
    spill_all_vars(Function* f){
        Function* currentF = f;
        auto int_graph = generate_interference(currentF,false);
        std::set<std::string> all_vars;
        for(auto p : int_graph){
            if(unordered_gp_registers.find(p.first) == unordered_gp_registers.end()){
                all_vars.insert(p.first);
            }
        }
        for(auto var : all_vars){
            currentF = generate_spill_code(currentF,var,var+"_spilled_");
        }
        return currentF;
    }

    std::pair<std::unordered_map<std::string, std::string>, Function*>
    generate_mapping(Function* f){
        bool done = false;
        std::unordered_map<std::string, std::string> mapping;
        
        Function* currentF = f;
        while(!done)
        {
            for(auto reg : unordered_gp_registers){
                mapping[reg] = reg;
            }

            std::cout << "\n\n current func is " << currentF->to_string();
            std::set<std::string> nodes_in_graph;

            //generate interference graph and add all non gp registers to nodes_in_graph set
            auto int_graph = generate_interference(currentF,false);
            for(auto p : int_graph){
                if(unordered_gp_registers.find(p.first) != unordered_gp_registers.end()) continue;
                nodes_in_graph.insert(p.first);
            }

            std::cout << "\n interference graph is : \n";
            for (auto p : int_graph){
                if(unordered_gp_registers.find(p.first) != unordered_gp_registers.end()) continue;
                std::cout << p.first << ": ";
                for (auto e : p.second){
                    std::cout << e << " ";
                }
                std::cout << "\n";
            }

            std::vector<std::string> node_stack;

            //removing nodes with less than 15 edges and adding to stack
            while(get_largest(nodes_in_graph, 15, int_graph) != ""){
                std::string next_node = get_largest(nodes_in_graph, 15, int_graph);
                std::cout << "next node is " << next_node << "\n";
                nodes_in_graph.erase(next_node);
                node_stack.push_back(next_node);
            }

            //removing the rest of the nodes in descneding order
            while(nodes_in_graph.size() > 0){
                std::string largest = get_largest(nodes_in_graph, -1, int_graph);
                nodes_in_graph.erase(largest);
                node_stack.push_back(largest);
            }


            

            std::cout << "nodes in stack are \n";
            for(auto e : node_stack){
                std::cout<< e << " ";
            }
            std::cout << "\n";

            //start loading nodes into graph and assigning colors
            std::set<std::string> failed_nodes;
            for(auto reg : unordered_gp_registers){
                nodes_in_graph.insert(reg);
            }

            std::cout << "nodes in graph are \n";
            for(auto e : nodes_in_graph){
                std::cout<< e << " ";
            }
            std::cout << "\n";


            while(node_stack.size() > 0){
                std::string node = node_stack.back();
                node_stack.pop_back();
                std::cout << "\n assigning color for " << node << "\n";
                std::string color = get_color(int_graph[node], nodes_in_graph, mapping);
                std::cout << "color is " << color << "\n";
                if(color == ""){
                    failed_nodes.insert(node);
                } 
                else{
                    nodes_in_graph.insert(node);
                    mapping[node] = color;
                }

            }
            std::cout << "failed noes are \n";
            for(auto reg : failed_nodes){
                std::cout << reg << " ";
            }
            std::cout << "\n";
            if(failed_nodes.size() == 0){
                done = true;
                return std::make_pair(mapping, currentF);
            }


            //if need to spill spill
            //first check if all already spilled
            bool all_spilled = true;
            for(auto var : failed_nodes){
                if(var.find("_spilled_") == std::string::npos) all_spilled = false;
            }
            if(failed_nodes.size() > 0 && all_spilled){
                std::cout << "spilling all\n";
                currentF = spill_all_vars(f);
                failed_nodes.clear();
            }

            for(auto var : failed_nodes){
                if(var.find("_spilled_") != std::string::npos) continue;
                std::cout << "\nspilling var " << var << "\n";
                currentF = generate_spill_code(currentF,var, (var+ "_spilled_"));
                std::cout << "finished spilling\n";
            }
            mapping.clear();

            
        }

        return std::make_pair(mapping, currentF);
    }


    std::string
    translate_to_code(std::unordered_map<std::string, std::string> map, Function* f){
        //change stackarg instructions first
        std::cout << "\n\nprint f to change\n";
        std::cout << f->to_string();
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
        f->instructions = instructions;
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