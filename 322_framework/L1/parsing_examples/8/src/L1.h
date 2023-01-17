#pragma once

#include <vector>
#include <string>

namespace L1 {

  enum RegisterID {rdi, rax, rsi, rdx, rcx, r8, r9, rbx, rbp, r10, r11, r12, r13, r14, r15, rsp};


  class Item {
  };

  class Register : public Item {
    public:
      Register (RegisterID r);

    private:
      RegisterID ID;
  };

  class InstructionLabel : public Item{
    public:
      InstructionLabel (std::string l);
    private:
      std::string label;
  };

  class InstructionNumber : public Item {
    public:
      InstructionNumber (int64_t n);
    private:
      int64_t val;
  };

  class CompareOp : public Item {
    public:
      CompareOp (std::string s);
    private:
      std::string op;
  };

  class ArithmeticOp : public Item {
    public:
      ArithmeticOp (std::string s);
    private:
      std::string op;
  };

  class ShiftOp : public Item {
    public:
      ShiftOp (std::string s);
    private:
      std::string op;
  };

  class CompareExpression : public Item {
    public:
      CompareExpression (Item *first, Item *second, Item *Op);
    private:
      Item *f;
      Item *s;
      Item *o;
  };
  /*
   * Instruction interface.
   */
  class Instruction{
  };

  /*
   * Instructions.
   */
  class Instruction_ret : public Instruction{
  };

  class Instruction_assignment : public Instruction{
    public:
      Instruction_assignment (Item *dst, Item *src);

    private:
      Item *s;
      Item *d;
  };

  class Instruction_cmp_assignment : public Instruction{
    public:
      Instruction_cmp_assignment (Item *dst, Item *first, Item *second, Item *op);

    private:
      Item *d;
      Item *f;
      Item *s;
      Item *o;
  };

  class Instruction_mem_load : public Instruction{
    public:
      Instruction_mem_load (Item *dst, Item *src, Item *num);

    private:
      Item *s;
      Item *d;
      Item *n;
  };

  class Instruction_mem_store : public Instruction{
    public:
      Instruction_mem_store (Item *s, Item *num, Item *x_register);

    private:
      Item *s;
      Item *n;
      Item *x;
  };

  class Instruction_aop : public Instruction{
    public:
      Instruction_aop (Item *t_rule, Item *op, Item *reg);

    private:
      Item *t;
      Item *o;
      Item *r;
  };

  class Instruction_sop : public Instruction{
    public:
      Instruction_sop (Item *shift, Item *op, Item *reg);

    private:
      Item *s;
      Item *o;
      Item *r;
  };

  /*
   * Function.
   */
  class Function{
    public:
      std::string name;
      int64_t arguments;
      int64_t locals;
      std::vector<Instruction *> instructions;
  };

  class Program{
    public:
      std::string entryPointLabel;
      std::vector<Function *> functions;
  };

}
