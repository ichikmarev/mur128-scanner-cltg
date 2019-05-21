/*
     Файл:    group_pairs.h
     Автор:   Илья Чикмарёв
*/
#ifndef GROUP_PAIRS_H
#define GROUP_PAIRS_H

#include <utility>
#include <vector>
#include "myconcepts.h"
#include "segment.h"

template<Integral K, typename V>
SegmentsV<K, V> group_pairs(const std::vector<std::pair<K, V>>& pairs){
    SegmentsV<K, V> result;

    size_t num_of_elems        = pairs.size();

    Segment_with_value<K,V> current;

    current.bounds.lower_bound = pairs[0].first;
    current.bounds.upper_bound = pairs[0].first;
    current.value              = pairs[0].second;

    for(size_t i = 1; i < num_of_elems; i++){
        auto p = pairs[i];
        if((current.value == p.second) && (current.bounds.upper_bound + 1 == p.first)){
            current.bounds.upper_bound++;
        }else{
            result.push_back(current);
            current.bounds.lower_bound = pairs[i].first;
            current.bounds.upper_bound = pairs[i].first;
            current.value              = pairs[i].second;
        }
    }
    result.push_back(current);
    return result;
}
#endif
