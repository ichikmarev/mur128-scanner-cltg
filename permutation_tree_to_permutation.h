/*
     Файл:    permutation_tree_to_permutation.h
     Автор:   Илья Чикмарёв
*/
#ifndef PERMUTATION_TREE_TO_PERMUTATION_H
#define PERMUTATION_TREE_TO_PERMUTATION_H
#include "permutation.h"
#include "create_permutation_tree.h"
/*
 * The following function uses the centered algorithm of the tree traversal from
 * Корнеев Г.А., Шамгунов Н.Н., Шалыто А.А. Обход дерева на основе автоматного подхода //
 * Компьютерные инструменты в образовании. --- 2004. №3. --- с.32--37.
*/
Permutation permutation_tree_to_permutation(const Permutation_tree& pt);
#endif
