#include <trace.h>

namespace IR{
    BasicBlock*
    fetch_and_remove(std::vector<BasicBlock*>* blocks){ // pass pointer since we modify original list
        BasicBlock* return_block = blocks->back();
        blocks->pop_back();
        return return_block; // maybe change heuristic later
    }

    std::vector<BasicBlock*>
    get_succ(BasicBlock* bb, std::vector<BasicBlock*> blocks){ //currently trivial implementation; need to think about how to get successors
        std::vector<BasicBlock*> successors {};
        Instruction* end = bb->end;
        if (end->get_name() == "Instruction_return" || end->get_name() == "Instruction_return_t"){
            return successors;
        } else if (end->get_name() == "Instruction_branch"){
            Instruction_branch* lab_end = (Instruction_branch*) end;
            std::string label = lab_end->get_label()->to_string();
            for (auto block : blocks){
                if (block->label->to_string() == label){
                    successors.push_back(block);
                }

            }
        } else if (end->get_name() == "Instruction_branch_t"){
            Instruction_branch_t* lab_end = (Instruction_branch_t*) end;
            std::string label1 = lab_end->get_label1()->to_string();
            std::string label2 = lab_end->get_label2()->to_string();
            if (label1 == label2){
                label2 = "";
            }
            for (auto block : blocks){
                if (block->label->to_string() == label1 || block->label->to_string() == label2){
                    successors.push_back(block);
                }
            }
        }
        
        return successors;
    }
    bool
    profitable(BasicBlock* bb, BasicBlock* c){
        return bb->instructions.size() > c->instructions.size(); //trivial heuristic: check line no.
        //future heuristic: maybe no. of branches/conditional jumps in each basic block?
    }
    //might need a function that checks how many branches/conditional jumps in each basic block

    std::vector<Trace*>
    get_traces(std::vector<BasicBlock*> b){
        std::vector<BasicBlock*> blocks = b;
        std::vector<Trace*> traces;
        std::set<BasicBlock*> placed_blocks;
        while (blocks.size() > 0){
            Trace* newTrace = new Trace{};
            BasicBlock* bb = fetch_and_remove(&blocks);
            while(placed_blocks.find(bb) != placed_blocks.end()){
                placed_blocks.insert(bb);
                newTrace->blocks.push_back(bb);
                std::vector<BasicBlock*> succs = get_succ(bb, blocks);
                for (auto c : succs){
                    if (placed_blocks.find(c) == placed_blocks.end() && profitable(bb,c)){
                        bb = c;
                    }
                }
            }
        }
        

        return traces;
    }
    
}