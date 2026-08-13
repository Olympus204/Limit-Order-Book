#pragma once

#include "order.hpp"
#include "trades.hpp"

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <map>
#include <deque>
#include <unordered_map>
#include <functional>


struct OrderLocation
{
    Side side;
    int price;
};


using PriceLevel = std::deque<Order>;

using BuyBook =
    std::map<int, PriceLevel, std::greater<int>>;

using SellBook =
    std::map<int, PriceLevel>;


struct Order_book
{
public:
    void process_order(Order incoming);
    bool cancel_order(int order_id);
    void print_orders() const;
    void print_top() const;

    const BuyBook& get_buys() const;
    const SellBook& get_sells() const;
    const std::vector<Trade>& get_trades() const;

private:
    BuyBook buys;
    SellBook sells;
    std::vector<Trade> trades;
    std::vector<int> used_ids;
    std::unordered_map<int, OrderLocation> active_orders;
};


int quantity_at_price_level(const PriceLevel& orders);
