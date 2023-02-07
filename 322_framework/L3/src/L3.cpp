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
    return;
}

FunctionName::FunctionName(std::string f){
    set_name("FunctionName");
    set_string(f);
    return;
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


Instruction_assignment::Instruction_assignment(Item* var, Item* s){
    set_name("Instruction_assignment");
    set_string("TODO");
    this->var = var;
    this->s = s;
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


Instruction_cmp::Instruction_cmp(Item* var, Item* t1, Item* op, Item* t2){
    set_name("Instruction_cmp");
    set_string("TODO");
    this->var = var;
    this->t1 = t1;
    this->op = op;
    this->t2 = t2;
    return;
}
Item*
Instruction_cmp::get_var(){
    return var;
}

Item*
Instruction_cmp::get_t1(){
    return t1;
}

Item*
Instruction_cmp::get_op(){
    return op;
}

Item*
Instruction_cmp::get_t2(){
    return t2;
}

Instruction_op::Instruction_op(Item* var, Item* t1, Item* op, Item* t2){
    set_name("Instruction_op");
    set_string("TODO");
    this->var = var;
    this->t1 = t1;
    this->op = op;
    this->t2 = t2;
    return;
}

Item*
Instruction_op::get_var(){
    return var;
}

Item*
Instruction_op::get_t1(){
    return t1;
}

Item*
Instruction_op::get_op(){
    return op;
}

Item*
Instruction_op::get_t2(){
    return t2;
}

Instruction_load::Instruction_load(Item* var_dst, Item* var_src){
    set_name("Instruction_load");
    set_string("TODO");
    this->var_dst = var_dst;
    this->var_src = var_src;
    return;
}

Item*
Instruction_load::get_var_dst(){
    return var_dst;
}

Item*
Instruction_load::get_var_src(){
    return var_src;
}

Instruction_store::Instruction_store(Item* var, Item* ){
    set_name("Instruction_store");
    set_string("TODO");
    this->var = var;
    this->s = s;
    return;
}

Item*
Instruction_store::get_var(){
    return var;
}

Item*
Instruction_store::get_s(){
    return s;
}

Instruction_return::Instruction_return(){
    set_name("Instruction_return");
    set_string("TODO");
    return;
}

Instruction_return_t::Instruction_return_t(Item* t){
    set_name("Instruction_return_t");
    set_string("TODO");
    this->t = t;
    return;
}

Item*
Instruction_return_t::get_t(){
    return t;
}

Instruction_label::Instruction_label(Item* l){
    set_name("Instruction_label");
    set_string("TODO");
    this->label = l;
    return;
}

Item*
Instruction_label::get_label(){
    return label;
}

Instruction_break::Instruction_break(Item* l){
    set_name("Instruction_break");
    set_string("TODO");
    this->label = l;
    return;
}

Item*
Instruction_break::get_label(){
    return label;
}

Instruction_break_t::Instruction_break_t(Item* t, Item* l){
    set_name("Instruction_break_t");
    set_string("TODO");
    this->t = t;
    this->label = l;
}

Item*
Instruction_break_t::get_t(){
    return t;
}

Item*
Instruction_break_t::get_label(){
    return label;
}

Instruction_call::Instruction_call(Item* callee, std::vector<Item*> *args){
    set_name("Instruction_call");
    set_string("TODO");
    this->callee = callee;
    this->args = args;
}

Item*
Instruction_call::get_callee(){
    return callee;
}

std::vector<Item*>*
Instruction_call::get_args(){
    return args;
}

Instruction_call_assignment::Instruction_call_assignment(Item* var, Item* callee, std::vector<Item*>* args){
    set_name("Instruction_call_assignment");
    set_string("TODO");
    this->var = var;
    this->callee = callee;
    this->args = args;
}

Item*
Instruction_call_assignment::get_var(){
    return var;
}

Item*
Instruction_call_assignment::get_callee(){
    return callee;
}

std::vector<Item*>*
Instruction_call_assignment::get_args(){
    return args;
}

std::string
Function::to_string(){
    auto instrs = instructions;
    std::string ans = "(" + name + "\n";
    ans += "\t";
    // L2 ONLY: CHANGE LATER
    // ans += (std::to_string(arguments) + " " + std::to_string(locals) + "\n");
    for(Instruction* instr : instrs){
        ans += "\t" + instr->to_string() + "\n";
    }
    ans += ")\n";
    return ans;
}

}