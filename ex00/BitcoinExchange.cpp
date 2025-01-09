#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(std::ifstream& db) : user_db(db)
{
    if (!user_db.is_open())
        throw BitcoinExchangeExcept("Error: can't open file");
}

BitcoinExchange::BitcoinExchangeExcept BitcoinExchange::error(const std::string& message)
{
    std::stringstream ss;
    ss << line_number + 1;
    std::string line_number_str = ss.str();
    return BitcoinExchangeExcept("Error at line " + line_number_str + ": " + message);
}

BitcoinExchange::~BitcoinExchange()
{
    wallet_db.close();
    user_db.close();
}

void BitcoinExchange::IgnoreWhitespace(std::string& str)
{
    if (str.find_first_not_of("\r\n\v\t\f ") != std::string::npos)
        str = str.substr(str.find_first_not_of("\r\n\v\t\f "), str.length());
    if (str.find_first_of("\r\n\v\t\f ") != std::string::npos)
        str = str.substr(0, str.find_first_of("\r\n\v\t\f "));
}

void BitcoinExchange::parseValue(const std::string& value)
{
    std::string value_copy = value;
    IgnoreWhitespace(value_copy);
    if (value_copy.empty())
        throw error("Error: empty value");
    for (size_t i = 0; i < value_copy.length(); i++)
    {
        if ((value_copy[i] == '-' || value_copy[i] == '+') && i == 0)
            continue;
        if (!isdigit(value_copy[i]) && value_copy[i] != '.')
            throw error("Error: invalid value => " + value);
        if (value_copy[i] == '.')
        {
            if (i == 0 || i == value_copy.length() - 1)
                throw error("Error: invalid value => " + value);
            if (value_copy.find('.') != value_copy.rfind('.'))
                throw error("Error: invalid value => " + value);
        }
    }
    std::stringstream ss;
    double value_d;
    ss << value;
    ss >> value_d;
    if (value_d < 0)
        throw error("Error: too small value => " + value);
    if (value_d > 1000)
        throw error("Error: too large value => " + value);
}

void BitcoinExchange::parseDate(const std::string& date)
{
    std::string delimiter = "-";
    std::string date_copy = date;
    std::string year = date_copy.substr(0, date.find(delimiter));
    date_copy = date_copy.substr(date_copy.find(delimiter) + 1, date_copy.length());
    std::string month = date_copy.substr(0, date_copy.find(delimiter));
    date_copy = date_copy.substr(date_copy.find(delimiter) + 1, date_copy.length());
    std::string day = date_copy.substr(0, date_copy.length());
    IgnoreWhitespace(year);
    IgnoreWhitespace(month);
    IgnoreWhitespace(day);
    if (year.length() != 4 || month.length() != 2 || day.length() != 2)
        throw error("Error: bad input => " + date);
    try
    {
        std::for_each(year.begin(), year.end(), Isdigit);
        std::for_each(month.begin(), month.end(), Isdigit);
        std::for_each(day.begin(), day.end(), Isdigit);
    }
    catch (std::exception& e)
    {
        throw error(std::string(e.what()) + " => " + date);
    }
    std::stringstream ss;
    double year_d;
    ss << year;
    ss >> year_d;
    ss.clear();
    double month_d;
    ss << month;
    ss >> month_d;
    ss.clear();
    double day_d;
    ss << day;
    ss >> day_d;
    if (year_d < 0 || month_d < 1 || month_d > 12 || day_d < 1 || day_d > 31)
        throw error("Error: bad input => " + date);
}

void BitcoinExchange::getUserData()
{
    line_number = 0;
    std::string line;
    std::string delimiter = "|";
    while (std::getline(user_db, line))
    {
        std::string value = line.substr(line.find(delimiter) + 1, line.length());
        IgnoreWhitespace(value);
        std::string date = line.substr(0, line.find(delimiter));
        IgnoreWhitespace(date);
        if(line_number == 0 && date == "date" && value == "value")
        {
            line_number++;
            continue;
        }
        try
        {
            parseDate(date);
            parseValue(value);
            struct Date date_struct;
            std::stringstream ss;
            ss << date.substr(0, 4);
            ss >> date_struct.year;
            ss.clear();
            ss << date.substr(5, 2);
            ss >> date_struct.month;
            ss.clear();
            ss << date.substr(8, 2);
            ss >> date_struct.day;
            ss.clear();
            Date closest_date = findClosestDate(date_struct);
            double value_d;
            ss << value;
            ss >> value_d;
            std::cout << date << " => " << value << " = " << wallet_data[closest_date] * value_d << std::endl; 
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
        line_number++;
    }
}

BitcoinExchange::Date BitcoinExchange::findClosestDate(const Date& date)
{
    Date closest_date;
    int closest_diff = 10000000;
    for (std::map<Date, double>::iterator it = wallet_data.begin(); it != wallet_data.end(); it++)
    {
        int diff = abs(it->first.encode() - date.encode());
        if (diff < closest_diff && it->first < date)
        {
            closest_diff = diff;
            closest_date = it->first;
        }
    }
    return closest_date;
}

void BitcoinExchange::getWalletData()
{
    line_number = 0;
    std::ifstream wallet_db("data.csv");
    if (!wallet_db.is_open())
        throw BitcoinExchangeExcept("Error: can't open file");
    std::string line;
    std::string delimiter = ",";
    while (std::getline(wallet_db, line))
    {
        std::string date = line.substr(0, line.find(delimiter));
        IgnoreWhitespace(date);
        std::string value = line.substr(line.find(delimiter) + 1, line.length());
        IgnoreWhitespace(value);
        if(line_number == 0 && date == "date" && value == "exchange_rate")
        {
            line_number++;
            continue;
        }
        Date date_struct;
        std::stringstream ss;
        ss << date.substr(0, 4);
        ss >> date_struct.year;
        ss.clear();
        ss << date.substr(5, 2);
        ss >> date_struct.month;
        ss.clear();
        ss << date.substr(8, 2);
        ss >> date_struct.day;
        ss.clear();
        ss << value;
        double value_d;
        ss >> value_d;
        wallet_data[date_struct] = value_d;
    }
}