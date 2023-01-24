#include <string>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <memory>
#include <liveness_generator.h>

using namespace std;

namespace L2{

    void generate_liveness(Program p){
        Function* func = p.functions.back();
        std::cerr << "\n\n func name is " << func->name << "\n";
        return;
    }

}