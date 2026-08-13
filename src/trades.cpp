#include "trades.hpp"
#include "order.hpp"
#include <vector>
#include <iostream>

void print_trades(const std::vector<Trade>& trades)
{
    for (const Trade& trade : trades)
    {
        std::cout << "BUY #" << trade.buy_order_id << " / SELL #" << trade.sell_order_id << '\n';
        std::cout << "Quantity: " << trade.quantity << '\n';
        std::cout << "Price: " << trade.price << "p" << '\n';
        std::cout << '\n';
    }
}

Trade execute_trade(Order& buy, Order& sell, int amount, Side incoming_side)
{
    buy.quantity -= amount;
    sell.quantity -= amount;
    int price;
    if (incoming_side == Side::buy)
    {
        price = sell.price;
    }
    else
    {
        price = buy.price;
    }

    Trade record{buy.id, sell.id, price, amount};
    return record;
}
