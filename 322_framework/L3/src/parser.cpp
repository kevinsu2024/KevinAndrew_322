#include <iostream> //DELETE THIS LINE
#include <sched.h>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include <iterator>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <stdint.h>
#include <assert.h>
#include <variant>

#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/raw_string.hpp>

#include <L3.h>
#include <parser.h>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

using namespace pegtl;

namespace L3 {
    /*
    * Tokens parsed
    */
    std::vector<Item *> parsed_items;
    /*
    * Grammar rules for L3
    */
   struct name:
    pegtl::seq<
      pegtl::plus< 
        pegtl::sor<
          pegtl::alpha,
          pegtl::one< '_' >
        >
      >,
      pegtl::star<
        pegtl::sor<
          pegtl::alpha,
          pegtl::one< '_' >,
          pegtl::digit
        >
      >
    > {};
    /*
    * Strings
    */
    struct str_arrow : TAOCPP_PEGTL_STRING( "<-" ) {};
    struct str_call : TAOCPP_PEGTL_STRING( "call" ) {};
    struct str_break : TAOCPP_PEGTL_STRING( "br" ) {};
    struct str_define : TAOCPP_PEGTL_STRING( "define" ){};
    struct str_print : TAOCPP_PEGTL_STRING( "print" ) {};
    struct str_input : TAOCPP_PEGTL_STRING( "input" ) {};
    struct str_allocate : TAOCPP_PEGTL_STRING( "allocate" ) {};
    struct str_return : TAOCPP_PEGTL_STRING( "return" ) {};
    struct str_tensor_error : TAOCPP_PEGTL_STRING( "tensor-error" ) {};
    struct str_load : TAOCPP_PEGTL_STRING( "load" ) {};
    struct str_store : TAOCPP_PEGTL_STRING( "store" ) {};
    struct str_plus: TAOCPP_PEGTL_STRING( "+" ) {};
    struct str_minus: TAOCPP_PEGTL_STRING( "-" ) {};
    struct str_times: TAOCPP_PEGTL_STRING( "*" ) {};
    struct str_and: TAOCPP_PEGTL_STRING( "&" ) {};
    struct str_lshift: TAOCPP_PEGTL_STRING( "<<" ) {};
    struct str_rshift: TAOCPP_PEGTL_STRING( ">>" ) {};
    struct str_lt : TAOCPP_PEGTL_STRING( "<" ) {};
    struct str_leq : TAOCPP_PEGTL_STRING( "<=" ) {};
    struct str_gt : TAOCPP_PEGTL_STRING( "<" ) {};
    struct str_geq : TAOCPP_PEGTL_STRING( "<=" ) {};
    struct str_eq : TAOCPP_PEGTL_STRING( "=" ) {};

    /*
    * name-related rules
    */

    struct label_rule:
        pegtl::seq<
            pegtl::one<':'>,
            name
        > {};

    struct variable_rule:
        pegtl::seq<
            pegtl::one<'%'>,
            name
        > {};
    
    struct function_name_rule:
        pegtl::seq<
            pegtl::one<'@'>,
            name
        > {};
    
    /*
    * sep rules
    */ 

    struct comment: 
        pegtl::disable< 
            TAOCPP_PEGTL_STRING( "//" ), 
            pegtl::until< pegtl::eolf > 
        > {};

    struct seps: 
        pegtl::star< 
            pegtl::sor< 
                pegtl::ascii::space, 
                comment 
            > 
        > {};
    
    /*
    * number rules
    */
   struct number_rule:
        pegtl::seq<
            pegtl::opt<
                pegtl::sor<
                pegtl::one< '-' >,
                pegtl::one< '+' >
                >
            >,
            pegtl::plus< 
                pegtl::digit
            >
        > {};

    struct op_rule:
        pegtl::sor<
            str_plus,
            str_minus,
            str_times,
            str_and,
            str_lshift,
            str_rshift
        > {};
    
    struct cmp_rule:
        pegtl::sor<
            str_lt,
            str_leq,
            str_eq,
            str_geq,
            str_gt
        > {};

    struct t_rule:
        pegtl::sor<
            variable_rule,
            number_rule
        > {};
    
    struct s_rule:
        pegtl::sor<
            t_rule,
            function_name_rule,
            label_rule
        > {};
    
    struct u_rule:
        pegtl::sor<
            variable_rule,
            label_rule
        > {};
    
    /*
    * Instruction rules
    */

    struct Instruction_return_rule:
        pegtl::seq<
            str_return
        > {};
    
    struct Instruction_return_t_rule:
        pegtl::seq<
            str_return,
            seps,
            t_rule
        > {};
    
    
    struct Instruction_label_rule:
        pegtl::seq<
            label_rule
        > {};

    struct Instruction_assignment_rule:
        ptegtl::seq<
            variable_rule,
            seps,
            str_arrow,
            seps,
            s_rule
        > {};

    



    struct Instruction_rule:
        pegtl::sor<
            pegtl::seq< pegtl::at<Instruction_return_t_rule>             , Instruction_return_t_rule              >,
            pegtl::seq< pegtl::at<Instruction_return_rule>             , Instruction_return_rule              >,
            pegtl::seq< pegtl::at<Instruction_label_rule>             , Instruction_label_rule              >,




        > {};

    /*
    * function/grammar rules
    */



template<> struct action < Instruction_label_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /*
       * Fetch the last two tokens parsed.
       */
      auto label = parsed_items.back();
      parsed_items.pop_back();
      
      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_label(label);     
      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_assignment_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){

      auto src = parsed_items.back();
      parsed_items.pop_back();
      auto dst = parsed_items.back();
      parsed_items.pop_back();

      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_assignment(dst, src); 
      // i->instruction_name = ;

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
      auto stored_instr = currentF->instructions.back();
    }
  };


    
}