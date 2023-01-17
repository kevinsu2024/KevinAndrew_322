#include "L1.h"

namespace L1 {

Register::Register (RegisterID r)
  : ID {r}{
  return ;
}


InstructionLabel::InstructionLabel (std::string l)
  : label {l}{
  return ;
}

InstructionNumber::InstructionNumber (int64_t n)
  : val {n}{
  return ;
}

CompareOp::CompareOp (std::string s)
  : op {s}{
  return ;
}

ArithmeticOp::ArithmeticOp (std::string s)
  : op {s}{
  return ;
}

ShiftOp::ShiftOp (std::string s)
  : op {s}{
  return ;
}

CompareExpression::CompareExpression (Item *first, Item *second, Item *op)
  : f { first },
    s { second },
    o { op } {
  return ;
}

Instruction_assignment::Instruction_assignment (Item *dst, Item *src)
  : s { src },
    d { dst } {
  return ;
}

Instruction_cmp_assignment::Instruction_cmp_assignment (Item *dst, Item *first, Item *second, Item *op)
  : d { dst },
    f { first },
    s { second },
    o { op } {
  return ;
}

Instruction_mem_load::Instruction_mem_load (Item *dst, Item *src, Item *num)
  : s { src },
    d { dst },
    n { num } {
  return ;
}

Instruction_mem_op_load::Instruction_mem_op_load (Item *dst, Item *src, Item *num, Item *op)
  : s { src },
    d { dst },
    n { num },
    o { op } {
  return ;
}

Instruction_mem_store::Instruction_mem_store (Item *src, Item *num, Item *x_register)
  : s { src },
    n { num },
    x { x_register } {
  return ;
}

Instruction_mem_op_store::Instruction_mem_op_store (Item *t_rule, Item *num, Item *x_register, Item *op)
  : t { t_rule },
    n { num },
    x { x_register },
    o { op } {
  return ;
}

Instruction_aop::Instruction_aop (Item *t_rule, Item *op, Item *reg)
  : t { t_rule },
    o { op },
    r { reg } {
  return ;
}


Instruction_sop::Instruction_sop (Item *shift, Item *op, Item *reg)
  : s { shift },
    o { op },
    r { reg } {
  return ;
}

Instruction_cjump::Instruction_cjump (Item *t1, Item *op, Item *t2, Item *label)
  : f { t1 },
    o { op },
    s { t2 },
    l {label} {
  return ;
}

Instruction_label::Instruction_label (Item *l)
  : label { l } {
  return ;
}
Instruction_goto::Instruction_goto (Item *l)
  : label { l } {
    return ;
  }

Instruction_pp::Instruction_pp (Item *r)
  : reg { r } {
  return ;
}



Instruction_mm::Instruction_mm (Item *r)
  : reg { r } {
  return ;
}
Instruction_at::Instruction_at (Item *r1, Item *r2, Item *r3, Item *n)
  : reg1 { r1 },
    reg2 { r2 },
    reg3 { r3 },
    num { n } {
  return ;
}

Instruction_call_u::Instruction_call_u (Item *up, Item *n)
  : u { up },
    num { n }{
  return ;
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

}



