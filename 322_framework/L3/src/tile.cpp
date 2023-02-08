#include <vector>
#include <string>
#include <iostream>

    struct Node{
        std::string node_type;
        std::string node_val;
        std::vector<Node*>  neighbors;
    };

    class Tile{
        public:
            Tile(Node* root, int64_t instr, std::string i);
            int64_t get_num_instructions();
            Node* get_root();
            std::string get_id();
        private:
            int64_t num_instructions;
            Node* root;
            std::string id;
    };

    std::vector<Tile*> tiles;

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
        tiles.push_back(new Tile(instruction, 0, "15")); //TODO

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



    Tile*
    get_matching_tile(Node* tree){
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
            code += (dest->node_val + " <- " + val1->node_val + "\n");
            code += (dest->node_val + " += " + val2->node_val + "\n");
        }
        else if(tile_id == "3"){
            Node* dest = tree->neighbors[0];
            Node* val1 = tree->neighbors[1]->neighbors[0];
            Node* val2 = tree->neighbors[1]->neighbors[1];
            code += (dest->node_val + " <- " + val1->node_val + "\n");
            code += (dest->node_val + " -= " + val2->node_val + "\n");
        }
        else if(tile_id == "4"){
            Node* dest = tree->neighbors[0];
            Node* val1 = tree->neighbors[1]->neighbors[0];
            Node* val2 = tree->neighbors[1]->neighbors[1];
            code += (dest->node_val + " <- " + val1->node_val + "\n");
            code += (dest->node_val + " *= " + val2->node_val + "\n");
        }
        else if(tile_id == "5"){
            Node* dest = tree->neighbors[0];
            Node* val1 = tree->neighbors[1]->neighbors[0];
            Node* val2 = tree->neighbors[1]->neighbors[1];
            code += (dest->node_val + " <- " + val1->node_val + "\n");
            code += (dest->node_val + " &= " + val2->node_val + "\n");
        }
        else if(tile_id == "6"){
            Node* dest = tree->neighbors[0];
            Node* val1 = tree->neighbors[1]->neighbors[0];
            Node* val2 = tree->neighbors[1]->neighbors[1];
            code += (dest->node_val + " <- " + val1->node_val + "\n");
            code += (dest->node_val + " <<= " + val2->node_val + "\n");
        }
        else if(tile_id == "7"){
            Node* dest = tree->neighbors[0];
            Node* val1 = tree->neighbors[1]->neighbors[0];
            Node* val2 = tree->neighbors[1]->neighbors[1];
            code += (dest->node_val + " <- " + val1->node_val + "\n");
            code += (dest->node_val + " >>= " + val2->node_val + "\n");
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


    int
    main(){
        std::cout<<"\n\nxdlmao\n\n";
        auto node = create_tiles();
        std::cout << tiles.size();

        Node* instruction = new Node{"<-"};
        Node* dest = new Node{"Variable"};
        dest->node_val = "%%xdlmao";
        Node* op = new Node{"+"};
        Node* arg = new Node{"T"};
        arg->node_val = "awfejoiwaefweo";
        Node* arg2 = new Node{"T"};
        arg2->node_val = "boom";
        instruction->neighbors.push_back(dest);
        instruction->neighbors.push_back(op);
        op->neighbors.push_back(arg);
        op->neighbors.push_back(arg2);
        std::cout <<"\n\n" << convert_to_instructions(instruction, tiles[1]) << "\n\n";
    }



