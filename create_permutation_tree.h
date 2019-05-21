/*
     Файл:    create_permutation_tree.h
     Автор:   Илья Чикмарёв
*/
#ifndef CREATE_PERMUTATION_TREE_H
#define CREATE_PERMUTATION_TREE_H
#include <cstddef>
#include <vector>
struct Permutation_node{
    size_t index  = 0;
    size_t left   = 0;
    size_t right  = 0;
    size_t parent = 0;

    Permutation_node()                        = default;
    Permutation_node(const Permutation_node&) = default;
    ~Permutation_node()                       = default;
};

using Permutation_tree = std::vector<Permutation_node>;
// Root ot the tree is the element with the index 1.

Permutation_tree create_permutation_tree(size_t n);
#endif
