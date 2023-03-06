#include <LB.h>
#include <iostream>

namespace LB{
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

Name::Name(std::string n){
    set_name("Name");
    set_string(n);
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

Op::Op(std::string o){
    set_name("Op");
    set_string(o);
    return;
}

Cmp::Cmp(std::string c){
    set_name("Cmp");
    set_string(c);
    return;
}

Type::Type(std::string s){
    set_name("Type");
    set_string(s);
    return;
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
    return instr + "\n";
}

Instruction_declaration::Instruction_declaration(Item* t, std::vector<Item*> v){
    set_name("Instruction_declaration");
    std::string str = "";
    for (auto var : v){
        str += t->to_string() + " " + var->to_string() + "\n";
    }
    set_string(str);
    var_type = t;
    vars = v;
}

Item*
Instruction_declaration::get_var_type(){
    return var_type;
}

std::vector<Item*>
Instruction_declaration::get_vars(){
    return vars;
}



Instruction_assignment::Instruction_assignment(Item* var, Item* t){
    set_name("Instruction_assignment");
    set_string(var->to_string() + " <- " + t->to_string());
    this->var = var;
    this->t = t;
    return;
}

Item*
Instruction_assignment::get_t(){
    return t;
}

Item*
Instruction_assignment::get_var(){
    return this->var;
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


Instruction_cmp::Instruction_cmp(Item* var, Item* t1, Item* cmp, Item* t2){
    set_name("Instruction_cmp");
    set_string(var->to_string() + " <- " + t1->to_string() + " " + cmp->to_string() + " " + t2->to_string());
    this->var = var;
    this->t1 = t1;
    this->cmp = cmp;
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
Instruction_cmp::get_cmp(){
    return cmp;
}

Item*
Instruction_cmp::get_t2(){
    return t2;
}



Instruction_load::Instruction_load(Item* d, Item* s, std::vector<Item*> i, int64_t l){
    set_name("Instruction_load");
    std::string st = (d->to_string() + " <- " + s->to_string());
    for(Item* ele : i){
        st += ("[" + ele->to_string() + "]");
    }
    set_string(st);
    this->var_dst = d;
    this->var_src = s;
    this->line_no = l;
    this->indices = i;
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

int64_t
Instruction_load::get_line_no(){
    return line_no;
}

std::vector<Item*>
Instruction_load::get_indices(){
    return indices;
}

Instruction_store::Instruction_store(Item* var, std::vector<Item*> i, Item* s, int64_t l){
    set_name("Instruction_store");
    std::string st = var->to_string();
    for(Item* ele : i){
        st += ("[" + ele->to_string() + "]");
    }
    st += " <- " + s->to_string();
    set_string(st);
    this->var = var;
    this->s = s;
    this->indices = i;
    this->line_no = l;
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

int64_t
Instruction_store::get_line_no(){
    return line_no;
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



Instruction_call::Instruction_call(Item* callee, std::vector<Item*> args){
    set_name("Instruction_call");
    std::string LB_string = callee->to_string() + "(";
    for (int i = 0; i < args.size(); i++){
        LB_string += args[i]->to_string();
        if (i < args.size() - 1){
            LB_string += ", ";
        }
        
    }
    LB_string += ")";
    set_string(LB_string);
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
    std::string LB_string = var->to_string() + " <- " + callee->to_string() + "(";
    for (int i = 0; i < args.size(); i++){
        LB_string += args[i]->to_string();
        if (i < args.size() - 1){
            LB_string += ", ";
        }
    }
    LB_string += ")";
    set_string(LB_string);
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



Instruction_label::Instruction_label(Item* l){
    set_name("Instruction_label");
    set_string(l->to_string());
    this->label = l;
    return;
}

Item*
Instruction_label::get_label(){
    return label;
}



Instruction_return::Instruction_return(){
    set_name("Instruction_return");
    set_string("return");
    return;
}

Instruction_continue::Instruction_continue(std::string ln){
    set_name("Instruction_continue");
    set_string("continue");
    line_num = ln;
    return;
}

std::string
Instruction_continue::get_line_num(){
    return line_num;
}

Instruction_break::Instruction_break(std::string ln){
    set_name("Instruction_break");
    set_string("break");
    line_num = ln;
    return;
}

std::string
Instruction_break::get_line_num(){
    return line_num;
}

Instruction_open_brace::Instruction_open_brace(){
    set_name("Instruction_open_brace");
    set_string("{");
    return;
}

Instruction_close_brace::Instruction_close_brace(){
    set_name("Instruction_close_brace");
    set_string("}");
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

Instruction_goto::Instruction_goto(Item* l){
    set_name("Instruction_goto");
    set_string("goto " + l->to_string());
    this->label = l;
    return;
}

Item*
Instruction_goto::get_label(){
    return label;
}

Instruction_if::Instruction_if(Item* t1, Item* cmp, Item* t2, Item* l1, Item* l2){
    set_name("Instruction_if");
    set_string("if (" + t1->to_string() + " " + cmp->to_string() + " " + t2->to_string() + ") " + l1->to_string() + " " + l2->to_string()); 
    this->t_1 = t1;
    this->cmp = cmp;
    this->t_2 = t2;
    this->label1 = l1;
    this->label2 = l2;
}

Item*
Instruction_if::get_t1(){
    return t_1;
}

Item*
Instruction_if::get_cmp(){
    return cmp;
}
Item*
Instruction_if::get_t2(){
    return t_2;
}
Item*
Instruction_if::get_label1(){
    return label1;
}

Item*
Instruction_if::get_label2(){
    return label2;
}

Instruction_while::Instruction_while(Item* t1, Item* cmp, Item* t2, Item* l1, Item* l2){
    set_name("Instruction_while");
    set_string("while (" + t1->to_string() + " " + cmp->to_string() + " " + t2->to_string() + ") " + l1->to_string() + " " + l2->to_string()); 
    this->t_1 = t1;
    this->cmp = cmp;
    this->t_2 = t2;
    this->label1 = l1;
    this->label2 = l2;
}

Item*
Instruction_while::get_t1(){
    return t_1;
}

Item*
Instruction_while::get_cmp(){
    return cmp;
}
Item*
Instruction_while::get_t2(){
    return t_2;
}
Item*
Instruction_while::get_label1(){
    return label1;
}

Item*
Instruction_while::get_label2(){
    return label2;
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
Function::to_string(){
    std::string ans = return_type + " " + name + "(";
    for(int64_t i = 0; i < types.size(); i++){
        ans += (types[i]->to_string() + " " + vars[i]->to_string() + ", ");
    }
    if(types.size() > 0) ans = ans.substr(0,ans.size()-2);
    ans += ")\n{\n";
    for(auto in : instructions){
        ans += in->to_string() + "\n";
    }
    ans += "}\n";
    return ans;
}

}