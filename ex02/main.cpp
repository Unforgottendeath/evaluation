#include "PmergeMe.hpp"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        std::cerr << "usage: ./pmerge <sequence of numbers to be sorted> [...]" << std::endl;
        return 1;
    }
    PMergeMe pmergme(argv);
    try
    {
        pmergme.parseNumbers();
        pmergme.mergeVNumbers();
        pmergme.mergeLNumbers();
    }
    catch (std::invalid_argument& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
