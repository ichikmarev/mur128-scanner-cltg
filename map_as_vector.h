/*
     Файл:    map_as_vector.h
     Автор:   Илья Чикмарёв
*/
#ifndef MAP_AS_VECTOR_H
#define MAP_AS_VECTOR_H

#include <vector>
#include <map>
#include <utility>

template<typename K, typename V>
std::vector<std::pair<K, V>> map_as_vector(const std::map<K, V>& m){
    std::vector<std::pair<K, V>> result;
    for(const auto e : m){
        result.push_back(e);
    }
    return result;
}

#endif
