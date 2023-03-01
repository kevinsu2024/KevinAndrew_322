#include <basic_block_generator.h>

namespace LA{
    std::vector<Instruction*>
    get_basic_blocks(std::vector<Instruction*> old, std::string longest_name, std::string return_type){
        std::vector<Instruction*> new_instrs;
        std::set<std::string> terminator_instrs = {"Instruction_return", "Instruction_return_t", "Instruction_branch", "Instruction_branch_t"};
        int64_t ctr = 0;
        bool startBB = true;
        while(ctr < old.size()){
            Instruction* in = old[ctr];
            if(startBB){
                if(in->get_name() != "Instruction_label"){
                    Instruction* new_label_instr = new Instruction_label(new InstructionLabel(longest_name + "_temp_label_name_" + std::to_string(ctr)));
                    new_instrs.push_back(new_label_instr);
                }
                startBB = false;
            }
            else if(in->get_name() == "Instruction_label"){
                Instruction_label* instr = (Instruction_label*) in;
                Instruction* new_br_instr = new Instruction_branch(instr->get_label());
                new_instrs.push_back(new_br_instr);
            }
            new_instrs.push_back(in);
            if(terminator_instrs.find(in->get_name()) != terminator_instrs.end()) startBB = true;
            ctr += 1;
        }
        if(!startBB){
            Instruction* in;
            if(return_type == "void") in = new Instruction_return();
            else in = new Instruction_return_t(new InstructionNumber(0));
            new_instrs.push_back(in);
        }

        return new_instrs;
    }
}