#include "RPN.hpp"

RPN::RPN()
{}

RPN::~RPN()
{
}

const char* RPN::RPNerror::what() const throw()
{
    return "Error";
}

double RPN::eval(const std::string& expr)
{
    std::stack<double> stack;
    for (std::string::const_iterator it = expr.begin(); it != expr.end(); ++it) {
        if (*it == ' ')
            continue;
        if (isdigit(*it)) {
            stack.push(*it - '0');
        } else {
            if (stack.size() < 2)
                throw(RPNerror());
            double op2 = stack.top();
            stack.pop();
            double op1 = stack.top();
            stack.pop();
            switch (*it) {
                case '+':
                    stack.push(op1 + op2);
                    break;
                case '-':
                    stack.push(op1 - op2);
                    break;
                case '*':
                    stack.push(op1 * op2);
                    break;
                case '/':
                    stack.push(op1 / op2);
                    break;
                default:
                    throw(RPNerror());
                    break;
            }
        }
    }
    if (stack.empty() || stack.size() > 1)
        throw (RPNerror());
    return stack.top();
}