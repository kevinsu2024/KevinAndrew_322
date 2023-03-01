#pragma once

#include <vector>
#include <string>
#include <typeinfo>


namespace LA{
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
    * IR items
    */
    class Name : public Item {
        public:
            Name (std::string name);
    };
    class InstructionLabel : public Item{
        public:
            InstructionLabel (std::string label);
    };

    class InstructionNumber : public Item {
        public:
            InstructionNumber (std::string number);
    };

    class Op : public Item {
        public:
            Op (std::string op);
    };

    class FunctionType : public Item{
        public:
            FunctionType (std::string s);
    };

    class Type : public Item{
        public:
            Type (std::string s);
    };

    class StandardLibrary : public Item{
        public:
            StandardLibrary(std::string standard_library_function_name);
    };

    class Instruction_bracket : public Item{
        public:
            Instruction_bracket(Item* i);
            Item* get_index();
        private:
            Item* index;
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
    class Instruction_declaration : public Instruction{
        public:
            Instruction_declaration(Item* t, Item* v);
            Item* get_var_type();
            Item* get_var();
        private:
            Item* var_type;
            Item* var;
    };
    
    class Instruction_assignment : public Instruction{
        public:
            Instruction_assignment (Item *var, Item *s);
            Item *get_s();
            Item *get_var();
        private:
            Item *var;
            Item *s;
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
            Instruction_load (Item* d, Item* s, std::vector<Item*> i, int64_t l);
            
            Item* get_var_dst();
            Item* get_var_src();
            std::vector<Item*> get_indices();
            int64_t get_line_no();
        private:
            Item *var_dst;
            Item *var_src;
            std::vector<Item*> indices;
            int64_t line_no;
    };

    class Instruction_store : public Instruction{
        public:
            Instruction_store (Item *var, std::vector<Item*> i, Item *s, int64_t l);
            Item* get_var();
            Item* get_s();
            std::vector<Item*> get_indices();
            int64_t get_line_no();
        private:
            Item *var;
            Item *s;
            std::vector<Item*> indices;
            int64_t line_no;
    };

    class Instruction_array_length : public Instruction{
        public:
            Instruction_array_length(Item* d, Item* s, Item* i);
            Item* get_dst_var();
            Item* get_src_var();
            Item* get_index();
        private:
            Item* dst_var;
            Item* src_var;
            Item* index;
    };

    class Instruction_tuple_length : public Instruction{
        public:
            Instruction_tuple_length(Item* d, Item* s);
            Item* get_dst_var();
            Item* get_src_var();
        private:
            Item* dst_var;
            Item* src_var;
    };

    class Instruction_label : public Instruction{
        public:
            Instruction_label (Item *l);
            Item* get_label();

        private:
            Item *label;
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
    

    class Instruction_branch : public Instruction{
        public:
            Instruction_branch (Item *l);
            Item* get_label();

        private:
            Item *label;
    };

    class Instruction_branch_t : public Instruction{
        public:
            Instruction_branch_t (Item *t, Item *l, Item* l2);
            Item* get_t();
            Item* get_label1();
            Item* get_label2();

        private:
            Item *t;
            Item *label1;
            Item* label2;
    };

    class Instruction_call : public Instruction{
        public:
            Instruction_call (Item *callee, std::vector<Item*> args);
            Item* get_callee();
            std::vector<Item*> get_args();

        private:
            Item *callee;
            std::vector<Item*> args;
    };
    class Instruction_call_assignment : public Instruction{
        public:
            Instruction_call_assignment (Item* var, Item *callee, std::vector<Item*> args);
            Item* get_var();
            Item* get_callee();
            std::vector<Item*> get_args();

        private:
            Item *var;
            Item *callee;
            std::vector<Item*> args;
    };

    class Instruction_create_array : public Instruction{
        public:
            Instruction_create_array(Item* d, std::vector<Item*> ar);
            Item* get_dst_var();
            std::vector<Item*> get_args();
        private:
            Item* dst_var;
            std::vector<Item*> args;
    };

    class Instruction_create_tuple : public Instruction{
        public:
            Instruction_create_tuple(Item* d, Item* s);
            Item* get_dst_var();
            Item* get_size();
        private:
            Item* dst_var;
            Item* size;
    };


    /*
    *
    */

    class Function{
        public:
            std::string name;
            std::string return_type;
            std::vector<Item*> types;
            std::vector<Item*> vars;
            std::vector<Instruction *> instructions;
            std::string longest_name;
            std::string longest_label;
            std::string to_string();
    };
    class Program{
        public:
            std::vector<Function *> functions;
    };
    


   
}