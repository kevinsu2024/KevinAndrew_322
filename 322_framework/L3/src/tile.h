#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <L3.h>
#include <algorithm>
#include <tuple>
#include <set>
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
    void print_tree_(Node* tree);
    void print_node_(Node* node);
    
    std::string convert_to_instructions(Node* tree, Tile* tile);
    Node* instruction_to_graph(Instruction* i);
    std::vector<std::tuple<Node*, Tile*>> maximal_munch(Node* tree, bool verbose);
    std::vector<Node*> remove_munched_portion(Node* tree, Node* tile_root, std::set<Node*> visited);
    bool check_tile(Node* tree, Node* tile, bool verbose);
    bool check_special(Node* tree, Node* tile, bool verbose);

    

    

}