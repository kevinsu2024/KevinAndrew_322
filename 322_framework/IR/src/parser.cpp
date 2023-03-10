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

#include <IR.h>
#include <parser.h>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

using namespace pegtl;

namespace IR {
    /*
    * Tokens parsed
    */
    std::vector<Item *> parsed_items;
    /*
    * Grammar rules for IR
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
    struct str_arrow : TAOCPP_PEGTL_STRING("<-") {};
    struct str_call : TAOCPP_PEGTL_STRING( "call" ) {};
    struct str_branch : TAOCPP_PEGTL_STRING( "br" ) {};
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
    struct str_gt : TAOCPP_PEGTL_STRING( ">" ) {};
    struct str_geq : TAOCPP_PEGTL_STRING( ">=" ) {};
    struct str_eq : TAOCPP_PEGTL_STRING( "=" ) {};
    struct str_at_main : TAOCPP_PEGTL_STRING( "@main" ) {};
    struct str_void : TAOCPP_PEGTL_STRING( "void" ) {};
    struct str_code : TAOCPP_PEGTL_STRING( "code" ) {};
    struct str_tuple : TAOCPP_PEGTL_STRING( "tuple" ) {};
    struct str_int : TAOCPP_PEGTL_STRING( "int64" ) {};
    struct str_type_bracket : TAOCPP_PEGTL_STRING( "[]" ) {};
    struct str_length: TAOCPP_PEGTL_STRING("length") {};
    struct str_new: TAOCPP_PEGTL_STRING("new") {};
    struct str_array: TAOCPP_PEGTL_STRING("Array") {};
    struct str_Tuple: TAOCPP_PEGTL_STRING("Tuple") {};

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

    


    struct type_rule:
        pegtl::sor<
            str_tuple,
            str_code,
            pegtl::seq<
                str_int,
                pegtl::star<
                    str_type_bracket
                >
            >
        > {};

    struct function_type_rule:
        pegtl::sor<
            str_void,
            type_rule
        > {};

    struct op_rule:
        pegtl::sor<
            pegtl::seq<pegtl::at<str_lshift>,str_lshift>,
            pegtl::seq<pegtl::at<str_rshift>,str_rshift>,
            pegtl::seq<pegtl::at<str_times>,str_times>,
            pegtl::seq<pegtl::at<str_plus>,str_plus>,
            pegtl::seq<pegtl::at<str_minus>,str_minus>,
            pegtl::seq<pegtl::at<str_and>,str_and>
        > {};
    
    struct cmp_rule:
        pegtl::sor<
            pegtl::seq<pegtl::at<str_geq>,str_geq>,
            pegtl::seq<pegtl::at<str_leq>,str_leq>,
            pegtl::seq<pegtl::at<str_lt>,str_lt>,
            pegtl::seq<pegtl::at<str_gt>,str_gt>,
            pegtl::seq<pegtl::at<str_eq>,str_eq>
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
            function_name_rule
        > {};
    
    struct arguments_rule:
        pegtl::seq<
            t_rule,
            seps,
            pegtl::star<
                pegtl::one<','>,
                seps,   
                t_rule
            >
        > {};

    struct function_argument:
        pegtl::seq<
            type_rule,
            seps,
            variable_rule
        > {};

    struct function_arguments_rule:
        pegtl::seq<
            function_argument,
            seps,
            pegtl::star<
                pegtl::one<','>,
                seps,
                function_argument
            >
        > {};

    struct standard_library_rule:
        pegtl::sor<
            str_print,
            str_input,
            str_tensor_error
        > {};

    struct callee_rule:
        pegtl::sor<
            u_rule,
            standard_library_rule
        > {};

    struct instruction_bracket_rule:
        pegtl::seq<
            pegtl::one<'['>,
            t_rule,
            pegtl::one<']'>
        > {};
    
    
    /*
    * Instruction rules
    */

    struct Instruction_declaration_rule:
        pegtl::seq<
            type_rule,
            seps,
            variable_rule
        > {};

    struct Instruction_assignment_rule:
        pegtl::seq<
            variable_rule,
            seps,
            str_arrow,
            seps,
            s_rule
        > {};

    struct Instruction_op_assignment_rule:
        pegtl::seq<
            variable_rule,
            seps,
            str_arrow,
            seps,
            t_rule,
            seps,
            op_rule,
            seps,
            t_rule
        > {};

    struct Instruction_cmp_assignment_rule:
        pegtl::seq<
            variable_rule,
            seps,
            str_arrow,
            seps,
            t_rule,
            seps,
            cmp_rule,
            seps,
            t_rule
        > {};

    struct Instruction_load_assignment_rule:
        pegtl::seq<
            variable_rule,
            seps,
            str_arrow,
            seps,
            variable_rule,
            seps,
            pegtl::plus<
                instruction_bracket_rule
            >
        > {};

    struct Instruction_store_assignment_rule:
        pegtl::seq<
            variable_rule,
            seps,
            pegtl::plus<
                instruction_bracket_rule
            >,
            seps,
            str_arrow,
            seps,
            s_rule
        > {};

    struct Instruction_array_length_rule:
        pegtl::seq<
            variable_rule,
            seps,
            str_arrow,
            seps,
            str_length,
            seps,
            variable_rule,
            seps,
            t_rule
        > {};

    struct Instruction_tuple_length_rule:
        pegtl::seq<
            variable_rule,
            seps,
            str_arrow,
            seps,
            str_length,
            seps,
            variable_rule,
            pegtl::until< pegtl::eolf, t_rule>
        > {};

    struct Instruction_call_rule:
        pegtl::seq<
            seps,
            str_call,
            seps,
            callee_rule,
            seps,
            pegtl::one<'('>,
            seps,
            pegtl::star<arguments_rule>,
            seps,
            pegtl::one<')'>
        > {};

    struct Instruction_call_assignment_rule:
        pegtl::seq<
            seps,
            variable_rule,
            seps,
            str_arrow,
            seps,
            str_call,
            seps,
            callee_rule,
            seps,
            pegtl::one<'('>,
            seps,
            pegtl::star<arguments_rule>,
            seps,
            pegtl::one<')'>
        > {};

    struct Instruction_create_array_rule:
        pegtl::seq<
            variable_rule,
            seps,
            str_arrow,
            seps,
            str_new,
            seps,
            str_array,
            seps,
            pegtl::one<'('>,
            arguments_rule,
            pegtl::one<')'>
        > {};

    struct Instruction_create_tuple_rule:
        pegtl::seq<
            variable_rule,
            seps,
            str_arrow,
            seps,
            str_new,
            seps,
            str_Tuple,
            seps,
            pegtl::one<'('>,
            t_rule,
            pegtl::one<')'>
        > {};

    struct Instruction_rule:
        pegtl::sor<
            pegtl::seq< pegtl::at<Instruction_declaration_rule>  , Instruction_declaration_rule   >,
            pegtl::seq< pegtl::at<Instruction_load_assignment_rule> , Instruction_load_assignment_rule  >,
            pegtl::seq< pegtl::at<Instruction_cmp_assignment_rule>  , Instruction_cmp_assignment_rule   >,
            pegtl::seq< pegtl::at<Instruction_op_assignment_rule>   , Instruction_op_assignment_rule    >,
            pegtl::seq< pegtl::at<Instruction_assignment_rule>      , Instruction_assignment_rule       >,
            pegtl::seq< pegtl::at<Instruction_store_assignment_rule>, Instruction_store_assignment_rule >,
            pegtl::seq< pegtl::at<Instruction_tuple_length_rule>            , Instruction_tuple_length_rule             >,
            pegtl::seq< pegtl::at<Instruction_array_length_rule>            , Instruction_array_length_rule             >,
            pegtl::seq< pegtl::at<Instruction_call_assignment_rule> , Instruction_call_assignment_rule  >,
            pegtl::seq< pegtl::at<Instruction_call_rule>            , Instruction_call_rule             >,
            pegtl::seq< pegtl::at<Instruction_create_array_rule>            , Instruction_create_array_rule             >,
            pegtl::seq< pegtl::at<Instruction_create_tuple_rule>            , Instruction_create_tuple_rule             >
        > {};
    
    struct Instructions_rule:
        pegtl::star<
            pegtl::seq<
                seps,
                Instruction_rule,
                seps
            >
        > {};

    


    struct Instruction_branch_rule:
        pegtl::seq<
            str_branch,
            seps,
            label_rule
        > {};
    
    struct Instruction_branch_t_rule:
        pegtl::seq<
            str_branch,
            seps,
            t_rule,
            seps,
            label_rule,
            seps,
            label_rule
        > {};

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

    struct End_Instructions_rule:
        pegtl::sor<
            pegtl::seq< pegtl::at<Instruction_branch_t_rule>         , Instruction_branch_t_rule          >,
            pegtl::seq< pegtl::at<Instruction_branch_rule>           , Instruction_branch_rule            >,
            pegtl::seq< pegtl::at<Instruction_return_t_rule>        , Instruction_return_t_rule         >,
            pegtl::seq< pegtl::at<Instruction_return_rule>          , Instruction_return_rule           >
        > {};


    struct Instruction_label_rule:
        pegtl::seq<
            label_rule
        > {};

    /*
    * function/grammar rules
    */
    struct define_function_rule:
        pegtl::seq<
            seps,
            str_define,
            seps,
            function_type_rule,
            seps,
            function_name_rule,
            seps,
            pegtl::one<'('>,
            seps,
            pegtl::star<function_arguments_rule>,
            seps,
            pegtl::one<')'>
        > {};

    struct basic_block_rule:
        pegtl::seq<
            Instruction_label_rule,
            seps,
            Instructions_rule,
            seps,
            End_Instructions_rule
        > {};

    struct Function_rule:
        pegtl::seq<
            define_function_rule,
            seps,
            pegtl::one<'{'>,
            seps,
            pegtl::plus<
                pegtl::seq<
                    seps,
                    basic_block_rule,
                    seps
                >
            >,
            seps,
            pegtl::one<'}'>
        > {};


    struct grammar:
        pegtl::must<
            pegtl::plus<
                Function_rule
            > 
        > {};

    /*
    * Actions attached to grammar rules.
    */

    /*
    * Item actions
    */
    template< typename Rule >
    struct action : pegtl::nothing< Rule >{};

    template<> struct action < function_name_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::string input_string = in.string();
            Item* fn = new FunctionName(input_string);
            parsed_items.push_back(fn);
        }
    };


    template<> struct action < function_type_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::string input_string = in.string();
            Item* fn = new FunctionType(input_string);
            parsed_items.push_back(fn);
        }
    };

    template<> struct action < type_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::string input_string = in.string();
            Item* fn = new Type(input_string);
            parsed_items.push_back(fn);
        }
    };

    template<> struct action < variable_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::string input_string = in.string();
            Item* var = new Variable(input_string);
            parsed_items.push_back(var);
            
        }
    };

    template<> struct action < label_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::string input_string = in.string();
            Item* label = new InstructionLabel(input_string);
            parsed_items.push_back(label);
            
        }
    };


    template<> struct action < number_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            
            std::string input_string = in.string();
            Item* num = new InstructionNumber(input_string);
            parsed_items.push_back(num);
            
        }
    };

    template<> struct action < op_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::string input_string = in.string();
            Item* op = new Op(input_string);
            parsed_items.push_back(op);
        }
    };

    template<> struct action < cmp_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::string input_string = in.string();
            Item* cmp = new CompareOp(input_string);
            parsed_items.push_back(cmp);
        }
    };


    

    
    template<> struct action < define_function_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto newF = new Function();
            while (parsed_items.size() > 0){
                auto popped_item = parsed_items.back();
                parsed_items.pop_back();
                if (popped_item->get_name() == "FunctionName"){
                    newF->name = popped_item->to_string();
                }
                else if (popped_item->get_name() == "FunctionType"){
                    newF->return_type = popped_item->to_string();
                    if(popped_item->to_string() != "void") parsed_items.pop_back();
                    break;
                }
                else if (popped_item->get_name() == "Type"){
                    newF->types.push_back(popped_item);
                }
                else {
                    newF->vars.push_back(popped_item);
                }
            }
            std::reverse(newF->vars.begin(), newF->vars.end());
            std::reverse(newF->types.begin(), newF->types.end());
            p.functions.push_back(newF);
            
        }
    };

    template<> struct action < Instruction_label_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){


            auto currentF = p.functions.back();
            auto label = parsed_items.back();
            parsed_items.pop_back();
            auto newBBlock = new BasicBlock();
            newBBlock->label = label;


            currentF->basic_blocks.push_back(newBBlock);

        }
    };


    template<> struct action < standard_library_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::string input_string = in.string();
            // std::cout << input_string << "\n";
            Item* stl = new StandardLibrary(input_string);
            parsed_items.push_back(stl);
        }
    };

    template<> struct action < instruction_bracket_rule > {
        template < typename Input >
        static void apply( const Input & in, Program & p){
            Item* index = parsed_items.back();
            parsed_items.pop_back();
            Item* instructionBracket = new Instruction_bracket(index);
            parsed_items.push_back(instructionBracket);
        }
    };


    /*
    * Instruction actions
    */
    template<> struct action < Instruction_declaration_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
            auto currentB = currentF->basic_blocks.back();

            Item*  var = parsed_items.back();
            parsed_items.pop_back();

            Item* type = parsed_items.back();
            parsed_items.pop_back();

            auto i = new Instruction_declaration(type,var);

            currentB->instructions.push_back(i);
        }
    };


    template<> struct action < Instruction_cmp_assignment_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
            auto currentB = currentF->basic_blocks.back();

            auto t2 = parsed_items.back();
            parsed_items.pop_back();
            auto op = parsed_items.back();
            parsed_items.pop_back();
            auto t1 = parsed_items.back();
            parsed_items.pop_back();
            auto var = parsed_items.back();
            parsed_items.pop_back();

            /* 
            * Create the instruction.
            */ 
            auto i = new Instruction_cmp(var, t1, op, t2); 
            /* 
            * Add the just-created instruction to the current function.
            */ 
            currentB->instructions.push_back(i);
        }
    };


    template<> struct action < Instruction_op_assignment_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
            auto currentB = currentF->basic_blocks.back();

            auto t2 = parsed_items.back();
            parsed_items.pop_back();
            auto op = parsed_items.back();
            parsed_items.pop_back();
            auto t1 = parsed_items.back();
            parsed_items.pop_back();
            auto var = parsed_items.back();
            parsed_items.pop_back();

            /* 
            * Create the instruction.
            */ 
            auto i = new Instruction_op(var, t1, op, t2); 

            /* 
            * Add the just-created instruction to the current function.
            */ 
            currentB->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_assignment_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
            auto currentB = currentF->basic_blocks.back();

            auto s = parsed_items.back();
            parsed_items.pop_back();
            auto var = parsed_items.back();
            parsed_items.pop_back();

            /* 
            * Create the instruction.
            */ 
            auto i = new Instruction_assignment(var, s); 

            /* 
            * Add the just-created instruction to the current function.
            */ 
            currentB->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_load_assignment_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){

            auto currentF = p.functions.back();
            auto currentB = currentF->basic_blocks.back();

            std::vector<Item*> indices;
            while (parsed_items.size() > 0 && (parsed_items.back()->get_name() == "Instruction_bracket")){
                Instruction_bracket* index = (Instruction_bracket*) parsed_items.back();
                parsed_items.pop_back();
                indices.push_back(index->get_index());
            }

            Item* src = parsed_items.back();
            parsed_items.pop_back();
            Item* dst = parsed_items.back();
            parsed_items.pop_back();

            /* 
            * Create the instruction.
            */ 
            auto i = new Instruction_load(dst, src, indices); 

            /* 
            * Add the just-created instruction to the current function.
            */ 
            currentB->instructions.push_back(i);
        }
    };


    template<> struct action < Instruction_store_assignment_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
            auto currentB = currentF->basic_blocks.back();

            auto s = parsed_items.back();
            parsed_items.pop_back();

            std::vector<Item*> indices;
            while (parsed_items.size() > 0 && (parsed_items.back()->get_name() == "Instruction_bracket")){
                Instruction_bracket* index = (Instruction_bracket*) parsed_items.back();
                parsed_items.pop_back();
                indices.push_back(index->get_index());
            }

            auto var = parsed_items.back();
            parsed_items.pop_back();

            /* 
            * Create the instruction.
            */ 
            auto i = new Instruction_store(var, indices, s); 

            /* 
            * Add the just-created instruction to the current function.
            */ 
            currentB->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_array_length_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
            auto currentB = currentF->basic_blocks.back();

            Item* index = parsed_items.back();
            parsed_items.pop_back();

            Item* src = parsed_items.back();
            parsed_items.pop_back();

            Item* dst = parsed_items.back();
            parsed_items.pop_back();

            auto i = new Instruction_array_length(dst, src, index);

            currentB->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_tuple_length_rule >{
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
            auto currentB = currentF->basic_blocks.back();

            Item* src = parsed_items.back();
            parsed_items.pop_back();

            Item* dst = parsed_items.back();
            parsed_items.pop_back();

            auto i = new Instruction_tuple_length(dst,src);

            currentB->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_call_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
            auto currentB = currentF->basic_blocks.back();
            std::vector<Item*> args;
            while (parsed_items.size() > 1){ //in call rule, only last item is the callee
                auto popped_item = parsed_items.back();
                parsed_items.pop_back();
                args.push_back(popped_item);
            }
            auto callee = parsed_items.back();
            parsed_items.pop_back();
            std::reverse(args.begin(), args.end());
            /* 
            * Create the instruction.
            */ 
            auto i = new Instruction_call(callee, args); 
            /* 
            * Add the just-created instruction to the current function.
            */ 
            currentB->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_call_assignment_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
            auto currentB = currentF->basic_blocks.back();

            std::vector<Item*> args;
            while (parsed_items.size() > 2){ //in call assignment rule, last two items are var and callee. rest are args
                auto popped_item = parsed_items.back();
                parsed_items.pop_back();
                args.push_back(popped_item);
            }
            std::reverse(args.begin(), args.end());
            auto callee = parsed_items.back();
            parsed_items.pop_back();
            auto var = parsed_items.back();
            parsed_items.pop_back();

            /* 
            * Create the instruction.
            */ 
            auto i = new Instruction_call_assignment(var, callee, args); 

            /* 
            * Add the just-created instruction to the current function.
            */ 
            currentB->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_create_array_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
            auto currentB = currentF->basic_blocks.back();

            std::vector<Item*> args;
            while (parsed_items.size() > 1){ 
                auto popped_item = parsed_items.back();
                parsed_items.pop_back();
                args.push_back(popped_item);
            }

            Item* dst = parsed_items.back();
            parsed_items.pop_back();

            auto i = new Instruction_create_array(dst,args);

            currentB->instructions.push_back(i);
        }
    };


    template<> struct action < Instruction_create_tuple_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
            auto currentB = currentF->basic_blocks.back();

            Item* length = parsed_items.back();
            parsed_items.pop_back();

            Item* dst = parsed_items.back();
            parsed_items.pop_back();

            auto i = new Instruction_create_tuple(dst,length);

            currentB->instructions.push_back(i);
        }
    };


    //end instructions

    template<> struct action < Instruction_branch_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
            auto currentB = currentF->basic_blocks.back();

            auto label = parsed_items.back();
            parsed_items.pop_back();

            /* 
            * Create the instruction.
            */ 
            auto i = new Instruction_branch(label); 

            /* 
            * Add the just-created instruction to the current function.
            */ 
            currentB->end = i;
        }
    };

    template<> struct action < Instruction_branch_t_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
            auto currentB = currentF->basic_blocks.back();

            auto label2 = parsed_items.back();
            parsed_items.pop_back();
            auto label1 = parsed_items.back();
            parsed_items.pop_back();
            auto t = parsed_items.back();
            parsed_items.pop_back();

            /* 
            * Create the instruction.
            */ 
            auto i = new Instruction_branch_t(t, label1, label2); 

            /* 
            * Add the just-created instruction to the current function.
            */ 
            currentB->end = i;
        }
    };

    template<> struct action < Instruction_return_t_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){

        /* 
        * Fetch the current function.
        */ 
        auto currentF = p.functions.back();
        auto currentB = currentF->basic_blocks.back();

        /*
        * Fetch the tokens parsed.
        */
        
        auto t = parsed_items.back();
        parsed_items.pop_back();
        
        /* 
        * Create the instruction.
        */ 
        auto i = new Instruction_return_t(t);
        /* 
        * Add the just-created instruction to the current function.
        */ 
        currentB->end = i;
        }
    };

    template<> struct action < Instruction_return_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){

        /* 
        * Fetch the current function.
        */ 
        auto currentF = p.functions.back();
        auto currentB = currentF->basic_blocks.back();

        /*
        * Fetch the tokens parsed.
        */

        /* 
        * Create the instruction.
        */ 
        auto i = new Instruction_return();
        /* 
        * Add the just-created instruction to the current function.
        */ 
        currentB->end = i;
        }
    };



    /*
    * functions for parsing file
    */
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