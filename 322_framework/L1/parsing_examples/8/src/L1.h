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

  class Instruction_mem_op_load : public Instruction{
    public:
      Instruction_mem_op_load (Item *dst, Item *src, Item *num, Item *op);

    private:
      Item *s;
      Item *d;
      Item *n;
      Item *o;
  };

  class Instruction_mem_store : public Instruction{
    public:
      Instruction_mem_store (Item *s, Item *num, Item *x_register);

    private:
      Item *s;
      Item *n;
      Item *x;
  };

  class Instruction_mem_op_store : public Instruction{
    public:
      Instruction_mem_op_store (Item *t_rule, Item *num, Item *x_register, Item *op);

    private:
      Item *t;
      Item *n;
      Item *x;
      Item *o;
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

  class Instruction_cjump : public Instruction{
    public:
      Instruction_cjump (Item *first, Item *op, Item *second, Item *label);

    private:
      Item *f;
      Item *o;
      Item *s;
      Item *l;
  };

  class Instruction_label : public Instruction{
    public:
      Instruction_label (Item *l);

    private:
      Item *label;
  };


  class Instruction_goto : public Instruction{
    public:
      Instruction_goto (Item *l);

    private:
      Item *label;
  };

  class Instruction_pp : public Instruction{
    public:
      Instruction_pp (Item *r);

    private:
      Item *reg;
  };

  class Instruction_mm : public Instruction{
    public:
      Instruction_mm (Item *r);

    private:
      Item *reg;
  };

  class Instruction_at : public Instruction{
    public:
      Instruction_at (Item *r1, Item *r2, Item *r3, Item *num);

    private:
      Item *reg1;
      Item *reg2;
      Item *reg3;
      Item *num;
  };

  class Instruction_call_u : public Instruction{
    public:
      Instruction_call_u (Item *up, Item *num);

    private:
      Item *u;
      Item *num;
  };

  class Instruction_call_print : public Instruction{
    public:
      Instruction_call_print ();
  };

  class Instruction_call_input : public Instruction{
    public:
      Instruction_call_input ();
  };

  class Instruction_call_allocate : public Instruction{
    public:
      Instruction_call_allocate ();
  };

  class Instruction_call_tensor_error : public Instruction{
    public:
      Instruction_call_tensor_error (Item *n);

    private:
      Item *num;
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
