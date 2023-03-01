#pragma oncee

#include <LA.h>
#include <iostream>
#include <fstream>
#include <set>
#include <basic_block_generator.h>


namespace LA{
    void generate_code(Program p);
    // int64_t check_tensor_error(std::vector<Instruction*>* ins, int64_t ins_ind, Instruction* in, Item* array, std::vector<Item*> indices, int64_t line_no, std::string longest_label, std::string longest_name);
}