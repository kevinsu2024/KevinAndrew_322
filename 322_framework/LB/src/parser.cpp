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
    * Scope stack
    * Logic: The back of scope_stack is the "most nested" scope. Whenever we see an open brace '{', we add a new scope at the 
    * back of the stack. When we see a close brace '}', we add this scope into the parent scope's vector of instructions, as scope
    * is a subclass of Instructions
    * When there are no scopes left in the scope stack, we know we have reached the end of a function. We then finally add this
    * scope into currentF->scope. See actions for str_open_brace and str_close_brace
    */
    std::vector<Scope*> scope_stack;
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
    struct str_print : TAOCPP_PEGTL_STRING( "print" ) {};
    struct str_input : TAOCPP_PEGTL_STRING( "input" ) {};
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
    struct str_open_brace : TAOCPP_PEGTL_STRING("{") {};
    struct str_close_brace : TAOCPP_PEGTL_STRING("}") {};

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

    // struct function_type_rule:
    //     pegtl::sor<
    //         str_void,
    //         type_rule
    //     > {};
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
            pegtl::seq<pegtl::at<cmp_rule>, cmp_rule>,
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

    struct standard_library_rule:
        pegtl::sor<
            str_print,
            str_input
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

    struct open_scope_rule:
        pegtl::seq<
            seps,
            pegtl::one<'{'>,
            seps
        > {};
    
    struct close_scope_rule:
        pegtl::seq<
            seps,
            pegtl::one<'}'>,
            seps
        > {};

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

    struct Instruction_return_rule:
        pegtl::seq<
            str_return
        > {};
    
    struct Instruction_continue_rule:
        pegtl::seq<
            str_continue
        > {};
    
    struct Instruction_break_rule:
        pegtl::seq<
            str_break
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

    struct Instruction_rule:
        pegtl::seq<
            pegtl::sor<
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
                pegtl::seq< pegtl::at<Instruction_op_assignment_rule>   , Instruction_op_assignment_rule    >,
                pegtl::seq< pegtl::at<Instruction_assignment_rule>      , Instruction_assignment_rule       >
            >
        > {};

    struct scope_rule:
        pegtl::star<
            pegtl::seq<
                pegtl::star<open_scope_rule>,
                pegtl::star<Instruction_rule>,
                pegtl::star<close_scope_rule>
            >
        > {};
    

    

    /*
    * function/grammar rules
    */
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

    struct Function_rule:
        pegtl::seq<
            define_function_rule,
            seps,
            scope_rule
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
            std::cerr << "in name rule\n" ;
            
            std::string input_string = in.string();
            std::cerr << input_string << "\n";
            if(p.functions.size() > 0 && input_string.size() > p.functions.back()->longest_name.size()) p.functions.back()->longest_name = input_string;
            Item* n = new Name(input_string);
            parsed_items.push_back(n);
        }
    };


    // template<> struct action < function_type_rule > {
    //     template< typename Input >
    //     static void apply( const Input & in, Program & p){
    //         std::string input_string = in.string();
    //         Item* fn = new FunctionType(input_string);
    //         parsed_items.push_back(fn);
    //     }
    // };

    template<> struct action < type_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "in type rule\n" ;
            std::string input_string = in.string();
            Item* fn = new Type(input_string);
            parsed_items.push_back(fn);
        }
    };

    template<> struct action < label_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            // if (parsed_items.size() > 0) parsed_items.pop_back();
            std::cerr << "in label rule\n" ;
            std::string input_string = in.string();
            if(p.functions.size() > 0 && input_string.size() > p.functions.back()->longest_label.size()) p.functions.back()->longest_label = input_string;
            Item* label = new InstructionLabel(input_string);
            parsed_items.push_back(label);
            
        }
    };

    template<> struct action < number_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "in number rule\n" ;
            std::string input_string = in.string();
            Item* num = new InstructionNumber(input_string);
            parsed_items.push_back(num);
            
        }
    };

    template<> struct action < op_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "in op rule\n" ;
            std::string input_string = in.string();
            Item* op = new Op(input_string);
            parsed_items.push_back(op);
        }
    };

    template<> struct action < cmp_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "in cmp rule\n" ;
            std::string input_string = in.string();
            Item* cmp = new Cmp(input_string);
            parsed_items.push_back(cmp);
        }
    };
    
    
    template<> struct action < define_function_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            // if we have function before, add longest name and label to it.
            std::cerr << "\ndefining new function " << "\n";
            std::cerr << in.string() << "\n";
            std::cerr << parsed_items.size() << "\n";
            auto newF = new Function();
            
            while (parsed_items.size() > 0){
                auto popped_item = parsed_items.back();
                std::cerr << "popped item is " << popped_item->get_name() << " " << popped_item->to_string() << "\n";
                parsed_items.pop_back();
                if (popped_item->get_name() == "Type"){
                    if (parsed_items.size() == 0){
                        newF->return_type = popped_item->to_string();
                    } else {
                        newF->types.push_back(popped_item);
                        if(popped_item->to_string() == "tuple"){
                            newF->tuple_names.insert(newF->vars.back()->to_string());
                        }
                    }   
                }
                else {
                    if (parsed_items.size() == 1){
                        newF->name = popped_item->to_string();
                    } else {
                        newF->vars.push_back(popped_item);
                        newF->var_names.insert(popped_item->to_string());
                    }
                }
            }
            std::cerr << "exited while loop\n";
            std::reverse(newF->vars.begin(), newF->vars.end());
            std::reverse(newF->types.begin(), newF->types.end());
            p.functions.push_back(newF);
            std::cerr << "pushed back func\n";
            
        }
    };

    template<> struct action < Instruction_label_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "in instruction label rule\n" ;
            auto currentF = p.functions.back();
            auto scope = scope_stack.back();

            /*
            * Fetch the tokens parsed.
            */
            auto label = parsed_items.back();
            parsed_items.pop_back();
            parsed_items.pop_back();
            /* 
            * Create the instruction.
            */ 
            auto i = new Instruction_label(label);
            /* 
            * Add the just-created instruction to the current function.
            */ 
            scope->instructions.push_back(i);

        }
    };


    template<> struct action < standard_library_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "in standard library rule\n" ;
            std::string input_string = in.string();
            // std::cout << input_string << "\n";
            Item* stl = new StandardLibrary(input_string);
            parsed_items.push_back(stl);
        }
    };

    template<> struct action < instruction_bracket_rule > {
        template < typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "in bracket rule\n" ;
            Item* index = parsed_items.back();
            parsed_items.pop_back();
            Item* instructionBracket = new Instruction_bracket(index);
            parsed_items.push_back(instructionBracket);
        }
    };

    template<> struct action < open_scope_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){

        /* 
        * Fetch the current function.
        */ 

        /*
        * Fetch the tokens parsed.
        */

        /* 
        * Create the instruction.
        */ 
        std::cerr << "making new scope\n";
        auto s = new Scope();
        scope_stack.push_back(s);
        /* 
        * Add the just-created instruction to the current function.
        */ 
        }
    };

    template<> struct action < close_scope_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){

        /* 
        * Fetch the current function.
        */ 

        /*
        * Fetch the tokens parsed.
        */

        /* 
        * Create the instruction.
        */ 
        std::cerr << "closing current child scope\n";
        auto s = scope_stack.back();
        std::cerr << "child instructions are: \n";
        for (auto ins : s->instructions){
            std::cerr << ins->to_string() << "\n";
        }
        scope_stack.pop_back();
        if (scope_stack.size() > 0){
            auto prev_s = scope_stack.back();
            prev_s->instructions.push_back(s);
        } else {
            auto currentF = p.functions.back();
            currentF->scope = *s;
        }
        
        /* 
        * Add the just-created instruction to the current function.
        */ 
        }
    };

    /*
    * Instruction actions
    */
    template<> struct action < Instruction_declaration_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "In declaration rule\n";
            auto currentF = p.functions.back();
            auto scope = scope_stack.back();
            std::vector<Item*> vars;
            while (parsed_items.size() > 1){
                Item* var = parsed_items.back();
                parsed_items.pop_back();
                p.functions.back()->var_names.insert(var->to_string());
                vars.push_back(var);
            }
            
            Item* type = parsed_items.back();
            parsed_items.pop_back();

            auto i = new Instruction_declaration(type,vars);

            scope->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_op_assignment_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "In op assign rule\n";
            auto currentF = p.functions.back();
            auto scope = scope_stack.back();

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
            scope->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_assignment_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "In assign rule\n";
            auto currentF = p.functions.back();
            auto scope = scope_stack.back();

            auto s = parsed_items.back();
            parsed_items.pop_back();
            if(s->get_name() == "Name" && currentF->var_names.find(s->to_string()) == currentF->var_names.end()){
                s = new FunctionName(s->to_string());
            }
            auto var = parsed_items.back();
            parsed_items.pop_back();

            /* 
            * Create the instruction.
            */ 
            auto i = new Instruction_assignment(var, s); 

            /* 
            * Add the just-created instruction to the current function.
            */ 
            scope->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_load_assignment_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "In load assign rule\n";

            auto currentF = p.functions.back();
            auto scope = scope_stack.back();

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
            scope->instructions.push_back(i);
        }
    };


    template<> struct action < Instruction_store_assignment_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "In store assign rule\n";
            std::cerr << "\n printing aprsed items in store instr " << in.string() << "\n";
            for(auto it : parsed_items){
                std::cerr << it->to_string() << " ";
            }
            std::cerr <<"\n";
            auto currentF = p.functions.back();
            auto scope = scope_stack.back();

            auto s = parsed_items.back();
            parsed_items.pop_back();
            if(s->get_name() == "InstructionLabel") parsed_items.pop_back();

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
            scope->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_array_length_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "In arr len rule\n";
            auto currentF = p.functions.back();
            auto scope = scope_stack.back();

            Item* index = parsed_items.back();
            parsed_items.pop_back();

            Item* src = parsed_items.back();
            parsed_items.pop_back();

            Item* dst = parsed_items.back();
            parsed_items.pop_back();

            auto i = new Instruction_array_length(dst, src, index);

            scope->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_call_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "In call rule\n";
            auto currentF = p.functions.back();
            auto scope = scope_stack.back();
            
            std::vector<Item*> args;
            while (parsed_items.size() > 1){ //in call rule, only last item is the callee
                auto popped_item = parsed_items.back();
                parsed_items.pop_back();
                args.push_back(popped_item);
            }
            auto callee = parsed_items.back();
            if(currentF->var_names.find(callee->to_string()) == currentF->var_names.end()){
                callee= new FunctionName(callee->to_string());
            } 
            parsed_items.pop_back();
            std::reverse(args.begin(), args.end());
            /* 
            * Create the instruction.
            */ 
            auto i = new Instruction_call(callee, args); 
            /* 
            * Add the just-created instruction to the current function.
            */ 
            scope->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_call_assignment_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "In call assign rule\n";
            auto currentF = p.functions.back();
            auto scope = scope_stack.back();
            std::vector<Item*> args;
            while (parsed_items.size() > 2){ //in call assignment rule, last two items are var and callee. rest are args
                auto popped_item = parsed_items.back();
                parsed_items.pop_back();
                args.push_back(popped_item);
            }
            std::reverse(args.begin(), args.end());
            auto callee = parsed_items.back();
            if(currentF->var_names.find(callee->to_string()) == currentF->var_names.end()){
                callee= new FunctionName(callee->to_string());
            } 
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
            scope->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_create_array_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "In create array rule\n";
            auto currentF = p.functions.back();
            auto scope = scope_stack.back();
            

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

            scope->instructions.push_back(i);
        }
    };


    template<> struct action < Instruction_create_tuple_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "In create tuple rule\n";
            auto currentF = p.functions.back();
            auto scope = scope_stack.back();
            

            Item* length = parsed_items.back();
            parsed_items.pop_back();

            Item* dst = parsed_items.back();
            parsed_items.pop_back();

            auto i = new Instruction_create_tuple(dst,length);

            scope->instructions.push_back(i);
        }
    };


    //end instructions

    template<> struct action < Instruction_goto_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "In goto rule\n";
            auto currentF = p.functions.back();
            auto scope = scope_stack.back();
            
            auto label = parsed_items.back();
            parsed_items.pop_back();
            parsed_items.pop_back();

            /* 
            * Create the instruction.
            */ 
            auto i = new Instruction_goto(label); 

            /* 
            * Add the just-created instruction to the current function.
            */ 
            scope->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_if_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "In if rule\n";
            auto currentF = p.functions.back();
            auto scope = scope_stack.back();
            std::cerr << "before pop: parsed item size is: " << parsed_items.size() << "\n";
            for (auto it : parsed_items){
                std::cerr << it->to_string() << "\n";
            }
            auto label2 = parsed_items.back();
            parsed_items.pop_back();
            parsed_items.pop_back();
            auto label1 = parsed_items.back();
            parsed_items.pop_back();
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
            scope->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_while_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "In while rule\n";
            auto currentF = p.functions.back();
            auto scope = scope_stack.back();
            auto label2 = parsed_items.back();
            parsed_items.pop_back();
            parsed_items.pop_back();
            auto label1 = parsed_items.back();
            parsed_items.pop_back();
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
            scope->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_return_t_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            

        /* 
        * Fetch the current function.
        */ 
       std::cerr << "in return t rule\n";
        auto currentF = p.functions.back();
        auto scope = scope_stack.back();
        

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
        scope->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_return_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){

        /* 
        * Fetch the current function.
        */ 
        std::cerr << "in return rule\n";
        auto currentF = p.functions.back();
        auto scope = scope_stack.back();
        
        std::cerr << parsed_items.size() << "\n";
        for (auto i: parsed_items){
            std::cerr << i->to_string() << "\n";
        }
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
        scope->instructions.push_back(i);
        }
    };

    template<> struct action < Instruction_continue_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "In continue rule\n";

        /* 
        * Fetch the current function.
        */ 
        auto currentF = p.functions.back();
        auto scope = scope_stack.back();
        

        /*
        * Fetch the tokens parsed.
        */

        /* 
        * Create the instruction.
        */ 
        auto i = new Instruction_continue();
        /* 
        * Add the just-created instruction to the current function.
        */ 
        scope->instructions.push_back(i);
        }
    };
    template<> struct action < Instruction_break_rule > {
        template< typename Input >
        static void apply( const Input & in, Program & p){
            std::cerr << "In break rule\n";

        /* 
        * Fetch the current function.
        */ 
        auto currentF = p.functions.back();
        auto scope = scope_stack.back();
        

        /*
        * Fetch the tokens parsed.
        */

        /* 
        * Create the instruction.
        */ 
        auto i = new Instruction_break();
        /* 
        * Add the just-created instruction to the current function.
        */ 
        scope->instructions.push_back(i);
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