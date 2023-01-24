#include <string>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <memory>
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
    int64_t find_label(std::string target_label, std::vector<Instruction*>* instructions){
        for (int i = 0; i < instructions->size(); i++){
            if ((*instructions)[i]->get_name() == "Instruction_label"){
                Instruction_label* label_instr = (Instruction_label*) (*instructions)[i];
                InstructionLabel* label_item = (InstructionLabel*) label_instr->get_label();
                if (label_item->get_label_name() == target_label){
                    return i;
                }
            }
        }
        return -1;
    }
    

    void generate_liveness(Program p){
        Function* func = p.functions.back();
        std::cerr << "\n\n func name is " << func->name << "\n";
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
                std::string label_name = label->get_label_name();
                int64_t succ_position = find_label(label_name, &instructs);
                nodes[i]->succs.insert(succ_position);
                nodes[succ_position]->preds.insert(i);
                std::cout << "succ position: " << succ_position << "\n";
                std::cout << "pred position: " << i << "\n";
            } else if (in->get_name() == "Instruction_ret" || in->get_name() == "Instruction_call_tensor_error"){

            } else if (in->get_name() == "Instruction_cjump"){
                Instruction_cjump* cjump_instr = (Instruction_cjump*) in;
                InstructionLabel* label = (InstructionLabel*) cjump_instr->get_label();
                std::string label_name = label->get_label_name();
                int64_t succ_position = find_label(label_name, &instructs);
                nodes[i]->succs.insert(succ_position);
                nodes[succ_position]->preds.insert(i);
                std::cout << "succ position: " << succ_position << "\n";
                std::cout << "pred position: " << i << "\n";
            } else {
                nodes[i]->succs.insert(i+1);
                nodes[i+1]->preds.insert(i);
                std::cout << "succ position: " << i+1 << "\n";
                std::cout << "pred position: " << i << "\n";
            }
        }
        for (int i = 0; i < instructs.size(); i++){
            std::cout << "pred for row " << i << "\n";
            for (int64_t num: nodes[i]->preds){
                std::cout << num << ' ';
            }
            std::cout << "\n\n";
            std::cout << "succs for row " << i << "\n";
            for (int64_t num: nodes[i]->succs){
                std::cout << num << " " ;
            }
            std::cout << "\n\n";
        }

        return;
    }

}