#include <code_generator.h>
#include <unordered_map>

namespace IR {

    bool
    is_te_instruction(Instruction* i) {
        return  i->get_name() == "Instruction_branch"   ||
                i->get_name() == "Instruction_branch_t" ||
                i->get_name() == "Instruction_label"    ||
                i->get_name() == "Instruction_return"   ||
                i->get_name() == "Instruction_return_t";
    }

    bool
    is_array_tuple_instruction(Instruction* i){
        return  i->get_name() == "Instruction_create_array"     ||
                i->get_name() == "Instruction_array_length"     ||
                i->get_name() == "Instruction_create_tuple"     ||
                i->get_name() == "Instruction_tuple_length"     ||
                i->get_name() == "Instruction_load"             ||
                i->get_name() == "Instruction_store";
                // i->get_name() == "Instruction_return_t";
    }
    void
    write_array_instruction(Instruction* i, std::ofstream& out){
        if (i->get_name() == "Instruction_create_array"){
            Instruction_create_array* ins = (Instruction_create_array*) i;
            translate_new_array(ins->get_dst_var(), ins->get_args(), out);
        }
    }
    void
    write_basic_instruction(Instruction* i, std::ofstream& out){
        out << "\t\t"<< i->to_string() << "\n";
    }
    void
    generate_code(Program p, bool verbose){

        std::ofstream outputFile;
        if (verbose){
            std::cerr << "printing now; \n";
            std::cerr << "no of functions: " << p.functions.size() << "\n";
        }
        
        for (auto f : p.functions){
            std::cerr << "no of basic blocks: " << f->basic_blocks.size() << "\n";
            auto traces = get_traces(f->basic_blocks, verbose);
            if (verbose){
                std::cerr << "traces has length " << traces.size() <<"\n";
            }
            for (auto trace: traces){
                for (auto bb : trace->blocks){
                    for (auto i : bb->instructions){
                        if (!is_array_tuple_instruction(i)){
                            write_basic_instruction(i, outputFile);
                        } else {
                            write_array_instruction(i, outputFile);
                        }
                    }
                }
            }
        }
        // return;
    }
}