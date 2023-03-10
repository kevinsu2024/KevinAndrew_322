#include <vector>
#include <string>
#include <iostream>
#include <tile.h>

namespace L3{
    std::string
    get_s_type(std::string s){
        if(s.substr(0,1) == "%") return "Variable";
        else if(s.substr(0,1) == ":") return "Label";
        else if(s.substr(0,1) == "@") return "FunctionName";
        else return "Number";
    }

    std::string
    get_t_type(std::string t){
        if(t.substr(0,1) == "%") return "Variable";
        return "Number";
    }
    void
    print_tree_(Node* n){
        std::cerr << "Node has node type " << n->node_type << " and node_val " << n->node_val << "\n";
        std::cerr <<"neighbors are: ";
        for(auto neigh : n->neighbors){
            std::cerr << "(" << neigh->node_type << ", " <<  neigh->node_val << ") ";
        }
        std::cerr << "\n";
        for (auto neigh : n->neighbors){
            print_tree_(neigh);
        }
    }

    void
    print_node_(Node* n){
        std::cerr << "Node has node type " << n->node_type << " and node_val " << n->node_val << "\n";
        std::cerr <<"neighbors are: ";
        for(auto neigh : n->neighbors){
            std::cerr << "(" << neigh->node_type << ", " <<  neigh->node_val << ") ";
        }
        std::cerr << "\n";
        return;
    }

    Node*
    instruction_to_graph(Instruction* i){
        if(i->get_name() == "Instruction_assignment"){
            Instruction_assignment* in = (Instruction_assignment*) i;
            std::string dest = in->get_var()->to_string();
            std::string s = in->get_s()->to_string();
            Node* dst = new Node{"Variable",dest};
            Node* s_node = new Node{get_s_type(s), s};
            dst->neighbors.push_back(s_node);
            return dst;
        }
        else if (i->get_name() == "Instruction_cmp"){
            Instruction_cmp* in = (Instruction_cmp*) i;
            std::string dest = in->get_var()->to_string();
            std::string t1 = in->get_t1()->to_string();
            std::string t2 = in->get_t2()->to_string();
            std::string op = in->get_op()->to_string();
            Node* dst = new Node{"Variable",dest};
            Node* val = new Node{get_t_type(t1), t1};
            Node* val2 = new Node{get_t_type(t2), t2};
            Node* oper = new Node{op};
            dst->neighbors.push_back(oper);
            oper->neighbors.push_back(val);
            oper->neighbors.push_back(val2);
            return dst;
        }
        else if (i->get_name() == "Instruction_op"){
            Instruction_op* in = (Instruction_op*) i;
            std::string dest = in->get_var()->to_string();
            std::string t1 = in->get_t1()->to_string();
            std::string t2 = in->get_t2()->to_string();
            std::string op = in->get_op()->to_string();
            Node* dst = new Node{"Variable",dest};
            Node* val = new Node{get_t_type(t1), t1};
            Node* val2 = new Node{get_t_type(t2), t2};
            Node* oper = new Node{op};
            dst->neighbors.push_back(oper);
            oper->neighbors.push_back(val);
            oper->neighbors.push_back(val2);
            return dst;
        }
        else if (i->get_name() == "Instruction_load"){
            Instruction_load* in = (Instruction_load*) i;
            std::string dest = in->get_var_dst()->to_string();
            std::string src = in->get_var_src()->to_string();
            Node* dst = new Node{"Variable",dest};
            Node* load = new Node{"Load"};
            Node* src_node = new Node{"Variable", src};
            dst->neighbors.push_back(load);
            load->neighbors.push_back(src_node);
            return dst;
        }
        else if (i->get_name() == "Instruction_store"){
            Instruction_store* in = (Instruction_store*) i;
            std::string var = in->get_var()->to_string();
            std::string s = in->get_s()->to_string();
            Node* store = new Node{"Store"};
            Node* var_node = new Node{"Variable", var};
            Node* s_node = new Node{get_s_type(s), s};
            store->neighbors.push_back(var_node);
            var_node->neighbors.push_back(s_node);
            return store;
        }
        else if (i->get_name() == "Instruction_return"){
            Node* root = new Node{"Return"};
            return root;
        }
        else if (i->get_name() == "Instruction_return_t"){
            Instruction_return_t* in = (Instruction_return_t*) i;
            std::string t_val = in->get_t()->to_string();
            Node* root = new Node{"Return"};
            Node* t = new Node{get_t_type(t_val),t_val};
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
            Node* t_node = new Node{get_t_type(t),t};
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

    void
    create_tiles(){
        Node* a = new Node{"Variable"};
        Node* b = new Node{"S"};
        a->neighbors.push_back(b);
        tiles.push_back(new Tile{a, 2, 1, "1"});

        a = new Node{"Variable"};
        b = new Node{"T"};
        Node* c = new Node{"T"};
        Node* op = new Node{"+"};
        a->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile{a, 4, 3, "2"});

        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{"-"};
        a->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile{a, 4, 3, "3"});

        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{"*"};
        a->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile{a, 4, 3, "4"}); 

        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{"&"};
        a->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile{a, 4, 3, "5"}); //TODO

        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{"<<"};
        a->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile{a, 4, 3, "6"}); 

        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{">>"};
        a->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile{a, 4, 3, "7"}); 

        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{"<"};
        a->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile{a, 4, 1, "8"}); 


        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{"<="};
        a->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile{a, 4, 1, "9"}); 

        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{"="};
        a->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile{a, 4, 1, "10"}); 


        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{">="};
        a->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile{a, 4, 1, "11"}); 


        a = new Node{"Variable"};
        b = new Node{"T"};
        c = new Node{"T"};
        op = new Node{">"};
        a->neighbors.push_back(op);
        op->neighbors.push_back(b);
        op->neighbors.push_back(c);
        tiles.push_back(new Tile{a, 4, 1, "12"}); 



        a = new Node{"Variable"};
        b = new Node{"Variable"};
        c = new Node{"Load"};
        a->neighbors.push_back(c);
        c->neighbors.push_back(b);
        tiles.push_back(new Tile{a, 3, 1, "13"});


        a = new Node{"Variable"};
        b = new Node{"S"};
        c = new Node{"Store"};
        c->neighbors.push_back(a);
        a->neighbors.push_back(b);
        tiles.push_back(new Tile{c, 3, 1, "14"});

        a = new Node{"Return"};
        tiles.push_back(new Tile{a, 1, 1, "15"}); 

        Node* instruction = new Node{"Return"};
        a = new Node{"T"};
        instruction->neighbors.push_back(a);
        tiles.push_back(new Tile{instruction, 2, 2, "16"});


        instruction = new Node{"Branch"};
        a = new Node{"Label"};
        instruction->neighbors.push_back(a);
        tiles.push_back(new Tile{instruction, 2, 1, "17"}); 

        instruction = new Node{"Branch"};
        a = new Node{"Label"};
        b = new Node{"T"};
        instruction->neighbors.push_back(b);
        instruction->neighbors.push_back(a);
        tiles.push_back(new Tile{instruction, 3, 1, "18"}); 

        
        //start of complex tiles
        // instruction = new Node{"Variable"};
        // Node* add_op = new Node{"+"};
        // a = new Node{"T"};
        // b = new Node{"T"};
        // instruction->neighbors.push_back(add_op);
        // add_op->neighbors.push_back(a);
        // add_op->neighbors.push_back(b);
        // Node* times_op = new Node{"*"};
        // b->neighbors.push_back(times_op);
        // c = new Node{"T"};
        // Node* d = new Node{""}

        
        // sort tiles for maximal munch: first by highest num of nodes, then break ties with lowest num instructions.
        std::sort(tiles.begin(), tiles.end(), [](const Tile* lhs, const Tile* rhs){
            return  lhs->num_nodes > rhs->num_nodes || 
                    (lhs->num_nodes == rhs->num_nodes && lhs->num_instructions < rhs->num_instructions);
        });

    }


    bool
    check_node_types(std::string t1, std::string tile){
        if(t1 == tile) return true;
        if(tile == "T" && (t1 == "Variable" || t1 == "Number")) return true;
        if(tile == "S" && (t1 == "Variable" || t1 == "Number" || t1 == "T" || t1 == "Label" || t1 == "FunctionName")) return true;
        if(tile == "SameVariable" && t1 == "Variable") return true;
        return false;
    }

    bool
    check_tile(Node* tree, Node* t, bool verbose){
        if((!tree) && (!t)) return true;
        if(!tree){
            if (verbose){
                std::cerr << "does not match reason: input tree is null\n";
            }
            return false;
        } 
        if(tree && !t) return true;
        if(!check_node_types(tree->node_type, t->node_type)){
            if (verbose){
                std::cerr << "does not match reason: node type" << tree->node_type << " does not match" << t->node_type << "\n";
            }
            return false;
        } 
        if(t->neighbors.size() > tree->neighbors.size()){
            if (verbose){
                std::cerr << "does not match reason: tile has neighbor size" << t->neighbors.size() << " versus tree's neighbor size" << tree->neighbors.size() << "\n";
            }
            return false;
        } 
        bool has_match = false;
        for(int64_t i = 0; i < tree->neighbors.size(); i++){
            for (int64_t j = 0; j < t->neighbors.size(); j++){
                if(check_tile(tree->neighbors[i], t->neighbors[j], verbose)){
                    if (verbose){
                        std::cerr << "neighbor of tree: " << tree->neighbors[i]->node_type << " matches neighbor of tile: " << t->neighbors[j]->node_type;
                    }
                    has_match = true;
                } else {
                    if (verbose){
                        std::cerr << "neighbor of tree: " << tree->neighbors[i]->node_type << " does not match neighbor of tile: " << t->neighbors[j]->node_type;
                    }
                }
            }
            if (!has_match && t->neighbors.size() != 0){
                if (verbose){
                    std::cerr << "none of tree: " << tree->node_type << "'s neighbors match with that of tile: " << t->node_type;
                }
                return false;
            }
        }
        
        return true;
    }

    // bool
    // check_special(Node* tree, Tile* t, bool verbose){
    //     //w aop t
    //     if (t->id == "19" || t->id == "20" || t->id == "21" || t->id == "22"){
    //         Node* op = tree->neighbors[0];
    //         std::vector<Node*> children = op->neighbors;
    //         if (children.size() != 2) return false;
    //         for (int i = 0; i < 2; i++){
    //             if (children[i]->node_type == "Variable" && children[(i-1) % 2]->node_type == "Number"){
    //                 if (children[i]->node_val != tree->node_val) return false;
    //             }
    //         }
    //         return true;
    //     }
    //     return false;
    // }
    void
    print_tree(Node* tree){
        std::cout << tree->node_type << " " << tree->node_val << "\n";
        for(auto neigh : tree->neighbors){
            print_tree(neigh);
        };
        return;
    }

    std::vector<Node*>
    remove_munched_portion(Node* tree, Node* t, std::set<Node*> visited){
        std::vector<Node*> subtrees;
        if((!tree) && (!t)) return subtrees;
        if(!tree) return subtrees;
        if(!t) return subtrees;
        if(tree->node_type != t->node_type) return subtrees;
        if(tree->neighbors.size() != t->neighbors.size()){
            subtrees.push_back(tree);
            return subtrees;
        }
        for(int64_t i = 0; i < tree->neighbors.size(); i++){
            for(int64_t j = 0; j < t->neighbors.size(); j++){
                if (std::find(visited.begin(), visited.end(), tree->neighbors[i]) != visited.end() &&
                    std::find(visited.begin(), visited.end(), t->neighbors[j]) != visited.end()){
                    return subtrees;
                } else {
                    visited.insert(tree->neighbors[i]);
                    visited.insert(t->neighbors[j]);
                    std::vector<Node*> new_munch = remove_munched_portion(tree->neighbors[i], t->neighbors[j], visited);
                    for (auto subt : new_munch){
                        subtrees.push_back(subt);
                    }
                }   
            }
        }
        // std::cerr << "failed to munch uh oh\n";
        return subtrees;
    }
    

    //return: a sequence of subtrees and their corresponding tiles
    std::vector<std::tuple<Node*, Tile*>>
    maximal_munch(Node* tree, bool verbose){
        // std::cout<< "\n\nprinting tree\n";
        // print_tree(tree);

        
        std::vector<std::tuple<Node*, Tile*>> ret_sequence;
        //if tree completely munched, return
        if (tree == nullptr){
            return ret_sequence;
        }
        //sort tile by no. of nodes, then by cost

        //check in order of the tiles array, the first tile that is able to match with tree.
        //then, take out subtree after munch and munch it
        if (verbose){
            std::cerr << "for tree: \n";
            print_tree_(tree);
            std::cerr << "\n\n";
        }
        
        for(Tile* t : tiles){
            std::tuple<Node*, Tile*> tup (nullptr, nullptr);
            std::set<Node*> visited;
            if (verbose){
                std::cerr << "checking tile: \n" ;
                print_node_(t->root);
                std::cerr << "\n\n";
            }
            if (!check_tile(tree,t->root, verbose)){
                if (verbose){
                    std::cerr << "tile does not match.\n";
                }
            } else {
                if (verbose){
                    std::cerr << "tile matches!\n";
                }
                
                std::get<1>(tup) = t;
                std::get<0>(tup) = tree;
                ret_sequence.push_back(tup);
                auto list_of_new_trees = remove_munched_portion(tree,t->root, visited);
                for (auto new_tree : list_of_new_trees){
                    auto sub_munch = maximal_munch(new_tree, verbose);
                    for (std::tuple<Node*, Tile*> tuple : sub_munch){
                        ret_sequence.push_back(tuple);
                    }
                }
                return ret_sequence;
                
            } 
        }
        if (ret_sequence.size() == 0){
            std::cerr << "failed to find tile uh oh\n";
        }
        return ret_sequence;
    }


    std::string
    convert_to_instructions(Node* tree, Tile* tile){
        std::string tile_id = tile->id;
        std::string code = "";
        if(tile_id == "1"){
            Node* first = tree;
            Node* second = tree->neighbors[0];
            code += (first->node_val + " <- " + second->node_val + "\n");
        }
        else if(tile_id == "2"){
            Node* dest = tree;
            Node* val1 = tree->neighbors[0]->neighbors[0];
            Node* val2 = tree->neighbors[0]->neighbors[1];
            code += (dest->node_val+"_holder_ <- " + val1->node_val + "\n");
            code += (dest->node_val+"_holder_ += " + val2->node_val + "\n");
            code += (dest->node_val + " <- " + dest->node_val+"_holder_" + "\n");
        }
        else if(tile_id == "3"){
            Node* dest = tree;
            Node* val1 = tree->neighbors[0]->neighbors[0];
            Node* val2 = tree->neighbors[0]->neighbors[1];
            code += (dest->node_val+"_holder_ <- " + val1->node_val + "\n");
            code += (dest->node_val+"_holder_ -= " + val2->node_val + "\n");
            code += (dest->node_val + " <- " + dest->node_val+"_holder_" + "\n");
        }
        else if(tile_id == "4"){
            Node* dest = tree;
            Node* val1 = tree->neighbors[0]->neighbors[0];
            Node* val2 = tree->neighbors[0]->neighbors[1];
            code += (dest->node_val+"_holder_ <- " + val1->node_val + "\n");
            code += (dest->node_val+"_holder_ *= " + val2->node_val + "\n");
            code += (dest->node_val + " <- " + dest->node_val+"_holder_" + "\n");
        }
        else if(tile_id == "5"){
            Node* dest = tree;
            Node* val1 = tree->neighbors[0]->neighbors[0];
            Node* val2 = tree->neighbors[0]->neighbors[1];
            code += (dest->node_val+"_holder_ <- " + val1->node_val + "\n");
            code += (dest->node_val+"_holder_ &= " + val2->node_val + "\n");
            code += (dest->node_val + " <- " + dest->node_val+"_holder_" + "\n");
        }
        else if(tile_id == "6"){
            Node* dest = tree;
            Node* val1 = tree->neighbors[0]->neighbors[0];
            Node* val2 = tree->neighbors[0]->neighbors[1];
            code += (dest->node_val+"_holder_ <- " + val1->node_val + "\n");
            code += (dest->node_val+"_holder_ <<= " + val2->node_val + "\n");
            code += (dest->node_val + " <- " + dest->node_val+"_holder_" + "\n");
        }
        else if(tile_id == "7"){
            Node* dest = tree;
            Node* val1 = tree->neighbors[0]->neighbors[0];
            Node* val2 = tree->neighbors[0]->neighbors[1];
            code += (dest->node_val+"_holder_ <- " + val1->node_val + "\n");
            code += (dest->node_val+"_holder_ >>= " + val2->node_val + "\n");
            code += (dest->node_val + " <- " + dest->node_val+"_holder_" + "\n");
        }

        else if(tile_id == "8"){
            Node* dest = tree;
            Node* val1 = tree->neighbors[0]->neighbors[0];
            Node* val2 = tree->neighbors[0]->neighbors[1];
            code += (dest->node_val + " <- " + val1->node_val + " < " + val2->node_val +"\n");
        }

        else if(tile_id == "9"){
            Node* dest = tree;
            Node* val1 = tree->neighbors[0]->neighbors[0];
            Node* val2 = tree->neighbors[0]->neighbors[1];
            code += (dest->node_val + " <- " + val1->node_val + " <= " + val2->node_val +"\n");
        }

        else if(tile_id == "10"){
            Node* dest = tree;
            Node* val1 = tree->neighbors[0]->neighbors[0];
            Node* val2 = tree->neighbors[0]->neighbors[1];
            code += (dest->node_val + " <- " + val1->node_val + " = " + val2->node_val +"\n");
        }

        else if(tile_id == "11"){
            Node* dest = tree;
            Node* val1 = tree->neighbors[0]->neighbors[0];
            Node* val2 = tree->neighbors[0]->neighbors[1];
            code += (dest->node_val + " <- " + val2->node_val + " <= " + val1->node_val +"\n");
        }

        else if(tile_id == "12"){
            Node* dest = tree;
            Node* val1 = tree->neighbors[0]->neighbors[0];
            Node* val2 = tree->neighbors[0]->neighbors[1];
            code += (dest->node_val + " <- " + val2->node_val + " < " + val1->node_val +"\n");
        }

        else if(tile_id == "13"){
            Node* dest = tree;
            Node* src = tree->neighbors[0]->neighbors[0];
            code += (dest->node_val + " <- mem " + src->node_val+ " 0\n");
        }
        else if(tile_id == "14"){
            Node* val1 = tree->neighbors[0];
            Node* val2 = tree->neighbors[0]->neighbors[0];
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



