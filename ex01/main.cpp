#include "RPN.hpp"

int main(int ac, char **av)
{
    if (ac != 2) {
        std::cerr << "Usage: " << av[0] << " <expression>" << std::endl;
        return 1;
    }
    RPN rpn;
    std::istringstream iss(av[1]);
    try
    {
        std::cout << rpn.eval(iss.str()) << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return (1);
    }
    return 0;
}