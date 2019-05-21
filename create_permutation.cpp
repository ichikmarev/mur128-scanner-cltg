/*
     Файл:    create_permutation.cpp
     Автор:   Илья Чикмарёв
*/
#include "create_permutation.h"
#include "create_permutation_tree.h"
#include "permutation_tree_to_permutation.h"

Permutation create_permutation(size_t n){
    Permutation result;
    Permutation_tree pt = create_permutation_tree(n);
    result              = permutation_tree_to_permutation(pt);
    return result;
}
