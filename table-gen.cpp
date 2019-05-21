/*
     Файл:    table-gen.cpp
     Автор:   Илья Чикмарёв
*/
#include <cstdlib>
#include <cstdio>
#include <string>
#include <map>
#include <utility>
#include <sstream>
#include <iomanip>
#include "char_conv.h"
#include "map_as_vector.h"
#include "segment.h"
#include "group_pairs.h"
#include "create_permutation_tree.h"
#include "permutation_tree_to_permutation.h"
#include "permutation.h"
#include "create_permutation.h"
#include "myconcepts.h"
#include "list_to_columns.h"
#include "format.h"

static std::map<char32_t, uint32_t> table;

enum class Category : uint32_t{
    Spaces,          Other,            Id_begin,
    Id_body,         Keyword_begin,    Delimiter_begin,
    Double_quote,    Letters_Xx,       Letters_Bb,
    Letters_Oo,      Single_quote,     Dollar,
    Hex_digit,       Oct_digit,        Bin_digit,
    Dec_digit,       Zero,             Letters_Ee,
    Plus_minus,      Precision_letter, Digits_1_to_9,
    Point
};

struct Category_info{
    std::u32string chars_;
    Category       cat_;
};

static std::u32string spaces_str()
{
    std::u32string s;
    for(char32_t c = 1; c <= U' '; c++){
        s += c;
    }
    return s;
}

static const char32_t* id_begin_chars          =
    U"_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
    U"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя";
static const char32_t* id_body_chars           =
    U"_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
    U"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя";
static const char32_t* keyword_begin_chars     =
    U"iucsfrbmpadoxlnjt";
static const char32_t* delim_begin_chars       =
    U"[]{}()!~^:;,=<>+-*/%|&";
static const char32_t* double_quote_chars      =
    U"\"";
static const char32_t* single_quote_chars      =
    U"\'";
static const char32_t* letters_Xx_chars        =
    U"Xx";
static const char32_t* letters_Bb_chars        =
    U"Bb";
static const char32_t* letters_Oo_chars        =
    U"Oo";
static const char32_t* dollar_chars            =
    U"$";
static const char32_t* hex_digits_chars        =
    U"0123456789ABCDEFabcdef";
static const char32_t* oct_digits_chars        =
    U"01234567";
static const char32_t* bin_digits_chars        =
    U"01";
static const char32_t* dec_digits_chars        =
    U"0123456789";
static const char32_t* zero_chars              =
    U"0";
static const char32_t* letters_Ee_chars        =
    U"Ee";
static const char32_t* plus_minus_chars        =
    U"+-";
static const char32_t* precision_letters_chars =
    U"fdxq";
static const char32_t* digits_1_to_9_chars     =
    U"123456789";
static const char32_t* point_chars     =
    U".";

static const Category_info categories[] = {
    {spaces_str(),            Category::Spaces          },
    {id_begin_chars,          Category::Id_begin        },
    {id_body_chars,           Category::Id_body         },
    {keyword_begin_chars,     Category::Keyword_begin   },
    {delim_begin_chars,       Category::Delimiter_begin },
    {double_quote_chars,      Category::Double_quote    },
    {single_quote_chars,      Category::Single_quote    },
    {letters_Xx_chars,        Category::Letters_Xx      },
    {letters_Bb_chars,        Category::Letters_Bb      },
    {letters_Oo_chars,        Category::Letters_Oo      },
    {dollar_chars,            Category::Dollar          },
    {hex_digits_chars,        Category::Hex_digit       },
    {oct_digits_chars,        Category::Oct_digit       },
    {bin_digits_chars,        Category::Bin_digit       },
    {dec_digits_chars,        Category::Dec_digit       },
    {zero_chars,              Category::Zero            },
    {letters_Ee_chars,        Category::Letters_Ee      },
    {plus_minus_chars,        Category::Plus_minus      },
    {precision_letters_chars, Category::Precision_letter},
    {digits_1_to_9_chars,     Category::Digits_1_to_9   },
    {point_chars,             Category::Point           }
};

// const char32_t* after_percent_chars = U"acdhiklmnst" ;
// const char32_t* delimiter_begin_chars = U",:{}-";

static void insert_char(const char32_t ch, Category category)
{
    auto it = table.find(ch);
    if(it != table.end()){
        table[ch] |= 1U << static_cast<uint32_t>(category);
    }else{
        table[ch] =  1U << static_cast<uint32_t>(category);
    }
}

static void add_category(const char32_t* p, Category category)
{
    while(char32_t ch = *p++){
        insert_char(ch, category);
    }
}

static inline void add_category(const Category_info& ci)
{
    add_category(ci.chars_.c_str(), ci.cat_);
}

void fill_table(){
    for(const auto& ci : categories){
        add_category(ci);
    }
//     std::u32string s = spaces_str();
//
//     add_category(s.c_str(),             Category::Spaces);
//     add_category(after_percent_chars,   Category::After_percent);
//     add_category(id_begin_chars,        Category::Id_begin);
//     add_category(id_body_chars,         Category::Id_body);
//     add_category(delimiter_begin_chars, Category::Delimiter_begin);
//     add_category(U"%",                  Category::Percent);
//     add_category(U"\"",                 Category::Double_quote);
//     add_category(U">",                  Category::Delimiter_body);
}

template<RandomAccessIterator DestIt, RandomAccessIterator SrcIt, Callable F>
void permutate(DestIt dest_begin, SrcIt src_begin, SrcIt src_end, F f){
    size_t num_of_elems = src_end - src_begin;
    for(size_t i = 0; i < num_of_elems; ++i){
        dest_begin[f(i)] = src_begin[i];
    }
}

template<Integral K, typename V>
SegmentsV<K, V> create_classification_table(const std::map<K, V>& m){
   SegmentsV<K,V> grouped_pairs = group_pairs(map_as_vector(m));
   size_t         n             = grouped_pairs.size();
   auto           result        = SegmentsV<K, V>(n);
   auto           perm          = create_permutation(grouped_pairs.size());
   auto           f             = [&perm](size_t i) -> size_t{return perm[i];};
   permutate(result.begin(), grouped_pairs.begin(), grouped_pairs.end(), f);
   return result;
}

static std::string show_char32(char32_t c)
{
    std::ostringstream oss;
    if(c <= U' '){
        oss << std::setw(4) << static_cast<uint32_t>(c);
    }else if(c == U'\\'){
        oss << R"~(U'\\')~";
    }else{
        oss << "U'" << char32_to_utf8(c) << "'";
    }
    return oss.str();
}

// static const std::string elem_fmt_spaces     = "{{{0:<4}, {1:<3}}}";
// static const std::string elem_fmt_not_spaces = R"~({{U'{0}', {1:<3}}})~";
static const std::string elem_fmt = R"~({{{{{0}, {1}}},  {2:<10}}})~";

static std::string show_table_elem(const Segment_with_value<char32_t, uint32_t>& e)
{
    std::string result;
    auto&       segment = e.bounds;
    result              = fmt::format(elem_fmt,
                                      show_char32(segment.lower_bound),
                                      show_char32(segment.upper_bound),
                                      e.value);
    return result;
}

static const std::string table_fmt = R"~(enum class Category : uint32_t{{
    Spaces,          Other,            Id_begin,
    Id_body,         Keyword_begin,    Delimiter_begin,
    Double_quote,    Letters_Xx,       Letters_Bb,
    Letters_Oo,      Single_quote,     Dollar,
    Hex_digit,       Oct_digit,        Bin_digit,
    Dec_digit,       Zero,             Letters_Ee,
    Plus_minus,      Precision_letter, Digits_1_to_9,
    Point
}};

/*
 * It happens that in std::map<K,V> the key type is integer, and a lot of keys with the
 * same corresponding values. If such a map must be a generated constant, then this map
 * can be optimized. Namely, iterating through a map using range-based for, we will
 * build a std::vector<std::pair<K, V>>. Then we group pairs std::pair<K, V> in pairs
 * in the form (segment, a value of type V), where 'segment' is a struct consisting of
 * lower bound and upper bound. Next, we permute the grouped pair in the such way that
 * in order to search for in the array of the resulting values we can use the algorithm
 * from the answer to exercise 6.2.24 of the book
 * Knuth D.E. The art of computer programming. Volume 3. Sorting and search. ---
 * 2nd ed. --- Addison-Wesley, 1998.
*/

#define RandomAccessIterator typename
#define Callable             typename
#define Integral             typename
template<typename T>
struct Segment{{
    T lower_bound;
    T upper_bound;

    Segment()               = default;
    Segment(const Segment&) = default;
    ~Segment()              = default;
}};

template<typename T, typename V>
struct Segment_with_value{{
    Segment<T> bounds;
    V          value;

    Segment_with_value()                          = default;
    Segment_with_value(const Segment_with_value&) = default;
    ~Segment_with_value()                         = default;
}};

/* This function uses algorithm from the answer to the exercise 6.2.24 of the monography
 *  Knuth D.E. The art of computer programming. Volume 3. Sorting and search. --- 2nd ed.
 *  --- Addison-Wesley, 1998.
*/
template<RandomAccessIterator I, typename K>
std::pair<bool, size_t> knuth_find(I it_begin, I it_end, K key)
{{
    std::pair<bool, size_t> result = {{false, 0}};
    size_t                  i      = 1;
    size_t                  n      = it_end - it_begin;
    while(i <= n){{
        const auto& curr        = it_begin[i - 1];
        const auto& curr_bounds = curr.bounds;
        if(key < curr_bounds.lower_bound){{
            i = 2 * i;
        }}else if(key > curr_bounds.upper_bound){{
            i = 2 * i + 1;
        }}else{{
            result.first = true; result.second = i - 1;
            break;
        }}
    }}
    return result;
}}

static const Segment_with_value<char32_t, uint64_t> categories_table[] = {{
{0}
}};

static constexpr size_t num_of_elems_in_categories_table = {1};

uint64_t get_categories_set(char32_t c)
{{
    auto t = knuth_find(categories_table,
                        categories_table + num_of_elems_in_categories_table,
                        c);
    return t.first ? categories_table[t.second].value : (1ULL << Other)
}}
)~";


std::string show_table(){
    std::string result;
    auto   t                 = create_classification_table(table);
    size_t num_of_elems      = t.size();

    std::vector<std::string> elems;
    for(const auto& e : t){
        elems.push_back(show_table_elem(e));
    }

    Format f;
    f.indent                 = 4;
    f.number_of_columns      = 3;
    f.spaces_between_columns = 2;

    auto table_body          =  string_list_to_columns(elems, f);
    result                   =  fmt::format(table_fmt, table_body, num_of_elems);
    return result;
}

// static const std::string table_fmt = R"~(enum Category : uint16_t{{
//     Spaces,          Other,        Percent,
//     After_percent,   Id_begin,     Id_body,
//     Delimiter_begin, Double_quote, Delimiter_body
// }};
//
// static const std::map<char32_t, uint16_t> categories_table = {{
// {0}
// }};)~";
//
// static constexpr size_t elems_in_group         = 5;
// static constexpr size_t indent_width           = 4;
// static constexpr size_t spaces_between_columns = 1;
//
// static std::string show_table()
// {
//     std::string result;
//     std::vector<std::string> elems;
//     for(const auto& p : table){
//         elems.push_back(show_table_elem(p));
//     }
//     Format f;
//     f.indent                 = indent_width;
//     f.number_of_columns      = elems_in_group;
//     f.spaces_between_columns = spaces_between_columns;
//     auto table_body          = string_list_to_columns(elems, f);
//     result                   = fmt::format(table_fmt, table_body);
//     return result;
// }

int main()
{
    fill_table();
    auto table_as_string = show_table();
    puts(table_as_string.c_str());
//     printf("sizeof(std::pair<char32_t, uint16_t>) = %zu\n",
//            sizeof(std::pair<char32_t, uint16_t>));
//     printf("num of elems in the table: %zu\n", table.size());
    return 0;
}
