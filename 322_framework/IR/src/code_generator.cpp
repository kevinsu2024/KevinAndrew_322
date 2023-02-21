#include <code_generator.h>
#include <unordered_map>

namespace IR {

    void
    generate_code(Program p, bool verbose){

        std::ofstream outputFile;
        if (verbose){
            std::cerr << "printing now; testing to_string \n";
            for (auto f : p.functions){
                for (auto bb : f->basic_blocks){
                    for (auto i : bb->instructions){
                        outputFile << i->to_string() << "\n";
                    }
                    
                }
            }
        }
        
        // return;
    }
}