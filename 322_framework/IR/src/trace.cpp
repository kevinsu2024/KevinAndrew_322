#include <trace.h>

namespace IR{
    std::vector<Trace*>
    get_traces(std::vector<BasicBlock*> b){
        std::vector<BasicBlock*> blocks = b;
        std::vector<Trace*> traces;
        while(blocks.size() > 0){
            Trace* newTrace = new Trace{};
            BasicBlock* bb = blocks.back(); // maybe change this heuristic
            blocks.pop_back();
            newTrace->blocks.push_back(bb);
        }

        return traces;
    }
    
}