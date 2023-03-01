#pragma oncee

#include <LA.h>
#include <iostream>
#include <fstream>
#include <set>


namespace LA{
    void generate_code(Program p);
    void check_tensor_error(Instruction* in, Item* array, std::vector<Item*> indices, int64_t line_no, std::ofstream& out, std::string longest_label, int* ctr);
}