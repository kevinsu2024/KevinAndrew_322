#pragma once

#include <vector>
#include <string>
#include <typeinfo>

namespace L3{
    /*
    * Item interface
    */
    class Item {
        public:
            std::string get_name();
            std::string to_string();
            void set_name(std::string item_name);
            void set_string(std::string item_str);
        private:
            std::string item_name;
            std::string item_str;
    };

    /*
    * L3 items
    */
    class Variable : public Item {
        public:
            Variable (std::string variable);
    };
    class InstructionLabel : public Item{
        public:
            InstructionLabel (std::string label);
    };

    class InstructionNumber : public Item {
        public:
            InstructionNumber (std::string number);
    };

    class CompareOp : public Item {
        public:
            CompareOp (std::string cmp_op);
    };

    class Op : public Item {
        public:
            Op (std::string op);
    };

    class FunctionName : public Item {
        public:
            FunctionName (std::string function_name);
    };

    class StandardLibrary : public Item{
        public:
            StandardLibrary(std::string standard_library_function_name);
    };


    /*
    * Instruction interface
    */
    class Instruction{
        public:
            std::string get_name();
            void set_name(std::string instruction_name);
            std::string to_string();
            void set_string(std::string instr);
        private:
            std::string name;
            std::string instr;
    };
    /*
    * Instructions
    */
    class Instruction_assignment : public Instruction{
        public:
            Instruction_assignment (Item *var, Item *s);
            Item *get_s();
            Item *get_var();
        private:
            Item *var;
            Item *s;
    };
    class Instruction_cmp : public Instruction{
        public:
            Instruction_cmp (Item *var, Item *t1, Item *op, Item *t2);
            
            Item* get_var();
            Item* get_t1();
            Item* get_op();
            Item* get_t2();
            

        private:
            Item *var;
            Item *t1;
            Item *op;
            Item *t2;
    };
    class Instruction_op : public Instruction{
        public:
            Instruction_op (Item *var, Item *t1, Item *op, Item *t2);
            
            Item* get_var();
            Item* get_t1();
            Item* get_op();
            Item* get_t2();
        private:
            Item *var;
            Item *t1;
            Item *op;
            Item *t2;
    };
    class Instruction_load : public Instruction{
        public:
            Instruction_load (Item *var_dst, Item *var_src);
            
            Item* get_var_dst();
            Item* get_var_src();
        private:
            Item *var_dst;
            Item *var_src;
    };
    class Instruction_store : public Instruction{
        public:
            Instruction_store (Item *var, Item *s);
            
            Item* get_var();
            Item* get_s();
        private:
            Item *var;
            Item *s;
    };
    class Instruction_return : public Instruction{
        public:
            Instruction_return();
    };
    class Instruction_return_t : public Instruction{
        public:
            Instruction_return_t(Item *t);
            Item* get_t();
        private:
            Item* t;
    };
    class Instruction_label : public Instruction{
        public:
            Instruction_label (Item *l);
            Item* get_label();

        private:
            Item *label;
    };
    class Instruction_break : public Instruction{
        public:
            Instruction_break (Item *l);
            Item* get_label();

        private:
            Item *label;
    };
    class Instruction_break_t : public Instruction{
        public:
            Instruction_break_t (Item *t, Item *l);
            Item* get_t();
            Item* get_label();

        private:
            Item *t;
            Item *label;
    };
    class Instruction_call : public Instruction{
        public:
            Instruction_call (Item *callee, std::vector<Item*> *args);
            Item* get_callee();
            std::vector<Item*>* get_args();

        private:
            Item *callee;
            std::vector<Item*> *args;
    };
    class Instruction_call_assignment : public Instruction{
        public:
            Instruction_call_assignment (Item* var, Item *callee, std::vector<Item*> *args);
            Item* get_var();
            Item* get_callee();
            std::vector<Item*>* get_args();

        private:
            Item *var;
            Item *callee;
            std::vector<Item*> *args;
    };
    /*
    *
    */
    class Function{
        public:
            std::string name;
            std::vector<Item*> vars;
            std::vector<Instruction *> instructions;
            std::string to_string();
    };
    class Program{
        public:
            std::vector<Function *> functions;
    };
    


   
}