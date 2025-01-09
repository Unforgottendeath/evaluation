#ifndef __RPN_HPP__
#define __RPN_HPP__

#include <iostream>
#include <string>
#include <stack>
#include <cstdlib>
#include <sstream>

class RPN {
public:
    class RPNerror : public std::exception
    {
        const char* what() const throw();
    };
    RPN();
    ~RPN();

    double eval(std::string const & expr);
};

#endif // __RPN_HPP__