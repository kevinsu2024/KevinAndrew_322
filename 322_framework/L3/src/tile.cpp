#include <vector>
#include <string>
#include <iostream>
#include <tile.h>

namespace L3{
    
    Node*
    instruction_to_graph(Instruction* i){
        if(i->get_name() == "Instruction_assignment"){
            Instruction_assignment* in = (Instruction_assignment*) i;
            std::string dest = in->get_var()->to_string();
            std::string s = in->get_s()->to_string();
            Node* root = new Node{"<-"};
            Node* dst = new Node{"Variable",dest};
            Node* s_node = new Node{"S", s};
            root->neighbors.push_back(dst);
            root->neighbors.push_back(s_node);
            return root;
        }
        else if (i->get_name() == "Instruction_cmp"){
            Instruction_cmp* in = (Instruction_cmp*) i;
            std::string dest = in->get_var()->to_string();
            std::string t1 = in->get_t1()->to_string();
            std::string t2 = in->get_t2()->to_string();
            std::string op = in->get_op()->to_string();
            Node* root = new Node{"<-"};
            Node* dst = new Node{"Variable",dest};
            Node* val = new Node{"T", t1};
            Node* val2 = new Node{"T", t2};
            Node* oper = new Node{op};
            root->neighbors.push_back(dst);
            root->neighbors.push_back(oper);
            oper->neighbors.push_back(val);
            oper->neighbors.push_back(val2);
            return root;
        }
        else if (i->get_name() == "Instruction_op"){
            Instruction_op* in = (Instruction_op*) i;
            std::string dest = in->get_var()->to_string();
            std::string t1 = in->get_t1()->to_string();
            std::string t2 = in->get_t2()->to_string();
            std::string op = in->get_op()->to_string();
            Node* root = new Node{"<-"};
            Node* dst = new Node{"Variable",dest};
            Node* val = new Node{"T", t1};
            Node* val2 = new Node{"T", t2};
            Node* oper = new Node{op};
            root->neighbors.push_back(dst);
            root->neighbors.push_back(oper);
            oper->neighbors.push_back(val);
            oper->neighbors.push_back(val2);
            return root;
        }
        else if (i->get_name() == "Instruction_load"){
            Instruction_load* in = (Instruction_load*) i;
            std::string dest = in->get_var_dst()->to_string();
            std::string src = in->get_var_src()->to_string();
            Node* root = new Node{"<-"};
            Node* dst = new Node{"Variable",dest};
            Node* load = new Node{"Load"};
            Node* src_node = new Node{"Variable", src};
            root->neighbors.push_back(dst);
            root->neighbors.push_back(load);
            root->neighbors.push_back(src_node);
            return root;
        }
        else if (i->get_name() == "Instruction_store"){
            Instruction_store* in = (Instruction_store*) i;
            std::string var = in->get_var()->to_string();
            std::string s = in->get_s()->to_string();
            Node* root = new Node{"<-"};
            Node* store = new Node{"Store"};
            Node* var_node = new Node{"Variable", var};
            Node* s_node = new Node{"S", s};
            root->neighbors.push_back(store);
            root->neighbors.push_back(var_node);
            root->neighbors.push_back(s_node);
            return root;
        }
        else if (i->get_name() == "Instruction_return"){
            Node* root = new Node{"Return"};
            return root;
        }
        else if (i->get_name() == "Instruction_return_t"){
            Instruction_return_t* in = (Instruction_return_t*) i;
            std::string t_val = in->get_t()->to_string();
            Node* root = new Node{"Return"};
            Node* t = new Node{"T",t_val};
            root->neighbors.push_back(t);
            return root;
        }
        else if (i->get_name() == "Instruction_branch"){
            Instruction_branch* in = (Instruction_branch*) i;
            std::string label = in->get_label()->to_string();
            Node* root = new Node{"Branch"};
            Node* l_node = new Node{"Label",label};
            root->neighbors.push_back(l_node);
            return root;
        }
        else if (i->get_name() == "Instruction_branch_t"){
            Instruction_branch_t* in = (Instruction_branch_t*) i;
            std::string label = in->get_label()->to_string();
            std::string t = in->get_t()->to_string();
            Node* root = new Node{"Branch"};
            Node* l_node = new Node{"Label",label};
            Node* t_node = new Node{"T",t};
            root->neighbors.push_back(t_node);
            root->neighbors.push_back(l_node);
            return root;
        }
        else{
            std::cerr << "\nFAILED TO FIND MATCH\n";
            return new Node{};
        }
        return new Node{};
    }

    Node*
    create_tiles(){
        Node* a = new Node{"Variable"};
        Node* b = new Node{"S"};
        Node* t1 = new Node;
        t1->node_type = "<-";
        t1->neighbors.push_back(a);
        t1->neighbors.push_back(b);
        tiles.push_back(new Tile(t1, 1, "1"));

        a = new Node{"Variable"};
        b = new Node{"T"};
        Node* c = new Node{"T"};
        Node* op = new Node{"+"};
        Node* instruction = new Node{"<-"};
        instruction->neighbors.push_back(a);
        instruction->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile(instruction, 0, "2")); //TODO

        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{"-"};
        instruction = new Node{"<-"};
        instruction->neighbors.push_back(a);
        instruction->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile(instruction, 0, "3")); //TODO

        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{"*"};
        instruction = new Node{"<-"};
        instruction->neighbors.push_back(a);
        instruction->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile(instruction, 0, "4")); //TODO

        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{"&"};
        instruction = new Node{"<-"};
        instruction->neighbors.push_back(a);
        instruction->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile(instruction, 0, "5")); //TODO

        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{"<<"};
        instruction = new Node{"<-"};
        instruction->neighbors.push_back(a);
        instruction->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile(instruction, 0, "6")); //TODO

        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{">>"};
        instruction = new Node{"<-"};
        instruction->neighbors.push_back(a);
        instruction->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile(instruction, 0, "7")); //TODO

        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{"<"};
        instruction = new Node{"<-"};
        instruction->neighbors.push_back(a);
        instruction->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile(instruction, 0, "8")); //TODO


        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{"<="};
        instruction = new Node{"<-"};
        instruction->neighbors.push_back(a);
        instruction->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile(instruction, 0, "9")); //TODO

        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{"="};
        instruction = new Node{"<-"};
        instruction->neighbors.push_back(a);
        instruction->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile(instruction, 0, "10")); //TODO


        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{">="};
        instruction = new Node{"<-"};
        instruction->neighbors.push_back(a);
        instruction->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile(instruction, 0, "11")); //TODO


        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{">"};
        instruction = new Node{"<-"};
        instruction->neighbors.push_back(a);
        instruction->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile(instruction, 0, "12")); //TODO



        a = new Node{"Variable"};
        b = new Node{"Variable"};
        c = new Node{"Load"};
        instruction = new Node{"<-"};
        instruction->neighbors.push_back(a);
        instruction->neighbors.push_back(c);
        instruction->neighbors.push_back(b);
        tiles.push_back(new Tile(instruction, 0, "13")); //TODO


        a = new Node{"Variable"};
        b = new Node{"S"};
        c = new Node{"Store"};
        instruction = new Node{"<-"};
        instruction->neighbors.push_back(c);
        instruction->neighbors.push_back(a);
        instruction->neighbors.push_back(c);
        tiles.push_back(new Tile(instruction, 0, "14")); //TODO

        instruction = new Node{"Return"};
        tiles.push_back(new Tile(instruction, 1, "15")); //TODO

        instruction = new Node{"Return"};
        a = new Node{"T"};
        instruction->neighbors.push_back(a);
        tiles.push_back(new Tile(instruction, 0, "16")); //TODO


        instruction = new Node{"Branch"};
        a = new Node{"Label"};
        instruction->neighbors.push_back(a);
        tiles.push_back(new Tile(instruction, 0, "17")); //TODO

        instruction = new Node{"Branch"};
        a = new Node{"Label"};
        b = new Node{"T"};
        instruction->neighbors.push_back(b);
        instruction->neighbors.push_back(a);
        tiles.push_back(new Tile(instruction, 0, "18")); //TODO



        return t1;
        
    }

    Tile::Tile(Node* r, int64_t i, std::string idd){
        num_instructions = i;
        root = r;
        id = idd;
    }

    int64_t
    Tile::get_num_instructions(){
        return num_instructions;
    }

    std::string
    Tile::get_id(){
        return id;
    }

    Node*
    Tile::get_root(){
        return root;
    }

    bool
    check_tile(Node* tree, Node* t){
        if((!tree) && (!t)) return true;
        if(!tree) return false;
        if(!t) return false;
        if(tree->node_type != t->node_type) return false;
        if(tree->neighbors.size() != t->neighbors.size()) return false;
        for(int64_t i = 0; i < tree->neighbors.size(); i++){
            if(!check_tile(tree->neighbors[i], t->neighbors[i])) return false;
        }
        return true;
    }

    void
    print_tree(Node* tree){
        std::cout << tree->node_type << " " << tree->node_val << "\n";
        for(auto neigh : tree->neighbors){
            print_tree(neigh);
        };
        return;
    }



    Tile*
    get_matching_tile(Node* tree){
        // std::cout<< "\n\nprinting tree\n";
        // print_tree(tree);
        for(Tile* t : tiles){
            if(check_tile(tree,t->get_root())) return t;
        }
        std::cerr << "\nfailed to match uh oh";
        return tiles[0];
    }


    std::string
    convert_to_instructions(Node* tree, Tile* tile){
        std::string tile_id = tile->get_id();
        std::string code = "";
        if(tile_id == "1"){
            Node* first = tree->neighbors[0];
            Node* second = tree->neighbors[1];
            code += (first->node_val + " <- " + second->node_val + "\n");
        }
        else if(tile_id == "2"){
            Node* dest = tree->neighbors[0];
            Node* val1 = tree->neighbors[1]->neighbors[0];
            Node* val2 = tree->neighbors[1]->neighbors[1];
            code += (dest->node_val+"_holder_ <- " + val1->node_val + "\n");
            code += (dest->node_val+"_holder_ += " + val2->node_val + "\n");
            code += (dest->node_val + " <- " + dest->node_val+"_holder_" + "\n");
        }
        else if(tile_id == "3"){
            Node* dest = tree->neighbors[0];
            Node* val1 = tree->neighbors[1]->neighbors[0];
            Node* val2 = tree->neighbors[1]->neighbors[1];
            code += (dest->node_val+"_holder_ <- " + val1->node_val + "\n");
            code += (dest->node_val+"_holder_ -= " + val2->node_val + "\n");
            code += (dest->node_val + " <- " + dest->node_val+"_holder_" + "\n");
        }
        else if(tile_id == "4"){
            Node* dest = tree->neighbors[0];
            Node* val1 = tree->neighbors[1]->neighbors[0];
            Node* val2 = tree->neighbors[1]->neighbors[1];
            code += (dest->node_val+"_holder_ <- " + val1->node_val + "\n");
            code += (dest->node_val+"_holder_ *= " + val2->node_val + "\n");
            code += (dest->node_val + " <- " + dest->node_val+"_holder_" + "\n");
        }
        else if(tile_id == "5"){
            Node* dest = tree->neighbors[0];
            Node* val1 = tree->neighbors[1]->neighbors[0];
            Node* val2 = tree->neighbors[1]->neighbors[1];
            code += (dest->node_val+"_holder_ <- " + val1->node_val + "\n");
            code += (dest->node_val+"_holder_ &= " + val2->node_val + "\n");
            code += (dest->node_val + " <- " + dest->node_val+"_holder_" + "\n");
        }
        else if(tile_id == "6"){
            Node* dest = tree->neighbors[0];
            Node* val1 = tree->neighbors[1]->neighbors[0];
            Node* val2 = tree->neighbors[1]->neighbors[1];
            code += (dest->node_val+"_holder_ <- " + val1->node_val + "\n");
            code += (dest->node_val+"_holder_ <<= " + val2->node_val + "\n");
            code += (dest->node_val + " <- " + dest->node_val+"_holder_" + "\n");
        }
        else if(tile_id == "7"){
            Node* dest = tree->neighbors[0];
            Node* val1 = tree->neighbors[1]->neighbors[0];
            Node* val2 = tree->neighbors[1]->neighbors[1];
            code += (dest->node_val+"_holder_ <- " + val1->node_val + "\n");
            code += (dest->node_val+"_holder_ >>= " + val2->node_val + "\n");
            code += (dest->node_val + " <- " + dest->node_val+"_holder_" + "\n");
        }

        else if(tile_id == "8"){
            Node* dest = tree->neighbors[0];
            Node* val1 = tree->neighbors[1]->neighbors[0];
            Node* val2 = tree->neighbors[1]->neighbors[1];
            code += (dest->node_val + " <- " + val1->node_val + " < " + val2->node_val +"\n");
        }

        else if(tile_id == "9"){
            Node* dest = tree->neighbors[0];
            Node* val1 = tree->neighbors[1]->neighbors[0];
            Node* val2 = tree->neighbors[1]->neighbors[1];
            code += (dest->node_val + " <- " + val1->node_val + " <= " + val2->node_val +"\n");
        }

        else if(tile_id == "10"){
            Node* dest = tree->neighbors[0];
            Node* val1 = tree->neighbors[1]->neighbors[0];
            Node* val2 = tree->neighbors[1]->neighbors[1];
            code += (dest->node_val + " <- " + val1->node_val + " = " + val2->node_val +"\n");
        }

        else if(tile_id == "11"){
            Node* dest = tree->neighbors[0];
            Node* val1 = tree->neighbors[1]->neighbors[0];
            Node* val2 = tree->neighbors[1]->neighbors[1];
            code += (dest->node_val + " <- " + val2->node_val + " <= " + val1->node_val +"\n");
        }

        else if(tile_id == "12"){
            Node* dest = tree->neighbors[0];
            Node* val1 = tree->neighbors[1]->neighbors[0];
            Node* val2 = tree->neighbors[1]->neighbors[1];
            code += (dest->node_val + " <- " + val2->node_val + " < " + val1->node_val +"\n");
        }

        else if(tile_id == "13"){
            Node* dest = tree->neighbors[0];
            Node* src = tree->neighbors[2];
            code += (dest->node_val + " <- mem " + src->node_val+ " 0\n");
        }
        else if(tile_id == "14"){
            Node* val1 = tree->neighbors[1];
            Node* val2 = tree->neighbors[2];
            code += ("mem " + val1->node_val + " 0 " + " <- " + val2->node_val+ "\n");
        }
        else if(tile_id == "15"){
            code += ("return\n");
        }
        else if(tile_id == "16"){
            Node* val = tree->neighbors[0];
            code += ("rax <- " + val->node_val + "\n");
            code += ("return\n");
        }
        else if(tile_id == "17"){
            Node* label = tree->neighbors[0];
            code += ("goto " + label->node_val + "\n");
        }
        else if(tile_id == "18"){
            Node* val = tree->neighbors[0];
            Node* label = tree->neighbors[1];
            code += ("cjump " + val->node_val + " = 1 " + label->node_val + "\n");
        }

        return code;
    }
}



