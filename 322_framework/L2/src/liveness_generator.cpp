#include <string>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <memory>
#include <liveness_generator.h>
#include <set>

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

    };
    void
    

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
            if(in->get_name() == 'Instruction_goto'){
                break;
            } else if ((in->get_name() == 'Instruction_ret') || (in->get_name() == 'Instruction_ret')
        }


        return;
    }

}