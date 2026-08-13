#pragma once
#include <vector>
#include "order.hpp"

struct Trade
{
    int buy_order_id;
    int sell_order_id;
    int price;
    int quantity;
};

void print_trades(const std::vector<Trade>& trades);
Trade execute_trade(Order& buy, Order& sell, int amount, Side incoming_side);