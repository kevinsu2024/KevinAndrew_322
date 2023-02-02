#include "L1.h"

namespace L1 {

std::string map_reg(std::string s){
    if (s == "%rax"){
      return R"(%al)";
    } else if (s == R"(%rbp)"){
      return R"(%bpl)";
    } else if (s == R"(%rbx)"){
      return R"(%bl)";
    } else if (s == R"(%rcx)"){
      return R"(%cl)";
    } else if (s == R"(%rdi)"){
      return R"(%dil)";
    } else if ( s == R"(%rdx)"){
      return R"(%dl)";
    } else if ( s == R"(%rsi)"){
      return R"(%sil)";
    } else{
      return s + "b";
    }
  }

std::string
Item::get_name() {
  return name;
}

void
Item::set_name(std::string item_name){
  name = item_name;
}

void
Item::set_string(std::string i){
  in = i;
}

std::string
Item::to_string(){
  return in;
}
Register::Register (RegisterID r)
  : ID {r}{
  set_name("Register");
  set_string("%" +print_reg_id(r));
  return;
}

std::string
print_reg_id(RegisterID ID){
  if(ID == rdi){
    return "rdi";
  } else if (ID == rax){
    return "rax";
  } else if (ID == rsi){
    return "rsi";
  } else if (ID == rdx){
    return "rdx";
  } else if (ID == rcx){
    return "rcx";
  } else if (ID == r8){
    return "r8";
  } else if (ID == r9){
    return "r9";
  } else if (ID == rbx){
    return "rbx";
  } else if (ID == rbp){
    return "rbp";
  } else if (ID == r10){
    return "r10";
  } else if (ID == r11){
    return "r11";
  } else if (ID == r12){
    return "r12";
  } else if (ID == r13){
    return "r13";
  } else if (ID == r14){
    return "r14";
  } else if (ID == r15){
    return "r15";
  } else if (ID == rsp){
    return "rsp";
  } 
  return "";
}


InstructionLabel::InstructionLabel (std::string l){
  set_name("InstructionLabel");
  set_string(l);
  return ;
}

InstructionNumber::InstructionNumber (std::string n){
  set_name("InstructionNumber");
  set_string("$" + n);
  return ;
}



CompareOp::CompareOp (std::string s){
  set_name("CompareOp");
  set_string(s);
  return;
}

ArithmeticOp::ArithmeticOp (std::string s){
  set_name("ArithmeticOp");
  set_string(s);
  return;
}

ShiftOp::ShiftOp (std::string s){
  set_name("ShiftOp");
  set_string(s);
  return;
}



std::string
Instruction::get_name () {
  return name;
}

void
Instruction::set_name (std::string instruction_name) {
  name = instruction_name;
}

void
Instruction::set_string(std::string st){
  s = st;
}

std::string
Instruction::to_string(){
  return s;
}


Instruction_ret::Instruction_ret () {
  set_name("Instruction_ret");
  set_string("retq");
  return;
}

FunctionName::FunctionName (std::string name)
  : n {name} {
    set_name("FunctionName");
    set_string(name);
    return;
}

std::string
FunctionName::get_function_name(){
  return n;
}




Instruction_assignment::Instruction_assignment (Item *dst, Item *src)
  : s { src },
    d { dst } {
  set_name("Instruction_assignment");
  set_string("movq " + src->to_string() + ", " + dst->to_string());
  return ;
}

Item*
Instruction_assignment::get_src(){
  return s;
}

Item*
Instruction_assignment::get_dst(){
  return d;
}

Instruction_cmp_assignment::Instruction_cmp_assignment (Item *dst, Item *first, Item *second, Item *op)
  : d { dst },
    f { first },
    s { second },
    o { op } {
  set_name("Instruction_cmp_assignment");
  bool first_reg = first->get_name() == "Register";
  bool second_reg = second->get_name() == "Register";
  std::string instr = "";
  if ((!first_reg) && (!second_reg)){
    int64_t f_num = stoi(first->to_string().substr(1,first->to_string().size()-1));
    int64_t s_num = stoi(second->to_string().substr(1,second->to_string().size()-1));
    bool val;
    if (op->to_string() == "<"){
      val = f_num < s_num;
    } else if (op->to_string() == "=") {
      val = f_num == s_num;
    } else {
      val = f_num <= s_num;
    }
    instr += "movq $" + std::to_string(val) + ", " + dst->to_string() + "\n";
  } else if (!first_reg){
    instr+= "cmpq " + first->to_string() + ", " + second->to_string() + "\n";
    if (op->to_string() == "<"){
      instr += "setg " + map_reg(dst->to_string()) + "\n";
    } else if (op->to_string() == "=") {
      instr += "\tsete " + map_reg(dst->to_string()) + "\n";
    } else {
      instr += "\tsetge " + map_reg(dst->to_string()) + "\n";
    }
    instr += "\tmovzbq " + map_reg(dst->to_string()) + ", " + dst->to_string() + "\n";
  } else {
    instr += "\tcmpq " + second->to_string() + ", " + first->to_string() + "\n";
    if (op->to_string() == "<"){
      instr += "\tsetl " + map_reg(dst->to_string()) + "\n";   
    } else if (op->to_string() == "=") {
      instr += "\tsete " + map_reg(dst->to_string()) + "\n";
    } else {
      instr += "\tsetle " + map_reg(dst->to_string()) + "\n";
    }
    instr += "\tmovzbq " + map_reg(dst->to_string()) + ", " + dst->to_string() + "\n";
  }
  set_string(instr);
  return;
}

Item*
Instruction_cmp_assignment::get_dst(){
  return d;
}
Item*
Instruction_cmp_assignment::get_first(){
  return f;
}
Item*
Instruction_cmp_assignment::get_second(){
  return s;
}
Item*
Instruction_cmp_assignment::get_op(){
  return o;
}

Instruction_function_assignment::Instruction_function_assignment (Item *dst, Item *fn)
  : d { dst },
    fname { fn }{
  set_name("Instruction_function_assignment");
  set_string("movq " + fn->to_string() + ", " + dst->to_string());
  return ;
}

Item*
Instruction_function_assignment::get_dst(){
  return d;
}

Item*
Instruction_function_assignment::get_fname(){
  return fname;
}

Instruction_mem_load::Instruction_mem_load (Item *dst, Item *src, Item *num)
  : s { src },
    d { dst },
    n { num } {
  set_name("Instruction_mem_load");
  std::string numb = num->to_string();
  numb = numb.substr(1,numb.size()-1);
  set_string("\tmovq " + numb + "(" + src->to_string() + "), " + dst->to_string() + "\n");
  return ;
}

Item*
Instruction_mem_load::get_dst(){
  return d;
}
Item*
Instruction_mem_load::get_src(){
  return s;
}
Item*
Instruction_mem_load::get_num(){
  return n;
}



Instruction_mem_op_load::Instruction_mem_op_load (Item *dst, Item *src, Item *num, Item *op)
  : s { src },
    d { dst },
    n { num },
    o { op } {
  set_name("Instruction_mem_op_load");
  set_string(dst->to_string() + " " + op->to_string() + " mem " + src->to_string() + " " +  num->to_string());
  return ;
}

Item*
Instruction_mem_op_load::get_dst(){
  return d;
}
Item*
Instruction_mem_op_load::get_src(){
  return s;
}
Item*
Instruction_mem_op_load::get_num(){
  return n;
}
Item*
Instruction_mem_op_load::get_op(){
  return o;
}


Instruction_mem_store::Instruction_mem_store (Item *src, Item *num, Item *x_register)
  : s { src },
    n { num },
    x { x_register } {
  set_name("Instruction_mem_store");
  set_string("mem " + x_register->to_string() + " " + num->to_string() + " <- " + src->to_string() );
  return ;
}

Item*
Instruction_mem_store::get_x_reg(){
  return x;
}
Item*
Instruction_mem_store::get_src(){
  return s;
}
Item*
Instruction_mem_store::get_num(){
  return n;
}

Instruction_mem_op_store::Instruction_mem_op_store (Item *t_rule, Item *num, Item *x_register, Item *op)
  : t { t_rule },
    n { num },
    x { x_register },
    o { op } {
  set_name("Instruction_mem_op_store");
  set_string("mem " + x_register->to_string() + " " + num->to_string() + " " + op->to_string() + " " + t_rule->to_string() );
  return;
}
Item*
Instruction_mem_op_store::get_x_reg(){
  return x;
}
Item*
Instruction_mem_op_store::get_src(){
  return t;
}
Item*
Instruction_mem_op_store::get_num(){
  return n;
}
Item*
Instruction_mem_op_store::get_op(){
  return o;
}

Instruction_aop::Instruction_aop (Item *t_rule, Item *op, Item *reg)
  : t { t_rule },
    o { op },
    r { reg } {
  set_name("Instruction_aop");
  set_string(reg->to_string() + " " + op->to_string() + " " + t_rule->to_string());
  return;
}

Item*
Instruction_aop::get_op(){
  return o;
}

Item*
Instruction_aop::get_dst(){
  return r;
}

Item*
Instruction_aop::get_src(){
  return t;
}

Instruction_sop::Instruction_sop (Item *shift, Item *op, Item *reg)
  : s { shift },
    o { op },
    r { reg } {
  set_name("Instruction_sop");
  set_string(reg->to_string() + " " + op->to_string() + " " + shift->to_string());
  return;
}

Item*
Instruction_sop::get_src(){
  return s;
}

Item*
Instruction_sop::get_dst(){
  return r;
}

Item*
Instruction_sop::get_op(){
  return o;
}

Instruction_cjump::Instruction_cjump (Item *t1, Item *op, Item *t2, Item *label)
  : f { t1 },
    o { op },
    s { t2 },
    l {label} {
  set_name("Instruction_cjump");
  set_string("cjump " + t1->to_string() + " " + op->to_string() + " " + t2->to_string() + " " + label->to_string());
  return ;
}

Item*
Instruction_cjump::get_first(){
  return f;
}
Item*
Instruction_cjump::get_second(){
  return s;
}
Item*
Instruction_cjump::get_label(){
  return l;
}
Item*
Instruction_cjump::get_op(){
  return o;
}

Instruction_label::Instruction_label (Item *l)
  : label { l } {
  return ;
}

Item*
Instruction_label::get_label(){
  return label;
}

Instruction_goto::Instruction_goto (Item *l)
  : label { l } {
    return ;
  }



Item* Instruction_goto::get_label(){
  return label;
}


Instruction_pp::Instruction_pp (Item *r)
  : reg { r } {
    set_name("Instruction_pp");
    set_string(r->to_string() + "++");
  return;
}

Item*
Instruction_pp::get_reg(){
  return reg;
}



Instruction_mm::Instruction_mm (Item *r)
  : reg { r } {
    set_name("Instruction_mm");
    set_string(r->to_string() + "--");
  return ;
}

Item*
Instruction_mm::get_reg(){
  return reg;
}

Instruction_at::Instruction_at (Item *r1, Item *r2, Item *r3, Item *n)
  : reg1 { r1 },
    reg2 { r2 },
    reg3 { r3 },
    num { n } {
      set_name("Instruction_at");
      set_string(r1->to_string() + " @ " + r2->to_string() + " " + r3->to_string() + " " + n->to_string());
  return ;
}

Item*
Instruction_at::get_reg1(){
  return reg1;
}
Item*
Instruction_at::get_reg2(){
  return reg2;
}
Item*
Instruction_at::get_reg3(){
  return reg3;
}
Item*
Instruction_at::get_num(){
  return num;
}
Instruction_call_u::Instruction_call_u (Item *up, Item *n)
  : u { up },
    num { n }{
  set_name("Instruction_call_u");
  set_string("call " + up->to_string() + " " + n->to_string());
  return;
}

Item*
Instruction_call_u::get_u(){
  return u;
}

Item*
Instruction_call_u::get_num(){
  return num;
}

Instruction_call_print::Instruction_call_print (){
  return ;
}

Instruction_call_function::Instruction_call_function (std::string st, Item* n)
  : s { st },
    num { n }{
  return;
}

Item*
Instruction_call_function::get_number(){
  return num;
}

std::string
Instruction_call_function::get_name(){
  return s;
}

Instruction_call_input::Instruction_call_input (){
  return ;
}

Instruction_call_allocate::Instruction_call_allocate (){
  return ;
}

Instruction_call_tensor_error::Instruction_call_tensor_error (Item *n)
  : num { n }{
  return ;
}

Item*
Instruction_call_tensor_error::get_val(){
  return num;
}

std::string
Function::to_string(){
  auto instrs = instructions;
  std::string ans = "(" + name + "\n";
  ans += "\t";
  ans += (std::to_string(arguments) + " " + std::to_string(locals) + "\n");
  for(Instruction* instr : instrs){
    ans += "\t" + instr->to_string() + "\n";
  }
  ans += ")\n";
  return ans;
}

}



