#include <IR.h>
#include <unordered_set>

namespace IR{
    struct Trace{
        std::vector<BasicBlock*> blocks;
    };
    BasicBlock* fetch_and_remove(std::vector<BasicBlock*>* blocks);
    std::vector<BasicBlock*> get_succ(BasicBlock* bb, std::vector<BasicBlock*> blocks);
    bool profitable(BasicBlock* bb, BasicBlock* c);
    std::vector<Trace*> get_traces(std::vector<BasicBlock*> b, bool verbose);
    
}