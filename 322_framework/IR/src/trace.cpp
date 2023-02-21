#include <trace.h>

namespace IR{
    BasicBlock*
    fetch_and_remove(std::vector<BasicBlock*>* blocks){ // pass pointer since we modify original list
        BasicBlock* return_block = blocks->back();
        blocks->pop_back();
        return return_block; // maybe change heuristic later
    }

    std::vector<BasicBlock*>
    get_succ(BasicBlock* bb){ //currently trivial implementation; need to think about how to get successors
        std::vector<BasicBlock*> successors {bb};
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
                std::vector<BasicBlock*> succs = get_succ(bb);
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