#include <multidim_array.h>

namespace IR{
    void
    translate_length(Item* variable, Item* array, Item* dimension, std::ofstream& out){
        int64_t dim = std::stoi(dimension->to_string());
        std::string array_name = array->to_string();
        std::string variable_name = array->to_string();
        out << "\t\t%offset <- 8\n";
        int64_t offset = dim * 8;
        out << "\t\t%offset <- %offset + " << std::to_string(offset) << "\n";
        out << "\t\t%address <- " << array_name << " + %offset\n";
        out << "\t\t" << variable_name << " <- load %address\n";
    }
    void
    translate_new_array(Item* array, std::vector<Item*> dimensions, std::ofstream& out){
        std::string array_name = array->to_string();
        for (int i = 0; i <  dimensions.size(); i++){
            out << "\t\t" << dimensions[i]->to_string() << "D <- " << dimensions[i]->to_string() << " >> 1";
        }
        out << "\t\t ";
        // out << "\t\t" <<
    }
    
}