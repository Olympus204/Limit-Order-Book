#include "command_parser.hpp"

#include <string>
#include <stdexcept>
#include <sstream>

Side parse_side(const std::string& text)
{
    if (text == "BUY")
    {
        return Side::buy;
    }
    else if (text ==  "SELL")
    {
        return Side::sell;
    }
    throw std::invalid_argument{"INVALID SIDE: " + text};
    
}

Order parse_add_command(const std::string& line)
{
    std::stringstream input{line};
    std::string command{};
    int id{};
    std::string side{};
    int price{};
    int quantity{};
    std::string extra{};
    if (!(input >> command >> id >> side >> price >> quantity))
    {
        throw std::invalid_argument{"INVALID COMMAND"};
    }
    if (input >> extra)
    {
        throw std::invalid_argument{"EXTRA VARIABLE: " + extra};
    }
    if (command != "ADD" or id <= 0 or price <= 0 or quantity <= 0)
    {
        throw std::invalid_argument{"INVALID ARGUMENTS"};        
    }
    return {id, parse_side(side), price, quantity};
}

int parse_cancel_command(const std::string& line)
{
    std::stringstream input{line};
    std::string command{};
    int id{};
    std::string extra{};
    if (!(input >> command >> id))
    {
        throw std::invalid_argument{"INVALID COMMAND"};
    }
    if (input >> extra)
    {
        throw std::invalid_argument{"EXTRA VARIABLE: " + extra};
    }
    if (command != "CANCEL" or id <= 0)
    {
        throw std::invalid_argument{"INVALID ARGUMENTS"};
    }
    return id;
}