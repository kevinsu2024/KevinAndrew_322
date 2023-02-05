#include "L3.h"


namespace L3{
std::string
Item::get_name(){
    return item_name;
}

std::string
Item::to_string(){
    return item_str;
}

void
Item::set_name(std::string s){
    item_str = s;
}

void
Item::set_string(std::string s){
    item_name = s;
}

Variable::Variable(std::string v){
    set_name("Variable");
    set_string(v);
    return;
}

InstructionLabel::InstructionLabel(std::string l){
    set_name("InstructionLabel");
    set_string(l);
    return;
}


InstructionNumber::InstructionNumber(std::string n){
    set_name("InstructionNumber");
    set_string(n);
    return;
}

CompareOp::CompareOp(std::string c){
    set_name("CompareOp");
    set_string(c);
    return;
}

Op::Op(std::string o){
    set_name("Op");
    set_string(o);
}

FunctionName::FunctionName(std::string f){
    set_name("FunctionName");
    set_string(f);
    return;
}

Callee::Callee(std::string c){
    set_name("Callee");
    set_string(c);
}



//Instructions
void
Instruction::set_name(std::string s){
    name = s;
}

std::string
Instruction::get_name(){
    return name;
}

void
Instruction::set_string(std::string s){
    instr = s;
}

std::string
Instruction::to_string() {
    return instr;
}


Instruction_assignment::Instruction_assignment(Item* v, Item* s1){
    set_name("Instruction_assignment");
    set_string("TODO");
    var = v;
    s = s1;
    return;
}

Item*
Instruction_assignment::get_s(){
    return s;
}

Item*
Instruction_assignment::get_var(){
    return var;
}


Instruction_cmp(item* v, Item* t1)

}