#ifndef __PMERGEME_HPP__
#define __PMERGEME_HPP__

#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <deque>
#include <iomanip>

class PMergeMe
{
    private:
        const char** arguments;
        std::vector<int> Vnumbers;
        std::deque<int> Lnumbers;
    public:
        PMergeMe(const char** arguments);
        ~PMergeMe();
        void parseNumbers();
        void mergeVNumbers();
        void mergeLNumbers();
        template <typename T>
        void merge_insertion_sort(T& numbers, size_t pairs_nb);
        template <typename T>
        void printNumbers(const std::string& message, T& numbers);
        template <typename T>
        void swap_pairs(typename T::iterator left, typename T::iterator right, size_t pairs_nb);
};

#endif // __PMERGEME_HPP__