#include "IR.h"


namespace IR{
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
    item_name = s;
}

void
Item::set_string(std::string s){
    item_str = s;
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

FunctionType::FunctionType(std::string s){
    set_name("FunctionType");
    set_string(s);
    return;
}


Type::Type(std::string s){
    set_name("Type");
    set_string(s);
    return;
}

StandardLibrary::StandardLibrary(std::string stl){
    set_name("StandardLibrary");
    set_string(stl);
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

Instruction_declaration::Instruction_declaration(Item* t, Item* v){
    set_name("Instruction_declaration");
    set_string(t->to_string() + " " + v->to_string());
    var_type = t;
    var = v;
}

Item*
Instruction_declaration::get_var_type(){
    return var_type;
}

Item*
Instruction_declaration::get_var(){
    return var;
}



Instruction_assignment::Instruction_assignment(Item* var, Item* s){
    set_name("Instruction_assignment");
    set_string(var->to_string() + " <- " + s->to_string());
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
    return this->var;
}


Instruction_cmp::Instruction_cmp(Item* var, Item* t1, Item* op, Item* t2){
    set_name("Instruction_cmp");
    set_string(var->to_string() + " <- " + t1->to_string() + " " + op->to_string() + " " + t2->to_string());
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
    set_string(var->to_string() + " <- " + t1->to_string() + " " + op->to_string() + " " + t2->to_string());
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

Instruction_bracket::Instruction_bracket(Item* item){
    index = item;
    set_name("Instruction_bracket");
    set_string("[" + item->to_string() + "]");
}

Item*
Instruction_bracket::get_index(){
    return index;
}

Instruction_load::Instruction_load(Item* d, Item* s, std::vector<Item*> i){
    set_name("Instruction_load");
    std::string st = (d->to_string() + " <- " + s->to_string());
    for(Item* ele : i){
        st += ("[" + ele->to_string() + "]");
    }
    set_string(st);
    this->var_dst = d;
    this->var_src = s;
    indices = i;
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

std::vector<Item*>
Instruction_load::get_indices(){
    return indices;
}

Instruction_store::Instruction_store(Item* var, std::vector<Item*> i, Item* s){
    set_name("Instruction_store");
    std::string st = var->to_string();
    for(Item* ele : i){
        st += ("[" + ele->to_string() + "]");
    }
    st += " <- " + s->to_string();
    set_string(st);
    this->var = var;
    this->s = s;
    indices = i;
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

std::vector<Item*>
Instruction_store::get_indices(){
    return indices;
}



Instruction_array_length::Instruction_array_length(Item* d, Item* s, Item* i){
    set_name("Instruction_array_length");
    set_string(d->to_string() + " <- length " + s->to_string() + " " + i->to_string());
    dst_var = d;
    src_var = s;
    index = i;
}

Item*
Instruction_array_length::get_dst_var(){
    return dst_var;
}

Item*
Instruction_array_length::get_src_var(){
    return src_var;
}

Item*
Instruction_array_length::get_index(){
    return index;
}



Instruction_tuple_length::Instruction_tuple_length(Item* d, Item* s){
    set_name("Instruction_tuple_length");
    set_string(d->to_string() + " <- length " + s->to_string());
    dst_var = d;
    src_var = s;
}

Item*
Instruction_tuple_length::get_dst_var(){
    return dst_var;
}

Item* Instruction_tuple_length::get_src_var(){
    return src_var;
}



Instruction_call::Instruction_call(Item* callee, std::vector<Item*> args){
    set_name("Instruction_call");
    std::string IR_string = "call " + callee->to_string() + "(";
    for (int i = 0; i < args.size(); i++){
        IR_string += args[i]->to_string();
        if (i < args.size() - 1){
            IR_string += ", ";
        }
        IR_string += ")";
    }
    set_string(IR_string);
    this->callee = callee;
    this->args = args;
}

Item*
Instruction_call::get_callee(){
    return callee;
}

std::vector<Item*>
Instruction_call::get_args(){
    return args;
}



Instruction_call_assignment::Instruction_call_assignment(Item* var, Item* callee, std::vector<Item*> args){
    set_name("Instruction_call_assignment");
    std::string IR_string = var->to_string() + " <- call " + callee->to_string() + "(";
    for (int i = 0; i < args.size(); i++){
        IR_string += args[i]->to_string();
        if (i < args.size() - 1){
            IR_string += ", ";
        }
        IR_string += ")";
    }
    set_string(IR_string);
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

std::vector<Item*>
Instruction_call_assignment::get_args(){
    return args;
}



Instruction_create_array::Instruction_create_array(Item* d, std::vector<Item*> ar){
    set_name("Instruction_create_array");
    std::string s = (d->to_string() + " <- new Array(");
    for(auto ele : ar){
        s += (ele->to_string() + ", ");
    }
    s = s.substr(0, s.size() - 2);
    s += ")";
    set_string(s);

    dst_var = d;
    args = ar;
}

Item*
Instruction_create_array::get_dst_var(){
    return dst_var;
}

std::vector<Item*>
Instruction_create_array::get_args(){
    return args;
}



Instruction_create_tuple::Instruction_create_tuple(Item* d, Item* s){
    set_name("Instruction_create_tuple");
    set_string(d->to_string() + " <- new Tuple(" + s->to_string() + ")");
    dst_var = d;
    size = s;
}

Item*
Instruction_create_tuple::get_dst_var(){
    return dst_var;
}

Item*
Instruction_create_tuple::get_size(){
    return size;
}



Instruction_return::Instruction_return(){
    set_name("Instruction_return");
    set_string("return");
    return;
}

Instruction_return_t::Instruction_return_t(Item* t){
    set_name("Instruction_return_t");
    set_string("return " + t->to_string());
    this->t = t;
    return;
}

Item*
Instruction_return_t::get_t(){
    return t;
}

Instruction_branch::Instruction_branch(Item* l){
    set_name("Instruction_branch");
    set_string("br " + l->to_string());
    this->label = l;
    return;
}

Item*
Instruction_branch::get_label(){
    return label;
}

Instruction_branch_t::Instruction_branch_t(Item* t, Item* l, Item* l2){
    set_name("Instruction_branch_t");
    set_string("br " + t->to_string() + " " + l->to_string() + " " + l2->to_string());
    this->t = t;
    this->label1 = l;
    label2 = l2;
}

Item*
Instruction_branch_t::get_t(){
    return t;
}

Item*
Instruction_branch_t::get_label1(){
    return label1;
}

Item*
Instruction_branch_t::get_label2(){
    return label2;
}

std::string
BasicBlock::to_string(){
    auto instrs = instructions;
    std::string ans = ("\t" + label->to_string() + "\n");
    for(auto instr : instrs){
        ans += "\t" + instr->to_string() + "\n";
    }
    ans += "\t" + end->to_string() + "\n";
    return ans;
}

std::string
Function::to_string(){
    auto blocks = basic_blocks;
    std::string ans = "define " + return_type + " " + name + "(";
    for(int64_t i = 0; i < types.size(); i++){
        ans += (types[i]->to_string() + " " + vars[i]->to_string() + ", ");
    }
    if(types.size() > 0) ans = ans.substr(0,ans.size()-2);
    ans += "){\n";
    for(auto block : blocks){
        ans += block->to_string();
    }
    ans += "}\n";
    return ans;
}

}