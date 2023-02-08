#pragma once

#include <vector>
#include <string>
#include <iostream>

namespace L3{
    struct Node{
        std::string node_type;
        std::string node_val;
        std::vector<Node*>  neighbors;
    };

    class Tile{
        public:
            Tile(Node* root, int64_t instr);
            int64_t get_num_instructions();
            Node* get_root();
        private:
            int64_t num_instructions;
            Node* root;
    };

    std::vector<Tile*> tiles;

}