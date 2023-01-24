#include <string>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <memory>
#include <liveness_generator.h>


using namespace std;

namespace L2{

    Node::Node(Instruction* i){
        instr = i;
        std::set<int64_t> preds;
        std::set<int64_t> succs;
        std::set<std::string> gen;
        std::set<std::string> kill;
        std::set<std::string> in;
        std::set<std::string> out;

    }

    std::string check_item(Item* it){
        if(it->get_name() == "Register"){
                Register* f = (Register*) it;
                return f->get_register_ID();
            } else if (it->get_name() == "Variable"){
                Variable* f = (Variable*) it;
                return f->get_variable_name();
            } 
            return "";
    }
    int64_t find_label(std::string target_label, std::vector<Instruction*>* instructions){
        for (int i = 0; i < instructions->size(); i++){
            if ((*instructions)[i]->get_name() == "Instruction_label"){
                Instruction_label* label_instr = (Instruction_label*) (*instructions)[i];
                InstructionLabel* label_item = (InstructionLabel*) label_instr->get_label();
                if (label_item->get_label_name() == target_label){
                    return i;
                }
            }
        }
        return -1;
    }

    void gen_kill(Node* n){
        Instruction* in = n->instr;
        std::cout << "\nxdxd\n";
        std::cout << in->get_name();
        if(in->get_name() == "Instruction_ret"){
            return;
        } else if (in->get_name() == "Instruction_cjump"){
            Instruction_cjump* i = (Instruction_cjump*) in;
            Item* first = i->get_first();
            Item* second = i->get_second();
            if (check_item(first) != ""){
                n->gen.insert(check_item(first));
            }
            if (check_item(second) != ""){
                n->gen.insert(check_item(second));
            }
        } else if (in->get_name() == "Instruction_call_u"){
            Instruction_call_u* i = (Instruction_call_u*) in;
            Item* item = i->get_u();
            if (check_item(item) != ""){
                n->gen.insert(check_item(item));
            }
        } else if (in->get_name() == "Instruction_pp"){
            Instruction_pp* i = (Instruction_pp*) in;
            Item* item = i->get_reg();
            if (check_item(item) != ""){
                n->gen.insert(check_item(item));
                n->kill.insert(check_item(item));
            }
        } else if (in->get_name() == "Instruction_mm"){
            Instruction_mm* i = (Instruction_mm*) in;
            Item* item = i->get_reg();
            if (check_item(item) != ""){
                n->gen.insert(check_item(item));
                n->kill.insert(check_item(item));
            }
        } else if (in->get_name() == "Instruction_at"){
            Instruction_at* i = (Instruction_at*) in;
            Item* item = i->get_reg1();
            Item* item2 = i->get_reg2();
            Item* item3 = i->get_reg3();
            if (check_item(item) != ""){
                n->kill.insert(check_item(item));
            }
            if (check_item(item2) != ""){
                n->gen.insert(check_item(item2));
            }
            if (check_item(item3) != ""){
                n->gen.insert(check_item(item3));
            }
        } else if (in->get_name() == "Instruction_cmp_assignment"){
            Instruction_cmp_assignment* i = (Instruction_cmp_assignment*) in;
            Item* item = i->get_dst();
            Item* item2 = i->get_first();
            Item* item3 = i->get_second();
            if (check_item(item) != ""){
                n->kill.insert(check_item(item));
            }
            if (check_item(item2) != ""){
                n->gen.insert(check_item(item2));
            }
            if (check_item(item3) != ""){
                n->gen.insert(check_item(item3));
            }
        }  else if (in->get_name() == "Instruction_function_assignment"){
            Instruction_function_assignment* i = (Instruction_function_assignment*) in;
            Item* item = i->get_dst();
            if (check_item(item) != ""){
                n->kill.insert(check_item(item));
            }
        } else if (in->get_name() == "Instruction_stackarg_assignment"){
            Instruction_stackarg_assignment* i = (Instruction_stackarg_assignment*) in;
            Item* item = i->get_dst();
            if (check_item(item) != ""){
                n->kill.insert(check_item(item));
            }
        } else if (in->get_name() == "Instruction_assignment"){
            Instruction_assignment* i = (Instruction_assignment*) in;
            std::cout << "herererer\n\n";
            

            Item* first = i->get_dst();
            std::cout << check_item(first) << "\n";
            Item* second = i->get_src();
            if (check_item(first) != ""){
                n->kill.insert(check_item(first));
            }
            if (check_item(second) != ""){
                n->gen.insert(check_item(second));
            }
        } else if (in->get_name() == "Instruction_mem_op_load"){
            Instruction_mem_op_load* i = (Instruction_mem_op_load*) in;
            Item* first = i->get_dst();
            Item* second = i->get_src();
            if (check_item(first) != ""){
                n->kill.insert(check_item(first));
            }
            if (check_item(second) != "" && check_item(second) != "rsp"){
                n->gen.insert(check_item(second));
            }
        } else if (in->get_name() == "Instruction_mem_load"){
            Instruction_mem_load* i = (Instruction_mem_load*) in;
            Item* first = i->get_dst();
            Item* second = i->get_src();
            if (check_item(first) != ""){
                n->kill.insert(check_item(first));
            }
            if (check_item(second) != "" && check_item(second) != "rsp"){
                n->gen.insert(check_item(second));
            }
        } else if (in->get_name() == "Instruction_mem_op_store"){
            Instruction_mem_op_store* i = (Instruction_mem_op_store*) in;
            Item* first = i->get_src();
            Item* second = i->get_x_reg();
            if (check_item(first) != ""){
                n->gen.insert(check_item(first));
            }
            if (check_item(second) != "" && check_item(second) != "rsp"){
                n->gen.insert(check_item(second));
            }
        } else if (in->get_name() == "Instruction_mem_store"){
            Instruction_mem_store* i = (Instruction_mem_store*) in;
            Item* first = i->get_src();
            Item* second = i->get_x_reg();
            if (check_item(first) != ""){
                n->gen.insert(check_item(first));
            }
            if (check_item(second) != "" && check_item(second) != "rsp"){
                n->gen.insert(check_item(second));
            }
        } else if (in->get_name() == "Instruction_aop"){
            Instruction_aop* i = (Instruction_aop*) in;
            Item* item = i->get_dst();
            Item* item2 = i->get_src();
            if (check_item(item) != ""){
                n->kill.insert(check_item(item));
            }
            if (check_item(item2) != ""){
                n->gen.insert(check_item(item2));
            }
        } else if (in->get_name() == "Instruction_sop"){
            Instruction_sop* i = (Instruction_sop*) in;
            Item* item = i->get_dst();
            Item* item2 = i->get_src();
            if (check_item(item) != ""){
                n->kill.insert(check_item(item));
            }
            if (check_item(item2) != ""){
                n->gen.insert(check_item(item2));
            }
        } 
    }
    

    void generate_liveness(Program p){
        Function* func = p.functions.back();
        std::cerr << "\n\n func name is " << func->name << "\n";
        std::vector<Instruction*> instructs = func->instructions;
        std::vector<Node*> nodes (instructs.size());
        for(int64_t i = 0; i < instructs.size(); ++i){
            nodes[i] = new Node(instructs[i]);
        }
        for(int64_t i = 0; i < instructs.size(); ++i){
            Instruction* in = instructs[i];
            if(in->get_name() == "Instruction_goto"){
                Instruction_goto* goto_label = (Instruction_goto*) in;
                InstructionLabel* label = (InstructionLabel*) goto_label->get_label();
                std::string label_name = label->get_label_name();
                int64_t succ_position = find_label(label_name, &instructs);
                nodes[i]->succs.insert(succ_position);
                nodes[succ_position]->preds.insert(i);
                // std::cout << "succ position: " << succ_position << "\n";
                // std::cout << "pred position: " << i << "\n";
            } else if (in->get_name() == "Instruction_ret" || in->get_name() == "Instruction_call_tensor_error"){

            } else if (in->get_name() == "Instruction_cjump"){
                Instruction_cjump* cjump_instr = (Instruction_cjump*) in;
                InstructionLabel* label = (InstructionLabel*) cjump_instr->get_label();
                std::string label_name = label->get_label_name();
                int64_t succ_position = find_label(label_name, &instructs);
                nodes[i]->succs.insert(succ_position);
                nodes[succ_position]->preds.insert(i);
                // std::cout << "succ position: " << succ_position << "\n";
                // std::cout << "pred position: " << i << "\n";
            } else {
                nodes[i]->succs.insert(i+1);
                nodes[i+1]->preds.insert(i);
                // std::cout << "succ position: " << i+1 << "\n";
                // std::cout << "pred position: " << i << "\n";
            }
        }

        //gen kill sets
        for (Node* n : nodes){
            gen_kill(n);

        }



        // for (int i = 0; i < instructs.size(); i++){
        //     std::cout << "pred for row " << i << "\n";
        //     for (int64_t num: nodes[i]->preds){
        //         std::cout << num << ' ';
        //     }
        //     std::cout << "\n\n";
        //     std::cout << "succs for row " << i << "\n";
        //     for (int64_t num: nodes[i]->succs){
        //         std::cout << num << " " ;
        //     }
        //     std::cout << "\n\n";
        // }

        for (int i = 0; i < instructs.size(); i++){
            std::cout << "gen for row " << i << "\n";
            for (std::string num: nodes[i]->gen){
                std::cout << num << ' ';
            }
            std::cout << "\n\n";
            std::cout << "kill for row " << i << "\n";
            for (std::string num: nodes[i]->kill){
                std::cout << num << " " ;
            }
            std::cout << "\n\n";
        }

        return;
    }

}