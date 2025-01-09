#include "BitcoinExchange.hpp"

int main(int ac, char **av)
{
    if (ac > 2)
    {
        std::cerr << "usage ./btc <name_of_your_db_file>" << std::endl;
        return 1;
    }
    std::ifstream db(av[1]);
    try
    {
        BitcoinExchange exchange(db);
        exchange.getWalletData();
        exchange.getUserData();
        return 0;
    }
    catch (BitcoinExchange::BitcoinExchangeExcept& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}