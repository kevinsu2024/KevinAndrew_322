#include <string>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <memory>
#include <code_generator.h>

using namespace std;

namespace L1{
  std::string convert_func_name(std::string s){
    return "_" + s.substr(1, s.size()-1);
  }

  std::string convert_label_name(std::string s){
    return "_" + s.substr(1,s.size()-1);
  }

  std::string convert_item_to_str(Item* i){
    if (typeid(*i) == typeid(Register)){
      Register* it = (Register*) i;
      return "%" + it->get_register_ID();
    } else if (typeid(*i) == typeid(InstructionLabel)){
      InstructionLabel* it = (InstructionLabel*) i;
      return "$" + convert_label_name(it->get_label_name());
    } else if (typeid(*i) == typeid(InstructionNumber)){
      auto it = (InstructionNumber*) i;
      return "$" + std::to_string(it->get_val());
    } else if (typeid(*i) == typeid(ArithmeticOp)){
      auto it = (ArithmeticOp*) i;
      std::string op_char = it->get_op_char();
      return op_char;
    } else if (typeid(*i) == typeid(ShiftOp)){
      auto it = (ShiftOp*) i;
      std::string op_char = it->get_op_char();
    }
  }

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


  void generate_code(Program p){

    /* 
     * Open the output file.
     */ 
    std::ofstream outputFile;
    outputFile.open("prog.S");

    std::string ePL = p.entryPointLabel;
    std::string name = ePL.substr(1,ePL.size()-1);

    std::vector<Function *> functions = p.functions;
    
    /* 
     * Generate target code
     */ 
    outputFile << ".text\n"
      "\t.globl go\n"
    "go:\n"
      "\tpushq %rbx\n"
      "\tpushq %rbp\n"
      "\tpushq %r12\n"
      "\tpushq %r13\n"
      "\tpushq %r14\n"
      "\tpushq %r15\n";

    outputFile << "\tcall _" << name << "\n";
    outputFile << "\tpopq %r15\n"
      "\tpopq %r14\n"
      "\tpopq %r13\n"
      "\tpopq %r12\n"
      "\tpopq %rbx\n"
      "\tpopq %rbp\n"
      "\tpopq %r15\n"
      "\tretq\n";

    for(Function *f : functions){
      outputFile << convert_func_name(f->name) << ":\n";
      //TODO STACK STUFF
      int64_t sub_stack_num = f->locals * 8;
      outputFile << "\tsubq $" << std::to_string(sub_stack_num) << ", %rsp\n";
      for(auto *i : f->instructions){
        Instruction_assignment* is_assignment = (Instruction_assignment*)i;
        Instruction_cjump* is_cjump = (Instruction_cjump*)i;
        if (typeid(*i) == typeid(Instruction_ret)){
          int64_t argument_multiplier = 0;
          if (f->arguments > 6){
            argument_multiplier += (f->arguments - 6);
          }
          sub_stack_num += (argument_multiplier * 8);
          outputFile << "\taddq $" << std::to_string(sub_stack_num) << ", %rsp\n";
          outputFile << "\tretq\n";
        } else if(typeid(*is_cjump) == typeid(Instruction_assignment)){ //CHANGE BACK
          Instruction_assignment* in = (Instruction_assignment *) i;
          std::string src = convert_item_to_str(in->get_src());
          std::string dst = convert_item_to_str(in->get_dst());
          outputFile << "\tmovq " << src << ", " << dst << "\n";

        } else if(typeid(*i) == typeid(Instruction_cmp_assignment)){
          Instruction_cmp_assignment* in = (Instruction_cmp_assignment *) i;
          std::string dst = convert_item_to_str(in->get_dst());
          Item* first = in->get_first();
          Item* second = in->get_second();
          bool first_reg = typeid(*first) == typeid(Register);
          bool second_reg = typeid(*second) == typeid(Register);
          std::string f_val = convert_item_to_str(first);
          std::string s_val = convert_item_to_str(second);
          auto op = (CompareOp* ) in->get_op();
          std::string cop = op->get_op_char();

          if ((!first_reg) && (!second_reg)){
            if (cop == "<"){
              int64_t f_num = stoi(f_val.substr(1,f_val.size()-1));
              int64_t s_num = stoi(s_val.substr(1,s_val.size()-1));
              bool val = f_num < s_num;
              outputFile << "\tmovq $" << val << ", " << dst << "\n";
            } else if (cop == "=") {
              int64_t f_num = stoi(f_val.substr(1,f_val.size()-1));
              int64_t s_num = stoi(s_val.substr(1,s_val.size()-1));
              bool val = f_num == s_num;
              outputFile << "\tmovq $" << val << ", " << dst << "\n";
            } else {
              int64_t f_num = stoi(f_val.substr(1,f_val.size()-1));
              int64_t s_num = stoi(s_val.substr(1,s_val.size()-1));
              bool val = f_num <= s_num;
              outputFile << "\tmovq $" << val << ", " << dst << "\n";
            }
          } else if (!first_reg){
            if (cop == "<"){
              outputFile << "\tcmpq " << f_val << ", " << s_val << "\n";
              outputFile << "\tsetg " << map_reg(dst) << "\n";
              outputFile << "\tmovzbq " << map_reg(dst) << ", " << dst << "\n";
            } else if (cop == "=") {
              outputFile << "\tcmpq " << f_val << ", " << s_val << "\n";
              outputFile << "\tsete " << map_reg(dst) << "\n";
              outputFile << "\tmovzbq " << map_reg(dst) << ", " << dst << "\n";
            } else {
              outputFile << "\tcmpq " << f_val << ", " << s_val << "\n";
              outputFile << "\tsetge " << map_reg(dst) << "\n";
              outputFile << "\tmovzbq " << map_reg(dst) << ", " << dst << "\n";
            }
          } else {
            if (cop == "<"){
              outputFile << "\tcmpq " << f_val << ", " << s_val << "\n";
              outputFile << "\tsetg " << map_reg(dst) << "\n";
              outputFile << "\tmovzbq " << map_reg(dst) << ", " << dst << "\n";
            } else if (cop == "=") {
              outputFile << "\tcmpq " << f_val << ", " << s_val << "\n";
              outputFile << "\tsete " << map_reg(dst) << "\n";
              outputFile << "\tmovzbq " << map_reg(dst) << ", " << dst << "\n";
            } else {
              outputFile << "\tcmpq " << f_val << ", " << s_val << "\n";
              outputFile << "\tsetge " << map_reg(dst) << "\n";
              outputFile << "\tmovzbq " << map_reg(dst) << ", " << dst << "\n";
            }
          }



        } else if(typeid(*i) == typeid(Instruction_mem_load)){
          Instruction_mem_load* in = (Instruction_mem_load*) i;
          std::string src = convert_item_to_str(in->get_src());
          std::string dst = convert_item_to_str(in->get_dst());
          std::string num = convert_item_to_str(in->get_num());
          outputFile << "\tmovq " << num << "(" << src << "), " << dst << "\n";

        } else if(typeid(*i) == typeid(Instruction_mem_op_load)){
          Instruction_mem_op_load* in = (Instruction_mem_op_load*) i;
          std::string src = convert_item_to_str(in->get_src());
          std::string dst = convert_item_to_str(in->get_dst());
          std::string num = convert_item_to_str(in->get_num());
          auto op = (ArithmeticOp* ) in->get_op();
          std::string aop = op->get_op_char();
          if (aop == "+="){
            outputFile << "\taddq " << num << "(" << src << "), " << dst << "\n";
          } else{
            outputFile << "\tsubq " << num << "(" << src << "), " << dst << "\n";
          }


        } else if(typeid(*i) == typeid(Instruction_mem_store)){
          Instruction_mem_store* in = (Instruction_mem_store*) i;
          std::string src = convert_item_to_str(in->get_src());
          std::string x_reg = convert_item_to_str(in->get_x_reg());
          std::string num = convert_item_to_str(in->get_num());
          outputFile << "\tmovq " << src << "< " << num << "(" << x_reg << ")\n";

        } else if(typeid(*i) == typeid(Instruction_mem_op_store)){
          Instruction_mem_op_store* in = (Instruction_mem_op_store*) i; 
          std::string src = convert_item_to_str(in->get_src());
          std::string x_reg = convert_item_to_str(in->get_x_reg());
          std::string num = convert_item_to_str(in->get_num());
          auto op = (ArithmeticOp* ) in->get_op();
          std::string aop = op->get_op_char();
          if (aop == "+="){
            outputFile << "\taddq " << src << ", " << num << "(" << x_reg << ")\n";
          } else{
            outputFile << "\tsubq " << src << ", " << num << "(" << x_reg << ")\n";
          }

        } else if(typeid(*i) == typeid(Instruction_aop)){
          Instruction_aop* in = (Instruction_aop*) i;
          std::string src = convert_item_to_str(in->get_src());
          std::string dst = convert_item_to_str(in->get_dst());
          std::string op_char = convert_item_to_str(in->get_op());
          std::string assembly_instr = "";
          if (op_char == "+="){
            assembly_instr = "addq";
          } else if (op_char == "-="){
            assembly_instr = "subq";
          } else if (op_char == "*="){
            assembly_instr = "imulq";
          } else if (op_char == "&*"){
            assembly_instr = "andq";
          }
          outputFile << "\t" << assembly_instr << " " << src << ", " << dst << "\n";
        } else if(typeid(*i) == typeid(Instruction_sop)){
          Instruction_sop* in = (Instruction_sop*) i;
          std::string src = convert_item_to_str(in->get_src());
          std::string dst = convert_item_to_str(in->get_dst());
          std::string op_char = convert_item_to_str(in->get_op());
          std::string assembly_instr = "";
          if (op_char == "<<="){
            assembly_instr = "salq";
          } else if (op_char == ">>="){
            assembly_instr = "sarq";
          }
          outputFile << "\t" << assembly_instr << " " << src << ", " << dst << "\n";
        } else if(typeid(*i) == typeid(Instruction_cjump)){
          auto in = (Instruction_cjump*) i;
          std::string label = convert_item_to_str(in->get_label());

          Item* first = in->get_first();
          Item* second = in->get_second();
          bool first_reg = typeid(*first) == typeid(Register);
          bool second_reg = typeid(*second) == typeid(Register);
          std::string f_val = convert_item_to_str(first);
          std::string s_val = convert_item_to_str(second);
          auto op = (CompareOp* ) in->get_op();
          std::string cop = op->get_op_char();

          if ((!first_reg) && (!second_reg)){
            if (cop == "<"){
              int64_t f_num = stoi(f_val.substr(1,f_val.size()-1));
              int64_t s_num = stoi(s_val.substr(1,s_val.size()-1));
              bool val = f_num < s_num;
              if (val > 0){
                outputFile << "\tjmp " << label << "\n";
              }
              
            } else if (cop == "=") {
              int64_t f_num = stoi(f_val.substr(1,f_val.size()-1));
              int64_t s_num = stoi(s_val.substr(1,s_val.size()-1));
              bool val = f_num == s_num;
              if (val > 0){
                outputFile << "\tjmp " << label << "\n";
              }
            } else {
              int64_t f_num = stoi(f_val.substr(1,f_val.size()-1));
              int64_t s_num = stoi(s_val.substr(1,s_val.size()-1));
              bool val = f_num <= s_num;
              if (val > 0){
                outputFile << "\tjmp " << label << "\n";
              }
            }
          } else if (!first_reg){
            if (cop == "<"){
              outputFile << "\tcmpq " << f_val << ", " << s_val << "\n";
              outputFile << "\tjg " << label << "\n";
            } else if (cop == "=") {
              outputFile << "\tcmpq " << f_val << ", " << s_val << "\n";
              outputFile << "\tje " << label << "\n";
            } else {
              outputFile << "\tcmpq " << f_val << ", " << s_val << "\n";
              outputFile << "\tjge " << label << "\n";
            }
          } else {
            if (cop == "<"){
              outputFile << "\tcmpq " << f_val << ", " << s_val << "\n";
              outputFile << "\tjg " << label << "\n";
            } else if (cop == "=") {
              outputFile << "\tcmpq " << f_val << ", " << s_val << "\n";
              outputFile << "\tje " << label << "\n";
            } else {
              outputFile << "\tcmpq " << f_val << ", " << s_val << "\n";
              outputFile << "\tjge " << label << "\n";
            }
          }

        } else if(typeid(*i) == typeid(Instruction_label)){
          auto in = (Instruction_label*) i;
          std::string l = convert_item_to_str(in->get_label()) ;
          outputFile << "\t" << l << ":\n";

          
        } else if(typeid(*i) == typeid(Instruction_goto)){
          auto in = (Instruction_goto*) i;
          std::string l = convert_item_to_str(in->get_label()) ;
          outputFile << "\tjmp " << l << "\n";
        } else if(typeid(*i) == typeid(Instruction_pp)){
          auto in = (Instruction_pp*) i;
          std::string dst = convert_item_to_str(in->get_reg());
          outputFile << "\tinc " << dst << "\n";

        } else if(typeid(*i) == typeid(Instruction_mm)){
          auto in = (Instruction_mm*) i;
          std::string dst = convert_item_to_str(in->get_reg());
          outputFile << "\tdec " << dst << "\n";
        } else if(typeid(*i) == typeid(Instruction_at)){
          auto in = (Instruction_at*) i;
          std::string reg1 = convert_item_to_str(in->get_reg1());
          std::string reg2 = convert_item_to_str(in->get_reg2());
          std::string reg3 = convert_item_to_str(in->get_reg3());
          std::string num = convert_item_to_str(in->get_num());
          outputFile << "\tlea (" << reg2 << ", " << reg3 << ", " << num << "), " << reg1 << "\n";
        } else if(typeid(*i) == typeid(Instruction_call_u)){
          auto in = (Instruction_call_u*) i;
          Item* u = in->get_u();
          std::string num = convert_item_to_str(in->get_num());
          if (typeid(u) == typeid(Register)){
            outputFile << "\tsubq $8, %rsp\n";
            outputFile << "\tjmp *" << convert_item_to_str(u) << "\n";
          } else {
            int64_t num_val = std::stoi(num.substr(1,num.size()-1));
            int64_t sub_num;
            if ((num_val - 6) > 0){
              sub_num = num_val - 6;
            } else {
              sub_num = 0;
            }
            std::string sub_num_str = "$" + std::to_string(sub_num + 8);
            outputFile << "\t subq " << sub_num_str << ", %rsp\n";
            outputFile << "\t jmp " << convert_item_to_str(u) << "\n";
          }

        } else if(typeid(*i) == typeid(Instruction_call_print)){
          outputFile << "\tcall print \n";
        } else if(typeid(*i) == typeid(Instruction_call_input)){
          outputFile << "\tcall input \n";
        } else if(typeid(*i) == typeid(Instruction_call_allocate)){
          outputFile << "\tcall allocate \n";
        } else if(typeid(*i) == typeid(Instruction_call_tensor_error)){
          Instruction_call_tensor_error* in = (Instruction_call_tensor_error*) i;
          std::string num = convert_item_to_str(in->get_val());
          if (num == "1"){
            outputFile << "\t call array_tensor_error_null \n";
          } else if (num == "3"){
            outputFile << "\t call array_error \n";
          } else if (num == "4"){
            outputFile << "\t call tensor_error \n";
          }
        } else {
          std::cout << "typeid doesnt match anything\n";
        }
      }


    }

    //TODO

    /* 
     * Close the output file.
     */ 
    outputFile.close();
   
    return ;
  }
}
