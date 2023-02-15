#include <liveness_generator.h>

using namespace std;

namespace L3{
    int64_t 
    find_label(std::string target_label, std::vector<Instruction*>* instructions){
        for (int i = 0; i < instructions->size(); i++){
            if ((*instructions)[i]->get_name() == "Instruction_label"){
                Instruction_label* label_instr = (Instruction_label*) (*instructions)[i];
                if (label_instr->get_label()->to_string() == target_label){
                    return i;
                }
            }
        }
        return -1;
    }

    void
    gen_kill(Liveness_Node* ln){
        Instruction* in = ln->instr;

        if(in->get_name() == "Instruction_assignment"){
            Instruction_assignment* instr = (Instruction_assignment*) in;
            

            if(instr->get_s()->get_name() == "Variable"){
                ln->gen.insert(instr->get_s()->to_string());
            }
            ln->kill.insert(instr->get_var()->to_string());
        }
        else if(in->get_name() == "Instruction_cmp"){
            Instruction_cmp* instr = (Instruction_cmp*) in;
            if(instr->get_t1()->get_name() == "Variable") ln->gen.insert(instr->get_t1()->to_string());
            if(instr->get_t2()->get_name() == "Variable") ln->gen.insert(instr->get_t2()->to_string());
            ln->kill.insert(instr->get_var()->to_string());
        }
        else if(in->get_name() == "Instruction_op"){
            Instruction_op* instr = (Instruction_op*) in;
            if(instr->get_t1()->get_name() == "Variable") ln->gen.insert(instr->get_t1()->to_string());
            if(instr->get_t2()->get_name() == "Variable") ln->gen.insert(instr->get_t2()->to_string());
            ln->kill.insert(instr->get_var()->to_string());
        }
        else if(in->get_name() == "Instruction_load"){
            Instruction_load* instr = (Instruction_load*) in;
            ln->gen.insert(instr->get_var_src()->to_string());
            ln->kill.insert(instr->get_var_dst()->to_string());
        }
        else if(in->get_name() == "Instruction_store"){
            Instruction_store* instr = (Instruction_store*) in;
            ln->gen.insert(instr->get_var()->to_string());
            if(instr->get_s()->get_name() == "Variable") ln->gen.insert(instr->get_s()->to_string());
        }
        else if(in->get_name() == "Instruction_return_t"){
            Instruction_return_t* instr = (Instruction_return_t*) in;
            if(instr->get_t()->get_name() == "Variable") ln->gen.insert(instr->get_t()->to_string());
        }
        else if(in->get_name() == "Instruction_branch_t"){
            Instruction_branch_t* instr = (Instruction_branch_t*) in;
            if(instr->get_t()->get_name() == "Variable") ln->gen.insert(instr->get_t()->to_string());
        }
        else if(in->get_name() == "Instruction_call"){
            Instruction_call* instr = (Instruction_call*) in;
            std::vector<Item*> args = instr->get_args();
            for(Item* arg : args){
                if(arg->get_name() == "Variable") ln->gen.insert(arg->to_string());
            }
        }
        else if(in->get_name() == "Instruction_call_assignment"){
            Instruction_call_assignment* instr = (Instruction_call_assignment*) in;
            std::vector<Item*> args = instr->get_args();
            for(Item* arg : args){
                if(arg->get_name() == "Variable") ln->gen.insert(arg->to_string());
            }
            ln->kill.insert(instr->get_var()->to_string());
        }
        else{
            return;
        }
        return;
    }

    void
    in_out(std::vector<Liveness_Node*>* nodes){
        bool change = true;
        while(change){
            change = false;
            for(int i = nodes->size()-1; i > -1; i--){
                std::set<std::string> in = {};
                std::set<std::string> out = {};
                Liveness_Node* cur_node = (*nodes)[i];

                for(int64_t succ: cur_node->succs){
                    for(std::string ele_in_suc: (*nodes)[succ]->in){
                        out.insert(ele_in_suc);
                    }
                }

                for(auto gen_item : cur_node->gen){
                    in.insert(gen_item);
                }

                for(auto out_item : cur_node->out){
                    if(cur_node->kill.find(out_item) == cur_node->kill.end()) in.insert(out_item);
                }

                if(out != cur_node->out){
                    change = true;
                    cur_node->out = out;
                }

                if(in != cur_node->in){
                    change = true;
                    cur_node->in = in;
                }

            }
        }
        return;
    }


    std::vector<Liveness_Node*>
    generate_liveness(Function* func, bool verbose){
        if(verbose) std::cerr << "starting liveness\n";
        std::vector<Instruction*> instructs = func->instructions;
        std::vector<Liveness_Node*> nodes (instructs.size());

        for(int64_t i = 0; i < instructs.size(); i++){
            nodes[i] = new Liveness_Node{instructs[i]};
        }
        if(verbose) std::cerr <<"num of liveness nodes are " << nodes.size() << "\n";;


        //do succs and preds
        for(int64_t i = 0; i < instructs.size(); i++){
            Instruction* in = instructs[i];

            if(in->get_name() == "Instruction_return_t" || in->get_name() == "Instruction_return"){
                continue;
            }
            else if(in->get_name() == "Instruction_call"){
                Instruction_call* instr = (Instruction_call*) in;
                std::string callee = instr->get_callee()->to_string();
                if(callee == "tensor-error"){
                    continue;
                }
                else{
                    nodes[i]->succs.insert(i+1);
                    nodes[i+1]->preds.insert(i);
                }
            }
            else if(in->get_name() == "Instruction_call_assignment"){
                Instruction_call_assignment* instr = (Instruction_call_assignment*) in;
                std::string callee = instr->get_callee()->to_string();
                if(callee == "tensor-error"){
                    continue;
                }
                else{
                    nodes[i]->succs.insert(i+1);
                    nodes[i+1]->preds.insert(i);
                }
            }
            else if(in->get_name() == "Instruction_branch"){
                Instruction_branch* instr = (Instruction_branch*) in;
                std::string label_name = instr->get_label()->to_string();
                int64_t succ_position = find_label(label_name, &instructs);
                nodes[i]->succs.insert(succ_position);
                nodes[succ_position]->preds.insert(i);
            }
            else if(in->get_name() == "Instruction_branch_t"){
                Instruction_branch_t* instr = (Instruction_branch_t*) in;
                std::string label_name = instr->get_label()->to_string();
                int64_t succ_position = find_label(label_name, &instructs);
                nodes[i]->succs.insert(succ_position);
                nodes[succ_position]->preds.insert(i);

                nodes[i]->succs.insert(i+1);
                nodes[i+1]->preds.insert(i);
            }
            else{
                nodes[i]->succs.insert(i+1);
                nodes[i+1]->preds.insert(i);
            }
        }
        if(verbose){
            for(auto ln : nodes){
                std::cerr << "preds are: \n";
                for(auto p : ln->preds){
                    std::cerr << p << " ";
                }
                std::cerr << "\n";
                std::cerr << "succs are: \n";
                for(auto s : ln->succs){
                    std::cerr << s << " ";
                }
                std::cerr << "\n";
            }
        }
        

        for(Liveness_Node* ln : nodes){
            gen_kill(ln);
        }
        if(verbose){
            for(auto ln : nodes){
                std::cerr << "gens are: \n";
                for(auto p : ln->gen){
                    std::cerr << p << " ";
                }
                std::cerr << "\n";
                std::cerr << "kills are: \n";
                for(auto s : ln->kill){
                    std::cerr << s << " ";
                }
                std::cerr << "\n";
            }
        }


        in_out(&nodes);

        if(verbose){
            for(auto ln : nodes){
                std::cerr << "ins are: \n";
                for(auto p : ln->in){
                    std::cerr << p << " ";
                }
                std::cerr << "\n";
                std::cerr << "outs are: \n";
                for(auto s : ln->out){
                    std::cerr << s << " ";
                }
                std::cerr << "\n";
                }   
        }



        return nodes;
    }
}