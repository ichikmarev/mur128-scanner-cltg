/*
    File:    list_to_columns.h
    Автор:   Илья Чикмарёв
*/

#ifndef LIST_TO_COLUMNS_H
#define LIST_TO_COLUMNS_H
#include <cstddef>
#include <string>
#include <vector>

struct Format{
    size_t indent                 = 0; //< number of spaces before each line
    size_t number_of_columns      = 2; //< required number of columns
    size_t spaces_between_columns = 0; //< number of spaces between columns
};

/**
 * \param [in] l   vector of the processed strings
 * \param [in] f   information about formatting
 * \param [in] d   separator
 *
 * \return strings from l are separated by d and then formatted corresponding
 *         to format information f
 */
std::string string_list_to_columns(const std::vector<std::string>& l,
                                   const Format& f, char d = ',');
#endif
