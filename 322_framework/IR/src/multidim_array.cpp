#include <multidim_array.h>

namespace IR{
    void
    translate_length(std::string variable_name, std::string array_name, std::string dimension, std::ofstream& out){
        int64_t dim = std::stoi(dimension);
        out << "\t\t%offset <- 8\n";
        int64_t offset = dim * 8;
        out << "\t\t%offset <- %offset + " << std::to_string(offset) << "\n";
        out << "\t\t%address <- " << array_name << " + %offset\n";
        out << "\t\t" << variable_name << " <- load %address\n";
    }
    void
    translate_new_array(std::string array_name, std::string dimension_1, std::string dimension_2, std::ofstream& out){
        int64_t dim1 = std::stoi(dimension_1);
        int64_t dim2 = std::stoi(dimension_2);
        out << "\t\t" << dimension_1 << "D <- " << dimension_1 << " >> 1";
        out << "\t\t" << dimension_2 << "D <- " << dimension_2 << " >> 1";
        // out << "\t\t" <<
    }
    
}