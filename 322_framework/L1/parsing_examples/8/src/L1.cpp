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

Instruction_mem_store::Instruction_mem_store (Item *src, Item *num, Item *x_register)
  : s { src },
    n { num },
    x { x_register } {
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

}
