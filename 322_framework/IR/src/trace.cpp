#include <trace.h>
#include <iostream>
namespace IR{
    void
    print_trace_(std::vector<Trace*> traces){
        for (auto trace: traces){
            for (auto bb: trace->blocks){
                std::cerr << bb->label->to_string() << "\n";
                for (auto i: bb->instructions){
                    std::cerr << i->to_string() << "\n";
                }
            }
        }
    }
    BasicBlock*
    fetch_and_remove(std::vector<BasicBlock*>* blocks){
        BasicBlock* return_block = blocks->back();
        blocks->pop_back();
        return return_block; 
    }

    std::vector<BasicBlock*>
    get_succ(BasicBlock* bb, std::vector<BasicBlock*> blocks){ 
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
        bool b_has_self_loop = false;
        bool c_has_self_loop = false;
        Instruction* b_end = bb->end;
        if (b_end->get_name() == "Instruction_branch_t"){
            Instruction_branch_t* b_lab_end = (Instruction_branch_t*) b_end;
            std::string b_label1 = b_lab_end->get_label1()->to_string();
            std::string b_label2 = b_lab_end->get_label2()->to_string();
            Item* b_label = bb->label;
            if (b_label->to_string() == b_label1 || b_label->to_string() == b_label2){
                b_has_self_loop == true;
            }
        }
        Instruction* c_end = c->end;
        if (c_end->get_name() == "Instruction_branch_t"){
            Instruction_branch_t* c_lab_end = (Instruction_branch_t*) c_end;
            std::string c_label1 = c_lab_end->get_label1()->to_string();
            std::string c_label2 = c_lab_end->get_label2()->to_string();
            Item* c_label = bb->label;
            if (c_label->to_string() == c_label1 || c_label->to_string() == c_label2){
                c_has_self_loop == true;
            }
        }

        return (b_has_self_loop && !c_has_self_loop) or bb->instructions.size() > c->instructions.size(); 
    }

    std::vector<Trace*>
    get_traces(std::vector<BasicBlock*> b, bool verbose){
        std::vector<BasicBlock*> blocks = b;
        std::sort(b.begin(), b.end(), [](const BasicBlock* lhs, const BasicBlock* rhs){
            return lhs->instructions.size() > rhs->instructions.size();
        });
        std::vector<Trace*> traces;
        std::unordered_set<BasicBlock*> placed_blocks;
        while (blocks.size() > 0){
            if (verbose) {
                std::cerr << "blocks is nonempty \n";
            }
            Trace* newTrace = new Trace{};
            BasicBlock* bb = fetch_and_remove(&blocks);
            while(placed_blocks.find(bb) == placed_blocks.end()){
                if (verbose){
                    std::cerr << "do not see current block in placed_blocks; perform trace action\n";
                }
                placed_blocks.insert(bb);
                newTrace->blocks.push_back(bb);
                std::vector<BasicBlock*> succs = get_succ(bb, blocks);
                for (auto c : succs){
                    if (placed_blocks.find(c) == placed_blocks.end() && profitable(bb,c)){
                        bb = c;
                    }
                }
            }
            if (newTrace->blocks.size() > 0){
                traces.push_back(newTrace);
            }
        }
        
        if (verbose){
            print_trace_(traces);
        }
        return traces;
    }
    
}