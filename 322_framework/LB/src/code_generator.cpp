#include <code_generator.h>

namespace LB{
    std::vector<Instruction*>
    translate_declarations(std::vector<Instruction*> instrs){
        std::vector<Instruction*> new_instrs;
        for(Instruction* in : instrs){
            if(in->get_name() == "Instruction_declaration"){
                Instruction_declaration* instr = (Instruction_declaration*) in;
                std::vector<Item*> vars = instr->get_vars();
                Item* type = instr->get_var_type();
                for(Item* v : vars){
                    std::vector<Item*> var{v};
                    new_instrs.push_back(new Instruction_declaration(type, var));
                }
            }
            else new_instrs.push_back(in);
        }

        return new_instrs;
    }

    std::string
    get_mapping(std::unordered_map<std::string,std::string> new_map, std::vector<std::unordered_map<std::string, std::string>> maps, std::string var){
        if(new_map.find(var) != new_map.end()) return new_map[var];
        while(maps.size() > 0){
            auto map = maps.back();
            maps.pop_back();
            if(map.find(var) != map.end()) return map[var];
        }
        return "";
    }

    std::vector<Instruction*>
    recursive_scope(std::vector<Instruction*> instructions, std::vector<std::unordered_map<std::string, std::string>> maps){
        std::vector<Instruction*> new_instrs;
        std::unordered_map<std::string,std::string> new_map;
        std::cerr << "\n\nstarting scope instructions are \n";
        for(auto ele : instructions){
            std::cerr << ele->to_string() << "\n";
        }
        
        int64_t ind = 0;
        while(ind < instructions.size()){
            Instruction* in = instructions[ind];
            if(in->get_name() == "Instruction_declaration"){
                Instruction_declaration* instr = (Instruction_declaration*) in;
                Item* type = instr->get_var_type();
                std::string var = instr->get_vars()[0]->to_string();
                if(maps.size() > 0){
                    new_map[var] = var + "_" + std::to_string(maps.size());
                    var = var + "_" + std::to_string(maps.size());
                }
                std::vector<Item*> vars{new Name(var)};
                Instruction* new_instr = new Instruction_declaration(type, vars);
                new_instrs.push_back(new_instr);
            }

            else if (in->get_name() == "Instruction_assignment"){
                Instruction_assignment* instr = (Instruction_assignment*) in;
                Item* var = instr->get_var();
                Item* t = instr->get_t();
                std::string mapped_var = get_mapping(new_map, maps, var->to_string());
                if(mapped_var != "") var = new Name(mapped_var);
                if(t->get_name() == "Name"){
                    std::string mapped_t = get_mapping(new_map,maps, t->to_string());
                    if(mapped_t != "") t = new Name(mapped_t);
                }

                Instruction* new_instr = new Instruction_assignment(var, t);
                new_instrs.push_back(new_instr);
            }

            else if(in->get_name() == "Instruction_open_brace"){
                std::vector<Instruction*> inner;
                int64_t current = ind + 1;
                int64_t open = 1;
                while(open > 0){
                    if(instructions[current]->get_name() == "Instruction_open_brace") open += 1;
                    else if(instructions[current]->get_name() == "Instruction_close_brace") open -= 1;
                    if(open > 0) inner.push_back(instructions[current]);
                    current += 1;
                }
                std::cerr << "scope at this point is \n";
                for(auto instruction : inner){
                    std::cerr << instruction->to_string();
                }
                maps.push_back(new_map);

                std::vector<Instruction*> inner_instructions = recursive_scope(inner, maps);
                for(Instruction* instr : inner_instructions){
                    new_instrs.push_back(instr);
                }
                maps.pop_back();
                ind = current - 1;

            }

            else{
                new_instrs.push_back(in);
            }
            ind += 1;
        }
        return new_instrs;
    }

    std::vector<Instruction*>
    translate_scopes(std::vector<Instruction*> instructions){
        std::vector<std::unordered_map<std::string, std::string>> maps;
        std::vector<Instruction*> ins(instructions.begin() + 1, instructions.end() - 1);
        instructions = recursive_scope(ins, maps);
        return instructions;
    }

    void
    generate_code(Program p){
        std::ofstream outputFile;
        outputFile.open("prog.a");

        for(auto f : p.functions){
            std::cerr << "currently translating function: \n"<< f->to_string() << "\n";
            std::string longest_label = f->longest_label + "_";
            std::string longest_name = f->longest_name + "_";
            if(longest_label.size() == 0) longest_label = ":";


            std::vector<Instruction*> instructions = f->instructions;

            //first get rid of multiple declarations
            instructions = translate_declarations(instructions);

            f->instructions = instructions;
            std::cerr << "after translate_declrations new func is \n" << f->to_string() << "\n";


            //now getting rid of scopes
            instructions = translate_scopes(instructions);

            f->instructions = instructions;
            std::cerr << "after translate_scopes new func is \n" << f->to_string() << "\n";
        }



        outputFile.close();
    }
}