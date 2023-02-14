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
    
    struct Tile{
        Node* root;
        int64_t num_nodes;
        int64_t num_instructions;
        std::string id;
    };


    static std::vector<Tile*> tiles;

    void create_tiles();
    
    std::string convert_to_instructions(Node* tree, Tile* tile);
    Node* instruction_to_graph(Instruction* i);
    Tile* get_matching_tile(Node* tree);

    

    

}