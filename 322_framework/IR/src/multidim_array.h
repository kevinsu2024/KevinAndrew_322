#include <IR.h>
#include <iostream>
#include <fstream>
namespace IR{
    void translate_length(std::string variable_name, std::string array_name, std::string dimension);
    void translate_new_array(std::string array_name, std::string dimension_1, std::string dimension_2);
    void translate_array_load_value(std::string array_name, std::vector<std::string> entry_dimensions, std::string load_value);
    void translate_array_store_value(std::string array_name, std::vector<std::string> entry_dimensions, std::string store_value);
    // void translate_tuple_load_value(std::string )
}