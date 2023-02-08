#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <L3.h>

namespace L3{
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

    static std::vector<Tile*> tiles;

    Node* create_tiles();
    
    std::string convert_to_instructions(Node* tree, Tile* tile);
    Node* instruction_to_graph(Instruction* i);
    Tile* get_matching_tile(Node* tree);

    

    

}