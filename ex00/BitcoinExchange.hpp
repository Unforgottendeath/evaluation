#ifndef __BITCOINEXCHANGE_HPP__
#define __BITCOINEXCHANGE_HPP__

#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <map>

class BitcoinExchange
{
    private:
        std::ifstream& user_db;
        std::ifstream wallet_db;
        int line_number;
        struct Date
        {
            int day;
            int month;
            int year;
            int encode() const
            {
                return year * 10000 + month * 100 + day;
            }
            bool operator<(const Date& other) const
            {
                return encode() < other.encode();
            }
            bool operator==(const Date& other) const
            {
                return encode() == other.encode();
            }
            bool operator>(const Date& other) const
            {
                return encode() > other.encode();
            }
        };
        std::map <Date, double> wallet_data;
    public:
        class BitcoinExchangeExcept : public std::exception
        {
            private:
                std::string message;
            public:
                BitcoinExchangeExcept(const std::string& message) : message(message)
                {}
                ~BitcoinExchangeExcept() throw()
                {}
                const char* what() const throw()
                {
                    return message.c_str();
                }
        };
        void getUserData();
        Date findClosestDate(const Date& date);
        void getWalletData();
        void parseDate(const std::string& date);
        void parseValue(const std::string& value);
        void IgnoreWhitespace(std::string& str);
        BitcoinExchangeExcept error(const std::string& message);
        BitcoinExchange(std::ifstream& db);
        ~BitcoinExchange();
};

inline void Isdigit(const char& c)
{
    if (!isdigit(c))
        throw std::invalid_argument("Error: invalid value");
}

#endif // __BITCOINEXCHANGE_HPP__