#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include<tuple>
#include <iterator>
#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include <assert.h>
#include <L2.h>
#include <parser.h>
#include <liveness_generator.h>
#include <interference_generator.h>
#include <spill_generator.h>
#include <color_graph.h>

void print_help (char *progName){
  std::cerr << "Usage: " << progName << " [-v] [-g 0|1] [-O 0|1|2] [-s] [-l] [-i] SOURCE" << std::endl;
  return ;
}

int main(
  int argc, 
  char **argv
  ){
  auto enable_code_generator = true;
  auto spill_only = false;
  auto interference_only = false;
  auto liveness_only = false;
  int32_t optLevel = 3;

  /* 
   * Check the compiler arguments.
   */
  auto verbose = false;
  if( argc < 2 ) {
    print_help(argv[0]);
    return 1;
  }
  int32_t opt;
  int64_t functionNumber = -1;
  while ((opt = getopt(argc, argv, "vg:O:sli")) != -1) {
    switch (opt){

      case 'l':
        liveness_only = true;
        break ;

      case 'i':
        interference_only = true;
        break ;

      case 's':
        spill_only = true;
        break ;

      case 'O':
        optLevel = strtoul(optarg, NULL, 0);
        break ;

      case 'g':
        enable_code_generator = (strtoul(optarg, NULL, 0) == 0) ? false : true ;
        break ;

      case 'v':
        verbose = true;
        break ;

      default:
        print_help(argv[0]);
        return 1;
    }
  }

  /*
   * Parse the input file.
   */
  L2::Program p;
  if (spill_only){

    /* 
     * Parse an L2 function and the spill arguments.
     */
    
    p = L2::parse_spill_file(argv[optind]);
 
  } else if (liveness_only){

    /*
     * Parse an L2 function.
     */
    p = L2::parse_function_file(argv[optind]);


  } else if (interference_only){
    /*
     * Parse an L2 function.
     */
    p = L2::parse_function_file(argv[optind]);

  } else {
    /* 
     * Parse the L2 program.
     */
    p = L2::parse_file(argv[optind]);
  }

  /*
   * Special cases.
   */
  if (spill_only){

    L2::Function* func = p.functions.back();

    std::string spill_var = p.spill_variable_name;
    std::string new_var = p.spill_string;
    L2::Function* new_func = L2::generate_spill_code(func,spill_var, new_var);
    std::cout << new_func->to_string();
    return 0;
  }

  /*
   * Liveness test.
   */
  if (liveness_only){
    // std::cerr << "generating\n";
    L2::Function* func = p.functions.back();
        //CHANGE ME: GET INSTRUCTIONS FROM ALL FUNCTIONS
        // std::cerr << p.functions.size() << "\n";
    auto _ = L2::generate_liveness(func, true);
    return 0;
  }

  /*
   * Interference graph test.
   */
  if (interference_only){
    L2::Function* func = p.functions.back();
    auto _ = L2::generate_interference(func,true);
    return 0;
  }


  //general full case
  auto functions = p.functions;
  std::ofstream outputFile;
  outputFile.open("prog.L1");

  outputFile << "(" + p.entryPointLabel + "\n";
  for(int64_t i = 0; i < functions.size(); i++){
    L2::Function* f = functions.at(i);
    // std::cout<< "oring func is\n" << f->to_string();
    auto res = L2::generate_mapping(f);
    auto mapping = res.first;
    auto new_f = res.second;
    for(auto p : mapping){
      std::cout << p.first << " mapped to " << p.second << "\n";
    }
    outputFile << "\t" << L2::translate_to_code(mapping, new_f);
  }
  outputFile << ")\n";


  /*
   * Generate the target code.
   */
  if (enable_code_generator){
    //TODO
  }
  outputFile.close();
  return 0;
}

