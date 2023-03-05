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
            else if (in->get_name() == "Instruction_op"){
                Instruction_op* instr = (Instruction_op*) in;
                Item* var = instr->get_var();
                Item* t1 = instr->get_t1();
                Item* op = instr->get_op();
                Item* t2 = instr->get_t2();
                std::string mapped_var = get_mapping(new_map, maps, var->to_string());
                if(mapped_var != "") var = new Name(mapped_var);
                if(t1->get_name() == "Name"){
                    std::string mapped_t = get_mapping(new_map,maps, t1->to_string());
                    if(mapped_t != "") t1 = new Name(mapped_t);
                }
                if(t2->get_name() == "Name"){
                    std::string mapped_t = get_mapping(new_map,maps, t2->to_string());
                    if(mapped_t != "") t2 = new Name(mapped_t);
                }

                Instruction* new_instr = new Instruction_op(var, t1, op, t2);
                new_instrs.push_back(new_instr);
            }

            else if (in->get_name() == "Instruction_if"){
                Instruction_if* instr = (Instruction_if*) in;
                Item* t1 = instr->get_t1();
                Item* cmp = instr->get_cmp();
                Item* t2 = instr->get_t2();
                Item* l1 = instr->get_label1();
                Item* l2 = instr->get_label2();
                if(t1->get_name() == "Name"){
                    std::string mapped_t = get_mapping(new_map,maps, t1->to_string());
                    if(mapped_t != "") t1 = new Name(mapped_t);
                }
                if(t2->get_name() == "Name"){
                    std::string mapped_t = get_mapping(new_map,maps, t2->to_string());
                    if(mapped_t != "") t2 = new Name(mapped_t);
                }

                Instruction* new_instr = new Instruction_if(t1, cmp, t2, l1,l2);
                new_instrs.push_back(new_instr);
            }


            else if (in->get_name() == "Instruction_return_t"){
                Instruction_return_t* instr = (Instruction_return_t*) in;
                Item* t1 = instr->get_t();
                if(t1->get_name() == "Name"){
                    std::string mapped_t = get_mapping(new_map,maps, t1->to_string());
                    if(mapped_t != "") t1 = new Name(mapped_t);
                }

                Instruction* new_instr = new Instruction_return_t(t1);
                new_instrs.push_back(new_instr);
            }

            else if (in->get_name() == "Instruction_while"){
                Instruction_while* instr = (Instruction_while*) in;
                Item* t1 = instr->get_t1();
                Item* cmp = instr->get_cmp();
                Item* t2 = instr->get_t2();
                Item* l1 = instr->get_label1();
                Item* l2 = instr->get_label2();
                if(t1->get_name() == "Name"){
                    std::string mapped_t = get_mapping(new_map,maps, t1->to_string());
                    if(mapped_t != "") t1 = new Name(mapped_t);
                }
                if(t2->get_name() == "Name"){
                    std::string mapped_t = get_mapping(new_map,maps, t2->to_string());
                    if(mapped_t != "") t2 = new Name(mapped_t);
                }

                Instruction* new_instr = new Instruction_while(t1, cmp, t2, l1,l2);
                new_instrs.push_back(new_instr);
            }

            else if (in->get_name() == "Instruction_load"){
                Instruction_load* instr = (Instruction_load*) in;
                Item* dst = instr->get_var_dst();
                Item* src = instr->get_var_src();
                std::vector<Item*> indices = instr->get_indices();
                int64_t ln = instr->get_line_no();


                std::string mapped_var = get_mapping(new_map, maps, dst->to_string());
                if(mapped_var != "") dst = new Name(mapped_var);

                if(src->get_name() == "Name"){
                    std::string mapped_t = get_mapping(new_map,maps, src->to_string());
                    if(mapped_t != "") src = new Name(mapped_t);
                }
                
                for(int j = 0; j < indices.size(); j++){
                    if(indices[j]->get_name() == "Name"){
                        std::string mapped_t = get_mapping(new_map,maps, indices[j]->to_string());
                        if(mapped_t != "") indices[j] = new Name(mapped_t);
                    }
                }

                Instruction* new_instr = new Instruction_load(dst, src, indices, ln);
                new_instrs.push_back(new_instr);
            }


            else if (in->get_name() == "Instruction_store"){
                Instruction_store* instr = (Instruction_store*) in;
                Item* var = instr->get_var();
                Item* s = instr->get_s();
                std::vector<Item*> indices = instr->get_indices();
                int64_t ln = instr->get_line_no();


                std::string mapped_var = get_mapping(new_map, maps, var->to_string());
                if(mapped_var != "") var = new Name(mapped_var);

                if(s->get_name() == "Name"){
                    std::string mapped_t = get_mapping(new_map,maps, s->to_string());
                    if(mapped_t != "") s = new Name(mapped_t);
                }
                
                for(int j = 0; j < indices.size(); j++){
                    if(indices[j]->get_name() == "Name"){
                        std::string mapped_t = get_mapping(new_map,maps, indices[j]->to_string());
                        if(mapped_t != "") indices[j] = new Name(mapped_t);
                    }
                }

                Instruction* new_instr = new Instruction_store(var, indices, s, ln);
                new_instrs.push_back(new_instr);
            }

            else if (in->get_name() == "Instruction_array_length"){
                Instruction_array_length* instr = (Instruction_array_length*) in;
                Item* dst = instr->get_dst_var();
                Item* src = instr->get_src_var();
                Item* index = instr->get_index();


                std::string mapped_var = get_mapping(new_map, maps, dst->to_string());
                if(mapped_var != "") dst = new Name(mapped_var);

                mapped_var = get_mapping(new_map, maps, src->to_string());
                if(mapped_var != "") src = new Name(mapped_var);

                if(index->get_name() == "Name"){
                    std::string mapped_t = get_mapping(new_map,maps, index->to_string());
                    if(mapped_t != "") index = new Name(mapped_t);
                }
                

                Instruction* new_instr = new Instruction_array_length(dst, src, index);
                new_instrs.push_back(new_instr);
            }

            else if (in->get_name() == "Instruction_call"){
                Instruction_call* instr = (Instruction_call*) in;
                Item* callee = instr->get_callee();
                std::vector<Item*> args = instr->get_args();

                std::string mapped_var = get_mapping(new_map, maps, callee->to_string());
                if(mapped_var != "") callee = new Name(mapped_var);
                
                for(int j = 0; j < args.size(); j++){
                    if(args[j]->get_name() == "Name"){
                        std::string mapped_t = get_mapping(new_map,maps, args[j]->to_string());
                        if(mapped_t != "") args[j] = new Name(mapped_t);
                    }
                }

                Instruction* new_instr = new Instruction_call(callee, args);
                new_instrs.push_back(new_instr);
            }

            else if (in->get_name() == "Instruction_call_assignment"){
                Instruction_call_assignment* instr = (Instruction_call_assignment*) in;
                Item* callee = instr->get_callee();
                Item* dst = instr->get_var();
                std::vector<Item*> args = instr->get_args();

                std::string mapped_var = get_mapping(new_map, maps, callee->to_string());
                if(mapped_var != "") callee = new Name(mapped_var);

                mapped_var = get_mapping(new_map, maps, dst->to_string());
                if(mapped_var != "") dst = new Name(mapped_var);
                
                for(int j = 0; j < args.size(); j++){
                    if(args[j]->get_name() == "Name"){
                        std::string mapped_t = get_mapping(new_map,maps, args[j]->to_string());
                        if(mapped_t != "") args[j] = new Name(mapped_t);
                    }
                }

                Instruction* new_instr = new Instruction_call_assignment(dst, callee, args);
                new_instrs.push_back(new_instr);
            }

            else if (in->get_name() == "Instruction_creeate_array"){
                Instruction_create_array* instr = (Instruction_create_array*) in;
                Item* dst = instr->get_dst_var();
                std::vector<Item*> args = instr->get_args();

                std::string mapped_var = get_mapping(new_map, maps, dst->to_string());
                if(mapped_var != "") dst = new Name(mapped_var);
                
                for(int j = 0; j < args.size(); j++){
                    if(args[j]->get_name() == "Name"){
                        std::string mapped_t = get_mapping(new_map,maps, args[j]->to_string());
                        if(mapped_t != "") args[j] = new Name(mapped_t);
                    }
                }

                Instruction* new_instr = new Instruction_create_array(dst, args);
                new_instrs.push_back(new_instr);
            }

            else if (in->get_name() == "Instruction_creeate_tuple"){
                Instruction_create_tuple* instr = (Instruction_create_tuple*) in;
                Item* dst = instr->get_dst_var();
                Item* size = instr->get_size();

                std::string mapped_var = get_mapping(new_map, maps, dst->to_string());
                if(mapped_var != "") dst = new Name(mapped_var);
                
                if(size->get_name() == "Name"){
                    mapped_var = get_mapping(new_map, maps, size->to_string());
                    if(mapped_var != "") size = new Name(mapped_var);
                }

                Instruction* new_instr = new Instruction_create_tuple(dst, size);
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

    std::vector<Instruction*>
    translate_ifs_and_gotos(std::vector<Instruction*> instructions, std::string longest_name){
        longest_name += "ifs_";
        int ctr = 0;
        std::vector<Instruction*> new_instrs;
        for(Instruction* in : instructions){
            if(in->get_name() == "Instruction_if"){
                Instruction_if* instr = (Instruction_if*) in;
                Item* t1 = instr->get_t1();
                Item* cmp = instr->get_cmp();
                Item* t2 = instr->get_t2();
                Item* l1 = instr->get_label1();
                Item* l2 = instr->get_label2();
                auto t = new Name(longest_name + std::to_string(ctr));
                std::vector<Item*> temp{t};
                auto type = new Type("int64");
                new_instrs.push_back(new Instruction_declaration(type, temp));
                new_instrs.push_back(new Instruction_op(t, t1, cmp, t2));
                new_instrs.push_back(new Instruction_branch_t(t, l1, l2));
                ctr++;

            }
            else if(in->get_name() == "Instruction_goto"){
                Instruction_goto* instr = (Instruction_goto*) in;
                Item* label = instr->get_label();
                new_instrs.push_back(new Instruction_branch(label));
            }
            else new_instrs.push_back(in);
        }

        return new_instrs;
    }

    std::vector<Instruction*>
    translate_whiles(std::vector<Instruction*> instructions, std::string longest_name,std::string longest_label){
        std::vector<Instruction*> new_instrs;
        std::unordered_map<std::string,Item*> beginWhile;
        std::unordered_map<std::string,Item*> endWhile;
        std::unordered_map<std::string,std::string> bWhile;
        std::unordered_map<std::string,std::string> eWhile;
        std::unordered_map<std::string,Item*> condLabel;
        longest_label += "label_";
        longest_name += "name_";
        int64_t l_ctr = 0;
        int64_t n_ctr = 0;
        for(Instruction* in : instructions){
            if(in->get_name() == "Instruction_while"){
                Instruction_while* instr = (Instruction_while*) in;
                Item* l1 = instr->get_label1();
                Item* l2 = instr->get_label2();
                beginWhile[instr->to_string()] = l1;
                endWhile[instr->to_string()] = l2;
                bWhile[l1->to_string()] = instr->to_string();
                eWhile[l2->to_string()] = instr->to_string();

                condLabel[instr->to_string()] = new InstructionLabel(longest_label+ std::to_string(l_ctr));
                l_ctr++;
                new_instrs.push_back(new Instruction_label(condLabel[instr->to_string()]));
            }
            new_instrs.push_back(in);
        }

        std::vector<std::string> loopStack;
        std::unordered_map<std::string,std::string> loop;
        for(Instruction* in : new_instrs){
            if(loopStack.size() > 0) loop[in->to_string()] = loopStack.back();

            if(in->get_name() == "Instruction_label"){
                Instruction_label* instr = (Instruction_label*) in;
                std::string label = instr->get_label()->to_string();
                if(bWhile.find(label) != bWhile.end()) loopStack.push_back(bWhile[label]);
                else if(eWhile.find(label) != eWhile.end()) loopStack.pop_back();
            }
        }

        std::vector<Instruction*> final_instrs;
        for(Instruction* in : new_instrs){
            if(in->get_name() == "Instruction_while"){
                Instruction_while* instr = (Instruction_while*) in;
                Item* t1 = instr->get_t1();
                Item* cmp = instr->get_cmp();
                Item* t2 = instr->get_t2();
                Item* l1 = instr->get_label1();
                Item* l2 = instr->get_label2();
                auto t = new Name(longest_name + std::to_string(n_ctr));
                std::vector<Item*> temp{t};
                auto type = new Type("int64");
                final_instrs.push_back(new Instruction_declaration(type, temp));
                final_instrs.push_back(new Instruction_op(t, t1, cmp, t2));
                final_instrs.push_back(new Instruction_branch_t(t, l1, l2));
                n_ctr++;
            }
            else if(in->get_name() == "Instruction_continue"){
                std::string s_loop = loop[in->to_string()];
                Item* l_cond = condLabel[s_loop];
                final_instrs.push_back(new Instruction_branch(l_cond));
            }
            else if(in->get_name() == "Instruction_break"){
                std::string s_loop = loop[in->to_string()];
                Item* l_exit = endWhile[s_loop];
                final_instrs.push_back(new Instruction_branch(l_exit));
            }
            else final_instrs.push_back(in);
        }

        return final_instrs;
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


            //now getting rid of ifs
            instructions = translate_ifs_and_gotos(instructions, longest_name);

            f->instructions = instructions;
            std::cerr << "after translate_ifs_and_gotos new func is \n" << f->to_string() << "\n";

            //now getting rid of whiles
            instructions = translate_whiles(instructions, longest_name, longest_label);

            f->instructions = instructions;
            std::cerr << "after translate_ifs_and_gotos new func is \n" << f->to_string() << "\n";

            outputFile << f->to_string();
        }



        outputFile.close();
    }
}