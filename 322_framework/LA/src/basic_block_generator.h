#include <LA.h>
#include <set>


namespace LA{
    std::vector<Instruction*>
    get_basic_blocks(std::vector<Instruction*> old, std::string longest_name, std::string return_type);

    std::vector<Instruction*>
    move_declarations(std::vector<Instruction*> old, std::string longest_label);
}