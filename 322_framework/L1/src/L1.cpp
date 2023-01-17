#include "L1.h"

namespace L1 {

Register::Register (RegisterID r)
  : ID {r}{
  return ;
}

std::string
Register::get_register_ID(){
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
}


InstructionLabel::InstructionLabel (std::string l)
  : label {l}{
  return ;
}

std::string 
InstructionLabel::get_label_name(){
  return label;
}

InstructionNumber::InstructionNumber (int64_t n)
  : val {n}{
  return ;
}

int64_t
InstructionNumber::get_val(){
  return val;
}

CompareOp::CompareOp (std::string s)
  : op {s}{
  return ;
}
std::string
CompareOp::get_op_char(){
  return op;
}

ArithmeticOp::ArithmeticOp (std::string s)
  : op {s}{
  return ;
}

std::string
ArithmeticOp::get_op_char() {
  return op;
}

ShiftOp::ShiftOp (std::string s)
  : op {s}{
  return ;
}

std::string
ShiftOp::get_op_char() {
  return op;
}

CompareExpression::CompareExpression (Item *first, Item *second, Item *op)
  : f { first },
    s { second },
    o { op } {
  return ;
}

FunctionName::FunctionName (std::string name)
  : n {name} {
    return;
}

std::string
Instruction_ret::get_name(){
  return "Instruction_ret";
}

Instruction_assignment::Instruction_assignment (Item *dst, Item *src)
  : s { src },
    d { dst } {
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

std::string
Instruction_assignment::get_name(){
  return "Instruction_assignment";
}

Instruction_cmp_assignment::Instruction_cmp_assignment (Item *dst, Item *first, Item *second, Item *op)
  : d { dst },
    f { first },
    s { second },
    o { op } {
  return ;
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

Instruction_mem_load::Instruction_mem_load (Item *dst, Item *src, Item *num)
  : s { src },
    d { dst },
    n { num } {
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
  return ;
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
  return ;
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
  return ;
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
  return ;
}

Item*
Instruction_pp::get_reg(){
  return reg;
}



Instruction_mm::Instruction_mm (Item *r)
  : reg { r } {
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
  return ;
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

}



