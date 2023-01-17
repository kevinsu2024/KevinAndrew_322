#include <string>
#include <iostream>
#include <fstream>

#include <code_generator.h>

using namespace std;

namespace L1{
  void generate_code(Program p){

    /* 
     * Open the output file.
     */ 
    std::ofstream outputFile;
    outputFile.open("prog.S");
    
    /* 
     * Generate target code
     */ 
    std::string boilerplate_code = ".text\n"
      "\t.globl go\n"
    "go:\n"
      "\tpushq %rbx\n"
      "\tpushq %rbp\n"
      "\tpushq %r12\n"
      "\tpushq %r13\n"
      "\tpushq %r14\n"
      "\tpushq %r15\n"
      "\tcall _[insert_entrypoint_label]\n"
      "\tpushq %r15\n"
      "\tpushq %r14\n"
      "\tpushq %r13\n"
      "\tpushq %r12\n"
      "\tpushq %rbx\n"
      "\tpushq %rbp\n"
      "\tpushq %r15\n"
      "\tretq\n";

    std::cout << boilerplate_code << "\n\n";
    std::cout << p.entryPointLabel << "\n\n";

    //TODO

    /* 
     * Close the output file.
     */ 
    outputFile.close();
   
    return ;
  }
}
