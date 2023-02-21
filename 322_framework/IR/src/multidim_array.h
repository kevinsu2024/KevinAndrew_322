#include <IR.h>
#include <iostream>
#include <fstream>
namespace IR{
    void translate_length(Item* variable, Item* array, Item* dimension, std::ofstream& out);
    void translate_new_array(Item* array, std::vector<Item*> entry_dimensions, std::ofstream& out);
    void translate_array_load_value(Item* array, std::vector<Item*> entry_dimensions, Item* load_value, std::ofstream& out);
    void translate_array_store_value(Item* array, std::vector<Item*> entry_dimensions, Item* store_value, std::ofstream& out);
    // void translate_tuple_load_value(std::string )
}