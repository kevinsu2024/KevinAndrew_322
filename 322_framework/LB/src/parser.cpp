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
#include <parser.h>
#include <LB.h>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

using namespace pegtl;

namespace LB {
    /*
    * Tokens parsed
    */
    std::vector<Item *> parsed_items;
    /*
    * Grammar rules for IR
    */
   struct name_rule:
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
    struct str_return : TAOCPP_PEGTL_STRING( "return" ) {};
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
    struct str_void : TAOCPP_PEGTL_STRING( "void" ) {};
    struct str_code : TAOCPP_PEGTL_STRING( "code" ) {};
    struct str_tuple : TAOCPP_PEGTL_STRING( "tuple" ) {};
    struct str_int : TAOCPP_PEGTL_STRING( "int64" ) {};
    struct str_type_bracket : TAOCPP_PEGTL_STRING( "[]" ) {};
    struct str_length: TAOCPP_PEGTL_STRING("length") {};
    struct str_new: TAOCPP_PEGTL_STRING("new") {};
    struct str_array: TAOCPP_PEGTL_STRING("Array") {};
    struct str_Tuple: TAOCPP_PEGTL_STRING("Tuple") {};
    struct str_if : TAOCPP_PEGTL_STRING("if") {};
    struct str_while : TAOCPP_PEGTL_STRING("while") {};
    struct str_goto : TAOCPP_PEGTL_STRING("goto") {};
    struct str_continue : TAOCPP_PEGTL_STRING("continue") {};
    struct str_break : TAOCPP_PEGTL_STRING("break") {};


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
    * name-related rules
    */

    struct label_rule:
        pegtl::seq<
            pegtl::one<':'>,
            name_rule
        > {};
    
    struct names_rule:
        pegtl::seq<
            name_rule,
            seps,
            pegtl::star<
                pegtl::one<','>,
                seps,
                name_rule
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
            >,
            str_void
        > {};



    struct cmp_rule:
        pegtl::sor<
            pegtl::seq<pegtl::at<str_geq>,str_geq>,
            pegtl::seq<pegtl::at<str_leq>,str_leq>,
            pegtl::seq<pegtl::at<str_lt>,str_lt>,
            pegtl::seq<pegtl::at<str_gt>,str_gt>,
            pegtl::seq<pegtl::at<str_eq>,str_eq>
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

   

    struct t_rule:
        pegtl::sor<
            name_rule,
            number_rule
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
            name_rule
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
            names_rule
        > {};

    struct Instruction_assignment_rule:
        pegtl::seq<
            name_rule,
            seps,
            str_arrow,
            seps,
            t_rule
        > {};

    struct Instruction_op_assignment_rule:
        pegtl::seq<
            name_rule,
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
            name_rule,
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
            name_rule,
            seps,
            str_arrow,
            seps,
            name_rule,
            seps,
            pegtl::plus<
                instruction_bracket_rule
            >
        > {};

    struct Instruction_store_assignment_rule:
        pegtl::seq<
            name_rule,
            seps,
            pegtl::plus<
                instruction_bracket_rule
            >,
            seps,
            str_arrow,
            seps,
            t_rule
        > {};

    struct Instruction_array_length_rule:
        pegtl::seq<
            name_rule,
            seps,
            str_arrow,
            seps,
            str_length,
            seps,
            name_rule,
            seps,
            t_rule
        > {};

    struct Instruction_call_rule:
        pegtl::seq<
            seps,
            name_rule,
            seps,
            pegtl::one<'('>,
            seps,
            pegtl::star<arguments_rule>,
            seps,
            pegtl::one<')'>
        > {};

    struct Instruction_call_assignment_rule:
        pegtl::seq<
            name_rule,
            seps,
            str_arrow,
            seps,
            name_rule,
            seps,
            pegtl::one<'('>,
            seps,
            pegtl::star<arguments_rule>,
            seps,
            pegtl::one<')'>
        > {};

    struct Instruction_create_array_rule:
        pegtl::seq<
            name_rule,
            seps,
            str_arrow,
            seps,
            str_new,
            seps,
            str_array,
            seps,
            pegtl::one<'('>,
            seps,
            pegtl::star<arguments_rule>,
            seps,
            pegtl::one<')'>
        > {};

    struct Instruction_create_tuple_rule:
        pegtl::seq<
            name_rule,
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

    struct Instruction_goto_rule:
        pegtl::seq<
            str_goto,
            seps,
            label_rule
        > {};
    
    struct Instruction_if_rule:
        pegtl::seq<
            str_if,
            seps,
            pegtl::one<'('>,
            seps,
            t_rule,
            seps,
            cmp_rule,
            seps,
            t_rule,
            seps,
            pegtl::one<')'>,
            seps,
            label_rule,
            seps,
            label_rule
        > {};

    struct Instruction_while_rule:
        pegtl::seq<
            str_while,
            seps,
            pegtl::one<'('>,
            seps,
            t_rule,
            seps,
            cmp_rule,
            seps,
            t_rule,
            seps,
            pegtl::one<')'>,
            seps,
            label_rule,
            seps,
            label_rule
        > {};

    
    struct Instruction_continue_rule:
        pegtl::seq<
            str_continue
        > {};
    
    struct Instruction_break_rule:
        pegtl::seq<
            str_break
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


    struct Instruction_label_rule:
        pegtl::seq<
            label_rule
        > {};

    struct Instruction_open_brace_rule:
        pegtl::seq<
            pegtl::one<'{'>
        > {};
    
    struct Instruction_close_brace_rule:
        pegtl::seq<
            pegtl::one<'}'>
        > {};

    struct define_function_rule:
        pegtl::seq<
            seps,
            type_rule,
            seps,
            name_rule,
            seps,
            pegtl::one<'('>,
            seps,
            pegtl::star<function_arguments_rule>,
            seps,
            pegtl::one<')'>
        > {};

    struct Instruction_rule:
        pegtl::seq<
            pegtl::sor<
                pegtl::seq< pegtl::at<define_function_rule>    , define_function_rule     >,
                pegtl::seq< pegtl::at<Instruction_op_assignment_rule>   , Instruction_op_assignment_rule    >,
                pegtl::seq< pegtl::at<Instruction_cmp_assignment_rule>  , Instruction_cmp_assignment_rule   >,
                pegtl::seq< pegtl::at<Instruction_open_brace_rule>    , Instruction_open_brace_rule     >,
                pegtl::seq< pegtl::at<Instruction_close_brace_rule>    , Instruction_close_brace_rule     >,
                pegtl::seq< pegtl::at<Instruction_create_array_rule>    , Instruction_create_array_rule     >,
                pegtl::seq< pegtl::at<Instruction_create_tuple_rule>    , Instruction_create_tuple_rule     >,
                pegtl::seq< pegtl::at<Instruction_call_assignment_rule> , Instruction_call_assignment_rule  >,
                pegtl::seq< pegtl::at<Instruction_call_rule>            , Instruction_call_rule             >,
                pegtl::seq< pegtl::at<Instruction_array_length_rule>    , Instruction_array_length_rule     >,
                pegtl::seq< pegtl::at<Instruction_if_rule>              , Instruction_if_rule               >,
                pegtl::seq< pegtl::at<Instruction_while_rule>           , Instruction_while_rule            >,
                pegtl::seq< pegtl::at<Instruction_declaration_rule>     , Instruction_declaration_rule      >,
                pegtl::seq< pegtl::at<Instruction_goto_rule>            , Instruction_goto_rule             >,
                pegtl::seq< pegtl::at<Instruction_return_t_rule>        , Instruction_return_t_rule         >,
                pegtl::seq< pegtl::at<Instruction_return_rule>          , Instruction_return_rule           >,
                pegtl::seq< pegtl::at<Instruction_continue_rule>        , Instruction_continue_rule         >,
                pegtl::seq< pegtl::at<Instruction_break_rule>           , Instruction_break_rule            >,
                pegtl::seq< pegtl::at<Instruction_label_rule>           , Instruction_label_rule            >,
                pegtl::seq< pegtl::at<Instruction_load_assignment_rule> , Instruction_load_assignment_rule  >,
                pegtl::seq< pegtl::at<Instruction_store_assignment_rule>, Instruction_store_assignment_rule >,
                pegtl::seq< pegtl::at<Instruction_cmp_assignment_rule>  , Instruction_cmp_assignment_rule   >,
                pegtl::seq< pegtl::at<Instruction_assignment_rule>      , Instruction_assignment_rule       >
            >
        > {};

    struct Instructions_rule:
        pegtl::star<
            pegtl::seq<
                seps,
                Instruction_rule,
                seps
            >
        > {};
    

    /*
    * function/grammar rules
    */
    

    struct Function_rule:
        pegtl::seq<
            seps,
            define_function_rule,
            seps,
            Instructions_rule,
            seps
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

    template<> struct action < name_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::string input_string = in.string();
            if(p.functions.size() > 0 && input_string.size() > p.functions.back()->longest_name.size()) p.functions.back()->longest_name = input_string;
            Item* n = new Name(input_string);
            parsed_items.push_back(n);
        }
    };


    template<> struct action < label_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            Item* name = parsed_items.back();
            parsed_items.pop_back();
            std::string input_string = ":" + name->to_string();
            if(p.functions.size() > 0 && input_string.size() > p.functions.back()->longest_label.size()) p.functions.back()->longest_label = input_string;
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

    template<> struct action < type_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::string input_string = in.string();
            Item* fn = new Type(input_string);
            parsed_items.push_back(fn);
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

    template<> struct action < instruction_bracket_rule > {
        template < typename Input >
        static void apply( const Input & in, Program & p){
            Item* index = parsed_items.back();
            parsed_items.pop_back();
            Item* instructionBracket = new Instruction_bracket(index);
            parsed_items.push_back(instructionBracket);
        }
    };

    template<> struct action < cmp_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::string input_string = in.string();
            Item* cmp = new Cmp(input_string);
            parsed_items.push_back(cmp);
        }
    };


    template<> struct action < define_function_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            // if we have function before, add longest name and label to it.
            auto newF = new Function();
            
            while (parsed_items.size() > 0){
                auto popped_item = parsed_items.back();
                parsed_items.pop_back();
                if (popped_item->get_name() == "Type"){
                    if (parsed_items.size() == 0){
                        newF->return_type = popped_item->to_string();
                    } else {
                        newF->types.push_back(popped_item);
                    }   
                }
                else {
                    if (parsed_items.size() == 1){
                        newF->name = popped_item->to_string();
                    } else {
                        newF->vars.push_back(popped_item);
                    }
                }
            }
            std::reverse(newF->vars.begin(), newF->vars.end());
            std::reverse(newF->types.begin(), newF->types.end());
            p.functions.push_back(newF);
            
        }
    };


    /*
    Instruction actions
    */

    template<> struct action < Instruction_label_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();

            /*
            * Fetch the tokens parsed.
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


    template<> struct action < Instruction_declaration_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();

            std::vector<Item*> vars;
            while (parsed_items.size() > 1){
                Item* var = parsed_items.back();
                parsed_items.pop_back();
                vars.push_back(var);
            }

            Item* type = parsed_items.back();
            parsed_items.pop_back();

            auto i = new Instruction_declaration(type,vars);

            currentF->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_op_assignment_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();

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
            currentF->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_cmp_assignment_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();

            auto t2 = parsed_items.back();
            parsed_items.pop_back();
            auto cmp = parsed_items.back();
            parsed_items.pop_back();
            auto t1 = parsed_items.back();
            parsed_items.pop_back();
            auto var = parsed_items.back();
            parsed_items.pop_back();

            /* 
            * Create the instruction.
            */ 
            auto i = new Instruction_cmp(var, t1, cmp, t2); 

            /* 
            * Add the just-created instruction to the current function.
            */ 
            currentF->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_assignment_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();

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
            currentF->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_load_assignment_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){

            auto currentF = p.functions.back();

            std::vector<Item*> indices;
            while (parsed_items.size() > 0 && (parsed_items.back()->get_name() == "Instruction_bracket")){
                Instruction_bracket* index = (Instruction_bracket*) parsed_items.back();
                parsed_items.pop_back();
                indices.push_back(index->get_index());
            }
            std::reverse(indices.begin(), indices.end());
            Item* src = parsed_items.back();
            parsed_items.pop_back();
            Item* dst = parsed_items.back();
            parsed_items.pop_back();
            int64_t line_no = in.position().line;
            /* 
            * Create the instruction.
            */ 
            auto i = new Instruction_load(dst, src, indices, line_no); 

            /* 
            * Add the just-created instruction to the current function.
            */ 
            currentF->instructions.push_back(i);
        }
    };


    template<> struct action < Instruction_store_assignment_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();

            auto s = parsed_items.back();
            parsed_items.pop_back();

            std::vector<Item*> indices;
            while (parsed_items.size() > 0 && (parsed_items.back()->get_name() == "Instruction_bracket")){
                Instruction_bracket* index = (Instruction_bracket*) parsed_items.back();
                parsed_items.pop_back();
                indices.push_back(index->get_index());
            }
            std::reverse(indices.begin(), indices.end());
            auto var = parsed_items.back();
            parsed_items.pop_back();
            int64_t line_no = in.position().line;
            /* 
            * Create the instruction.
            */ 
            auto i = new Instruction_store(var, indices, s, line_no); 

            /* 
            * Add the just-created instruction to the current function.
            */ 
            currentF->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_array_length_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();

            Item* index = parsed_items.back();
            parsed_items.pop_back();

            Item* src = parsed_items.back();
            parsed_items.pop_back();

            Item* dst = parsed_items.back();
            parsed_items.pop_back();

            auto i = new Instruction_array_length(dst, src, index);

            currentF->instructions.push_back(i);
        }
    };


    template<> struct action < Instruction_call_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
            
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
            currentF->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_call_assignment_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
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
            currentF->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_create_array_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
            

            std::vector<Item*> args;
            while (parsed_items.size() > 1){ 
                auto popped_item = parsed_items.back();
                parsed_items.pop_back();
                args.push_back(popped_item);
            }
            std::reverse(args.begin(), args.end());
            Item* dst = parsed_items.back();
            parsed_items.pop_back();

            auto i = new Instruction_create_array(dst,args);

            currentF->instructions.push_back(i);
        }
    };


    template<> struct action < Instruction_create_tuple_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
            

            Item* length = parsed_items.back();
            parsed_items.pop_back();

            Item* dst = parsed_items.back();
            parsed_items.pop_back();

            auto i = new Instruction_create_tuple(dst,length);

            currentF->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_return_t_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){

        /* 
        * Fetch the current function.
        */ 
        auto currentF = p.functions.back();
        

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
        currentF->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_return_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){

        /* 
        * Fetch the current function.
        */ 
        auto currentF = p.functions.back();
        

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
        currentF->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_goto_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
            
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

    template<> struct action < Instruction_if_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
            auto label2 = parsed_items.back();
            parsed_items.pop_back();
            auto label1 = parsed_items.back();
            parsed_items.pop_back();
            auto t2 = parsed_items.back();
            parsed_items.pop_back();
            auto cmp = parsed_items.back();
            parsed_items.pop_back();
            auto t1 = parsed_items.back();
            parsed_items.pop_back();

            /* 
            * Create the instruction.
            */ 
            auto i = new Instruction_if(t1, cmp, t2, label1, label2); 

            /* 
            * Add the just-created instruction to the current function.
            */ 
            currentF->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_while_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            auto currentF = p.functions.back();
            auto label2 = parsed_items.back();
            parsed_items.pop_back();
            auto label1 = parsed_items.back();
            parsed_items.pop_back();
            auto t2 = parsed_items.back();
            parsed_items.pop_back();
            auto cmp = parsed_items.back();
            parsed_items.pop_back();
            auto t1 = parsed_items.back();
            parsed_items.pop_back();

            /* 
            * Create the instruction.
            */ 
            auto i = new Instruction_while(t1, cmp, t2, label1, label2); 

            /* 
            * Add the just-created instruction to the current function.
            */ 
            currentF->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_continue_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){

        /* 
        * Fetch the current function.
        */ 
        auto currentF = p.functions.back();
        std::string line_no = std::to_string(in.position().line);
        

        /*
        * Fetch the tokens parsed.
        */

        /* 
        * Create the instruction.
        */ 
        auto i = new Instruction_continue(line_no);
        /* 
        * Add the just-created instruction to the current function.
        */ 
        currentF->instructions.push_back(i);
        }
    };
    template<> struct action < Instruction_break_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){

        /* 
        * Fetch the current function.
        */ 
        auto currentF = p.functions.back();
        std::string line_no = std::to_string(in.position().line);
        

        /*
        * Fetch the tokens parsed.
        */

        /* 
        * Create the instruction.
        */ 
        auto i = new Instruction_break(line_no);
        /* 
        * Add the just-created instruction to the current function.
        */ 
        currentF->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_open_brace_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){

        /* 
        * Fetch the current function.
        */ 
        auto currentF = p.functions.back();
        

        /*
        * Fetch the tokens parsed.
        */

        /* 
        * Create the instruction.
        */ 
        auto i = new Instruction_open_brace();
        /* 
        * Add the just-created instruction to the current function.
        */ 
        currentF->instructions.push_back(i);
        }
    };


    template<> struct action < Instruction_close_brace_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){

        /* 
        * Fetch the current function.
        */ 
        auto currentF = p.functions.back();
        

        /*
        * Fetch the tokens parsed.
        */

        /* 
        * Create the instruction.
        */ 
        auto i = new Instruction_close_brace();
        /* 
        * Add the just-created instruction to the current function.
        */ 
        currentF->instructions.push_back(i);
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