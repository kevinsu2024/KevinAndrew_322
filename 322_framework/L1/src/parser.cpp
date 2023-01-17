/*
 * SUGGESTION FROM THE CC TEAM:
 * double check the order of actions that are fired.
 * You can do this in (at least) two ways:
 * 1) by using gdb adding breakpoints to actions
 * 2) by adding printing statements in each action
 *
 * For 2), we suggest writing the code to make it straightforward to enable/disable all of them 
 * (e.g., assuming shouldIPrint is a global variable
 *    if (shouldIPrint) std::cerr << "MY OUTPUT" << std::endl;
 * )
 */
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

#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/raw_string.hpp>

#include <L1.h>
#include <parser.h>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

using namespace pegtl;

namespace L1 {

  /* 
   * Tokens parsed
   */ 
  std::vector<Item *> parsed_items;

  /* 
   * Grammar rules from now on.
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
   * Keywords.
   */
  struct str_return : TAOCPP_PEGTL_STRING( "return" ) {};
  struct str_arrow : TAOCPP_PEGTL_STRING( "<-" ) {};
  struct str_rdi : TAOCPP_PEGTL_STRING( "rdi" ) {};
  struct str_rax : TAOCPP_PEGTL_STRING( "rax" ) {};
  struct str_rsi : TAOCPP_PEGTL_STRING( "rsi" ) {};
  struct str_rdx : TAOCPP_PEGTL_STRING( "rdx" ) {};
  struct str_rcx : TAOCPP_PEGTL_STRING( "rcx" ) {};
  struct str_r8 : TAOCPP_PEGTL_STRING( "r8" ) {};
  struct str_r9 : TAOCPP_PEGTL_STRING( "r9" ) {};
  struct str_rbx : TAOCPP_PEGTL_STRING( "rbx" ) {};
  struct str_rbp : TAOCPP_PEGTL_STRING( "rbp" ) {};
  struct str_r10 : TAOCPP_PEGTL_STRING( "r10" ) {};
  struct str_r11 : TAOCPP_PEGTL_STRING( "r11" ) {};
  struct str_r12 : TAOCPP_PEGTL_STRING( "r12" ) {};
  struct str_r13 : TAOCPP_PEGTL_STRING( "r13" ) {};
  struct str_r14 : TAOCPP_PEGTL_STRING( "r14" ) {};
  struct str_r15 : TAOCPP_PEGTL_STRING( "r15" ) {};
  struct str_rsp : TAOCPP_PEGTL_STRING( "rsp" ) {};
  struct str_mem : TAOCPP_PEGTL_STRING( "mem" ) {};
  struct str_plus_eq : TAOCPP_PEGTL_STRING( "+=" ) {};
  struct str_minus_eq : TAOCPP_PEGTL_STRING( "-=" ) {};
  struct str_times_eq : TAOCPP_PEGTL_STRING( "*=" ) {};
  struct str_and_eq : TAOCPP_PEGTL_STRING( "&=" ) {};
  struct str_lshift : TAOCPP_PEGTL_STRING( "<<=" ) {};
  struct str_rshift : TAOCPP_PEGTL_STRING( ">>=" ) {};
  struct str_lt : TAOCPP_PEGTL_STRING( "<" ) {};
  struct str_leq : TAOCPP_PEGTL_STRING( "<=" ) {};
  struct str_eq : TAOCPP_PEGTL_STRING( "=" ) {};
  struct str_cjump : TAOCPP_PEGTL_STRING( "cjump" ) {};
  struct str_goto : TAOCPP_PEGTL_STRING( "goto" ) {};
  struct str_pp : TAOCPP_PEGTL_STRING( "++" ) {};
  struct str_mm : TAOCPP_PEGTL_STRING( "--" ) {};
  struct str_at : TAOCPP_PEGTL_STRING( "@" ) {};
  struct str_call : TAOCPP_PEGTL_STRING( "call" ) {};
  struct str_print : TAOCPP_PEGTL_STRING( "print" ) {};
  struct str_input : TAOCPP_PEGTL_STRING( "input" ) {};
  struct str_allocate : TAOCPP_PEGTL_STRING( "allocate" ) {};
  struct str_tensor_error : TAOCPP_PEGTL_STRING( "tensor-error" ) {};


  struct label:
    pegtl::seq<
      pegtl::one<':'>,
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
  * register rules
  */ 
  struct register_rdi_rule:
      str_rdi {};

  struct register_rax_rule:
      str_rax {};

  struct register_rsi_rule:
      str_rsi {};
   
  struct register_rdx_rule:
      str_rdx {};

  struct register_rcx_rule:
      str_rcx {};

  struct register_r8_rule:
      str_r8 {};

  struct register_r9_rule:
      str_r9 {};

  struct register_rbx_rule:
      str_rbx {};

  struct register_rbp_rule:
      str_rbp {};

  struct register_r10_rule:
      str_r10 {};

  struct register_r11_rule:
      str_r11 {};

  struct register_r12_rule:
      str_r12 {};
  
  struct register_r13_rule:
      str_r13 {};

  struct register_r14_rule:
      str_r14 {};

  struct register_r15_rule:
      str_r15 {};

  struct register_rsp_rule:
      str_rsp {};
  
  struct shift_register_rule:
    pegtl::sor<
      register_rcx_rule
    > {};
  
  struct argument_register_rule:
    pegtl::sor<
      register_rdi_rule,
      register_rsi_rule,
      register_rdx_rule,
      shift_register_rule,
      register_r8_rule,
      register_r9_rule
    > {};
  
  struct register_rule:
    pegtl::sor<
      argument_register_rule,
      register_rax_rule,
      register_rbx_rule,
      register_rbp_rule,
      register_r10_rule,
      register_r11_rule,
      register_r12_rule,
      register_r13_rule,
      register_r14_rule,
      register_r15_rule
    > {};
  struct x_register_rule:
    pegtl::sor<
      register_rule,
      register_rsp_rule
    > {};

  /*
  * number rules
  */

  struct number:
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
    >{};

  struct instruction_number:
    number {} ;
  
  struct aop:
    pegtl::sor<
      str_plus_eq,
      str_minus_eq,
      str_times_eq,
      str_and_eq
  > {};
  struct plus_minus_op:
    pegtl::sor<
      str_plus_eq,
      str_minus_eq
    > {};
  struct sop:
    pegtl::sor<
      str_lshift,
      str_rshift
  > {};
  struct cmp:
    pegtl::sor<
      pegtl::seq<pegtl::at<str_leq>, str_leq>,
      pegtl::seq<pegtl::at<str_lt>, str_lt>,
      pegtl::seq<pegtl::at<str_eq>, str_eq>
  > {};

  struct label_rule:
    label {};
  struct t_rule:
    pegtl::sor<
      x_register_rule,
      instruction_number
    > {};
  
  struct s_rule:
    pegtl::sor<
      t_rule,
      label_rule,
      function_name_rule
  > {};

  struct u_rule:
    pegtl::sor<
      register_rule,
      label_rule,
      function_name_rule
  > {};


  struct function_name:
    label {};

  struct argument_number:
    number {};

  struct local_number:
    number {} ;



  
  /*
  * instruction rules
  */
  struct Instruction_return_rule:
    pegtl::seq<
      str_return
    > { };

  struct Instruction_cmp_assignment_rule:
    pegtl::seq<
      register_rule,
      seps,
      str_arrow,
      seps,
      t_rule,
      seps,
      cmp,
      seps,
      t_rule
    > {};

  struct Instruction_assignment_rule:
    pegtl::seq<
      register_rule,
      seps,
      str_arrow,
      seps,
      s_rule
    > {};
  struct Instruction_aop_rule:
    pegtl::seq<
      register_rule,
      seps,
      aop,
      seps,
      t_rule
    > {};
  struct Instruction_sop_rule:
    pegtl::seq<
      register_rule,
      seps,
      sop,
      seps,
      pegtl::sor<
        shift_register_rule,
        instruction_number>
    > {};
  struct Instruction_mem_load_rule:
    pegtl::seq<
      register_rule,
      seps,
      str_arrow,
      seps,
      str_mem,
      seps,
      x_register_rule,
      seps,
      instruction_number
    > {};

  struct Instruction_mem_store_rule:
    pegtl::seq<
      str_mem,
      seps,
      x_register_rule,
      seps,
      instruction_number,
      seps,
      str_arrow,
      seps,
      s_rule
    > {};



  struct Instruction_cjump_rule:
    pegtl::seq<
      str_cjump,
      seps,
      t_rule,
      seps,
      cmp,
      seps,
      t_rule,
      seps,
      label_rule
    > {};

  struct Instruction_label_rule:
    pegtl::seq<
      label_rule
    > {};

  struct Instruction_goto_rule:
    pegtl::seq<
      str_goto,
      seps,
      label_rule
    > {};

  struct Instruction_mem_op_store_rule:
    pegtl::seq<
      str_mem,
      seps,
      x_register_rule,
      seps,
      instruction_number,
      seps,
      plus_minus_op,
      seps,
      t_rule
    > {};

  struct Instruction_mem_op_load_rule:
    pegtl::seq<
      register_rule,
      seps,
      plus_minus_op,
      seps,
      str_mem,
      seps,
      x_register_rule,
      seps,
      instruction_number
    > {};

  struct Instruction_pp_rule:
    pegtl::seq<
      register_rule,
      seps,
      str_pp
    > {};
  struct Instruction_mm_rule:
    pegtl::seq<
      register_rule,
      seps,
      str_mm
    > {};

  struct Instruction_at_rule:
    pegtl::seq<
      register_rule,
      seps,
      str_at,
      seps,
      register_rule,
      seps,
      register_rule,
      seps,
      instruction_number
    > {};

  struct Instruction_call_u_rule:
    pegtl::seq<
      str_call,
      seps,
      u_rule,
      seps,
      instruction_number
    > {};

  struct Instruction_call_print_rule:
    pegtl::seq<
      str_call,
      seps,
      str_print,
      seps,
      pegtl::one< '1' >
    > {};

  struct Instruction_call_input_rule:
    pegtl::seq<
      str_call,
      seps,
      str_input,
      seps,
      pegtl::one< '0' >
    > {};

  struct Instruction_call_allocate_rule:
    pegtl::seq<
      str_call,
      seps,
      str_allocate,
      seps,
      pegtl::one< '2' >
    > {};

  struct Instruction_call_tensor_error_rule:
    pegtl::seq<
      str_call,
      seps,
      str_tensor_error,
      seps,
      instruction_number
    > {};
  
   struct Instruction_function_start_rule:
      pegtl::seq<
      seps,
      pegtl::one< '(' >,
      seps,
      function_name_rule,
      seps
    > {};


  struct Instruction_rule:
    pegtl::sor<
      pegtl::seq< pegtl::at<Instruction_return_rule>            , Instruction_return_rule             >,
      pegtl::seq< pegtl::at<Instruction_cjump_rule>             , Instruction_cjump_rule              >,
      pegtl::seq< pegtl::at<Instruction_pp_rule>                , Instruction_pp_rule                 >,
      pegtl::seq< pegtl::at<Instruction_mm_rule>                , Instruction_mm_rule                 >,
      pegtl::seq< pegtl::at<Instruction_call_u_rule>            , Instruction_call_u_rule             >,
      pegtl::seq< pegtl::at<Instruction_call_print_rule>        , Instruction_call_print_rule         >,
      pegtl::seq< pegtl::at<Instruction_call_input_rule>        , Instruction_call_input_rule         >,
      pegtl::seq< pegtl::at<Instruction_call_allocate_rule>     , Instruction_call_allocate_rule      >,
      pegtl::seq< pegtl::at<Instruction_call_tensor_error_rule> , Instruction_call_tensor_error_rule  >,
      pegtl::seq< pegtl::at<Instruction_at_rule>                , Instruction_at_rule                 >,
      pegtl::seq< pegtl::at<Instruction_cmp_assignment_rule>    , Instruction_cmp_assignment_rule     >,
      pegtl::seq< pegtl::at<Instruction_assignment_rule>        , Instruction_assignment_rule         >,
      pegtl::seq< pegtl::at<Instruction_mem_op_load_rule>       , Instruction_mem_op_load_rule        >,
      pegtl::seq< pegtl::at<Instruction_mem_op_store_rule>      , Instruction_mem_op_store_rule       >,
      pegtl::seq< pegtl::at<Instruction_mem_load_rule>          , Instruction_mem_load_rule           >,
      pegtl::seq< pegtl::at<Instruction_mem_store_rule>         , Instruction_mem_store_rule          >,
      pegtl::seq< pegtl::at<Instruction_aop_rule>               , Instruction_aop_rule                >,
      pegtl::seq< pegtl::at<Instruction_sop_rule>               , Instruction_sop_rule                >,
      pegtl::seq< pegtl::at<Instruction_goto_rule>              , Instruction_goto_rule               >,
      pegtl::seq< pegtl::at<Instruction_label_rule>             , Instruction_label_rule              >,
      pegtl::seq< pegtl::at<Instruction_function_start_rule>    , Instruction_function_start_rule     >
    > {};

  struct Instructions_rule:
    pegtl::plus<
      pegtl::seq<
        seps,
        Instruction_rule,
        seps
      >
    > {};

  struct Function_rule:
    pegtl::seq<
      seps,
      pegtl::one< '(' >,
      seps,
      function_name_rule,
      seps,
      argument_number,
      seps,
      local_number,
      seps,
      Instructions_rule,
      seps,
      pegtl::one< ')' >
    > {};

  struct Functions_rule:
    pegtl::plus<
      seps,
      Function_rule,
      seps
    > {};

  struct entry_point_rule:
    pegtl::seq<
      seps,
      pegtl::one< '(' >,
      seps,
      function_name_rule,
      seps,
      Functions_rule,
      seps,
      pegtl::one< ')' >,
      seps
    > { };

  struct grammar : 
    pegtl::must< 
      entry_point_rule
    > {};

  /* 
   * Actions attached to grammar rules.
   */
  template< typename Rule >
  struct action : pegtl::nothing< Rule > {};

  template<> struct action < function_name_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){
      if (p.entryPointLabel.empty()){
        p.entryPointLabel = in.string();
      } else {
        std::string input_string = in.string();
        auto fn = new FunctionName(input_string);
        parsed_items.push_back(fn);
        auto newF = new Function();
        newF->name = in.string();
        p.functions.push_back(newF);
      }
    }
  };

  template<> struct action < argument_number > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      currentF->arguments = std::stoll(in.string());
    }
  };

  template<> struct action < local_number > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      currentF->locals = std::stoll(in.string());
    }
  };

  template<> struct action < instruction_number > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      auto n = new InstructionNumber(stoi(in.string()));
      parsed_items.push_back(n);
    }
  };

  template<> struct action < str_return > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_ret();
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < register_rdi_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      auto r = new Register(RegisterID::rdi);
      parsed_items.push_back(r);
    }
  };

  template<> struct action < register_rax_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      auto r = new Register(RegisterID::rax);
      parsed_items.push_back(r);
    }
  };


  template<> struct action < register_rsi_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      auto r = new Register(RegisterID::rsi);
      parsed_items.push_back(r);
    }
  };

  template<> struct action < register_rdx_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      auto r = new Register(RegisterID::rdx);
      parsed_items.push_back(r);
    }
  };

  template<> struct action < register_rcx_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){

      auto r = new Register(RegisterID::rcx);
      parsed_items.push_back(r);
    }
  };


  template<> struct action < register_r8_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      auto r = new Register(RegisterID::r8);
      parsed_items.push_back(r);
    }
  };


  template<> struct action < register_r9_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      auto r = new Register(RegisterID::r9);
      parsed_items.push_back(r);
    }
  };


  template<> struct action < register_rbx_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      auto r = new Register(RegisterID::rbx);
      parsed_items.push_back(r);
    }
  };


  template<> struct action < register_rbp_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      auto r = new Register(RegisterID::rbp);
      parsed_items.push_back(r);
    }
  };

  template<> struct action < register_r10_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      auto r = new Register(RegisterID::r10);
      parsed_items.push_back(r);
    }
  };
  template<> struct action < register_r11_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      auto r = new Register(RegisterID::r11);
      parsed_items.push_back(r);
    }
  };
  template<> struct action < register_r12_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      auto r = new Register(RegisterID::r12);
      parsed_items.push_back(r);
    }
  };
  template<> struct action < register_r13_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      auto r = new Register(RegisterID::r13);
      parsed_items.push_back(r);
    }
  };
  template<> struct action < register_r14_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      auto r = new Register(RegisterID::r14);
      parsed_items.push_back(r);
    }
  };
  template<> struct action < register_r15_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      auto r = new Register(RegisterID::r15);
      parsed_items.push_back(r);
    }
  };
  template<> struct action < register_rsp_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      auto r = new Register(RegisterID::rsp);
      parsed_items.push_back(r);
    }
  };

  

  template<> struct action < label_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      std::cout << "\n\nherer8\n\n";
      auto l = new InstructionLabel(in.string());
      parsed_items.push_back(l);
    }
  };

  template<> struct action < cmp > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      std::cout << "\n\nadding op " << in.string();
      std:: cout << "size b4 is " << parsed_items.size();
      auto op = new CompareOp(in.string());
      parsed_items.push_back(op);
      std::cout << "size after is " << parsed_items.size() << "\n\n";
    }
  };

  template<> struct action < aop > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      auto op = new ArithmeticOp(in.string());
      parsed_items.push_back(op);
    }
  };

  template<> struct action < plus_minus_op > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      auto op = new ArithmeticOp(in.string());
      parsed_items.push_back(op);
    }
  };

  template<> struct action < sop > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      auto op = new ShiftOp(in.string());
      parsed_items.push_back(op);
      std::cout << "\n\n" << parsed_items.size() << "\n\n";
    }
  };
  template<> struct action < Instruction_function_start_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      std::cout << "\n\nherer func start\n\n";
      std::cout << "\n\n" << parsed_items.size() << "   func_start_before_pop \n\n";
      auto name = parsed_items.back();
      parsed_items.pop_back();
      std::cout << "\n\n" << parsed_items.size() << "   func_start_after_pop \n\n";
    }
  };

  template<> struct action < Instruction_cmp_assignment_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      std::cout << "\n\nherer7\n\n";
      auto currentF = p.functions.back();
      
      std::cout << "\n\n" << parsed_items.size() << "   cmp_before_pop \n\n";
      auto second = parsed_items.back();
      parsed_items.pop_back();
      auto op = parsed_items.back();
      parsed_items.pop_back();
      auto first = parsed_items.back();
      parsed_items.pop_back();
      auto dst = parsed_items.back();
      parsed_items.pop_back();
      std::cout << "\n\n" << parsed_items.size() << "   cmp_after_pop \n\n";

      auto i = new Instruction_cmp_assignment(dst, first, second, op); 

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_assignment_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){
      std::cout << "\n\nherer6\n\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /*
       * Fetch the last two tokens parsed.
       */
      auto src = parsed_items.back();
      parsed_items.pop_back();
      auto dst = parsed_items.back();
      parsed_items.pop_back();

      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_assignment(dst, src); 

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
      auto stored_instr = currentF->instructions.back();
    }
  };

  template<> struct action < Instruction_mem_op_load_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){
      std::cout << "\n\nherer50\n\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /*
       * Fetch the last two tokens parsed.
       */
      std::cout << "\n\n size before pop herer50" << parsed_items.size() << "\n\n";
      auto num = parsed_items.back();
      parsed_items.pop_back();
      auto src = parsed_items.back();
      parsed_items.pop_back();
      auto op = parsed_items.back();
      parsed_items.pop_back();
      auto dst = parsed_items.back();
      parsed_items.pop_back();
      std::cout << "\n\n size after pop herer50" << parsed_items.size() << "\n\n";
      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_mem_op_load(dst, src, num, op); 

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_mem_load_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){
      std::cout << "\n\nherer5\n\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /*
       * Fetch the last two tokens parsed.
       */
      auto num = parsed_items.back();
      parsed_items.pop_back();
      auto src = parsed_items.back();
      parsed_items.pop_back();
      auto dst = parsed_items.back();
      parsed_items.pop_back();
      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_mem_load(dst, src, num); 

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  };

  

  template<> struct action < Instruction_mem_store_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){
      std::cout << "\n\nherer4\n\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /*
       * Fetch the last two tokens parsed.
       */
      std::cout << "\n\n size before pop herer4" << parsed_items.size() << "\n\n"; 
      auto s = parsed_items.back();
      parsed_items.pop_back();
      auto num = parsed_items.back();
      parsed_items.pop_back();
      auto x_register = parsed_items.back();
      parsed_items.pop_back();
      std::cout << "\n\n size after pop herer4" << parsed_items.size() << "\n\n";
      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_mem_store(s, num, x_register); 

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_mem_op_store_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){
      std::cout << "\n\nherer40\n\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /*
       * Fetch the last two tokens parsed.
       */
      std::cout << "\n\n size before pop herer40" << parsed_items.size() << "\n\n";
      auto t_rule = parsed_items.back();
      parsed_items.pop_back();
      auto op = parsed_items.back();
      parsed_items.pop_back();
      auto num = parsed_items.back();
      parsed_items.pop_back();
      auto x_register = parsed_items.back();
      parsed_items.pop_back();
      std::cout << "\n\n size after pop herer40" << parsed_items.size() << "\n\n";
      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_mem_op_store(t_rule, num, x_register, op); 

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_aop_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){
      std::cout << "\n\nherer3\n\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /*
       * Fetch the last two tokens parsed.
       */
      auto t = parsed_items.back();
      parsed_items.pop_back();
      auto op = parsed_items.back();
      parsed_items.pop_back();
      auto r = parsed_items.back();
      parsed_items.pop_back();
      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_aop(t, op, r); 

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  };


  template<> struct action < Instruction_sop_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){
      std::cout << "\n\nherer2\n\n";

      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();
      std::cout << "\n\n" << parsed_items.size() << "   num 2\n\n";

      /*
       * Fetch the last two tokens parsed.
       */
      auto shift = parsed_items.back();
      parsed_items.pop_back();
      auto op = parsed_items.back();
      parsed_items.pop_back();
      auto r = parsed_items.back();
      parsed_items.pop_back();
      //std::cout<<"xd";
      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_sop(shift, op, r); 
      std::cout << "\n\n" << parsed_items.size() << "  num 3\n\n";

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
      std::cout << "\n\n" << parsed_items.size() << "   num 4 \n\n";
    }
  };

  template<> struct action < Instruction_cjump_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){

      std::cout << "\n\nherer1\n\n";
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /*
       * Fetch the last two tokens parsed.
       */
      auto label = parsed_items.back();
      parsed_items.pop_back();
      auto t2 = parsed_items.back();
      parsed_items.pop_back();
      auto op = parsed_items.back();
      parsed_items.pop_back();
      auto t1 = parsed_items.back();
      parsed_items.pop_back();
      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_cjump(t1, op, t2, label); 

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_label_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){

      std::cout << "\n\nherer0\n\n";
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

  template<> struct action < Instruction_goto_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){

      std::cout << "\n\nherer-1\n\n";
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
      auto i = new Instruction_goto(label); 

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  };


  template<> struct action < Instruction_pp_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){

      std::cout << "\n\nherer-2\n\n";
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /*
       * Fetch the last two tokens parsed.
       */
      auto reg = parsed_items.back();
      parsed_items.pop_back();
      
      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_pp(reg); 

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_mm_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){

      std::cout << "\n\nherer-3\n\n";
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /*
       * Fetch the last two tokens parsed.
       */
      auto reg = parsed_items.back();
      parsed_items.pop_back();
      
      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_mm(reg); 

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_at_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){

      std::cout << "\n\nherer-4\n\n";
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /*
       * Fetch the last two tokens parsed.
       */
      auto num = parsed_items.back();
      parsed_items.pop_back();
      auto reg3 = parsed_items.back();
      parsed_items.pop_back();
      auto reg2 = parsed_items.back();
      parsed_items.pop_back();
      auto reg1 = parsed_items.back();
      parsed_items.pop_back();
      
      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_at(reg1,reg2,reg3,num); 

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_call_u_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){

      std::cout << "\n\nherer-5\n\n";
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /*
       * Fetch the last two tokens parsed.
       */
      auto num = parsed_items.back();
      parsed_items.pop_back();
      auto u = parsed_items.back();
      parsed_items.pop_back();
      
      
      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_call_u(u, num); 

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_call_print_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){

      std::cout << "\n\nherer-5\n\n";
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();
      
      
      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_call_print(); 

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_call_input_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){

      std::cout << "\n\nherer-5\n\n";
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      
      
      
      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_call_input(); 

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_call_allocate_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){

      std::cout << "\n\nherer-5\n\n";
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      
      
      
      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_call_allocate(); 

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_call_tensor_error_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){

      std::cout << "\n\nherer-5\n\n";
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /*
       * Fetch the last two tokens parsed.
       */
      auto num = parsed_items.back();
      parsed_items.pop_back();
      
      
      
      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_call_tensor_error(num); 

      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  };

  

  Program parse_file (char *fileName){

    /* 
     * Check the grammar for some possible issues.
     */
    pegtl::analyze< grammar >();

    /*
     * Parse.
     */
    file_input< > fileInput(fileName);
    Program p;
    parse< grammar, action >(fileInput, p);
    return p;
  }

}
