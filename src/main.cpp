#include "command_parser.hpp"
#include "order_book.hpp"
#include "trades.hpp"

#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

int main()
{
    Order_book book;

    std::string line;

    while (std::getline(std::cin, line))
    {
        std::stringstream input{line};

        std::string command;
        input >> command;
        try
        {
            if (command == "ADD")
            {
                book.process_order(parse_add_command(line));
            }
            else if (command == "TOP")
            {
                book.print_top();
            }
            else if (command == "PRINT")
            {
                book.print_orders();
            }
            else if (command == "TRADES")
            {
                print_trades(book.get_trades());
            }
            else if (command == "CANCEL")
            {
                book.cancel_order(parse_cancel_command(line));
            }
            else if (command == "QUIT")
            {
                break;
            }
            else
            {
                std::cout << "ERROR: UNKNOWN COMMAND: " << command << '\n';
            }
        }
        catch (const std::invalid_argument& error)
        {
            std::cout << "ERROR: " << error.what() << '\n';
        }
    }
}
