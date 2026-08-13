#include "Order_Book.hpp"
#include <iostream>

void Order_book::process_order(Order incoming)
{
    if (std::find(used_ids.begin(), used_ids.end(), incoming.id) != used_ids.end())
    {
        throw std::invalid_argument("Order ID already exists");
    }
    used_ids.push_back(incoming.id);
    if (incoming.side == Side::buy)
    {
        while (incoming.quantity > 0 and !sells.empty() and incoming.price >= sells.begin()->first)
        {
            int amount = std::min(incoming.quantity, sells.begin()->second.front().quantity);
            trades.push_back(execute_trade(incoming, sells.begin()->second.front(), amount, incoming.side));
            if (sells.begin()->second.front().quantity == 0)
            {
                active_orders.erase(sells.begin()->second.front().id);
                sells.begin()->second.pop_front();
                if (sells.begin()->second.empty())
                {
                    sells.erase(sells.begin());
                }
            }
        }
        if (incoming.quantity > 0)
        {
            buys[incoming.price].push_back(incoming);
            active_orders[incoming.id] = {incoming.side, incoming.price};
        }
    }
    else
    {
        while (incoming.quantity > 0 and !buys.empty() and incoming.price <= buys.begin()->first)
        {
            int amount = std::min(incoming.quantity, buys.begin()->second.front().quantity);
            trades.push_back(execute_trade(buys.begin()->second.front(), incoming, amount, incoming.side));
            if (buys.begin()->second.front().quantity == 0)
            {
                active_orders.erase(buys.begin()->second.front().id);
                buys.begin()->second.pop_front();
                if (buys.begin()->second.empty())
                {
                    buys.erase(buys.begin());
                }
            }
        }
        if (incoming.quantity > 0)
        {
            sells[incoming.price].push_back(incoming);
            active_orders[incoming.id] = {incoming.side, incoming.price};
        }
    }
}

bool Order_book::cancel_order(int order_id)
{
    auto location_it{active_orders.find(order_id)};
    if (location_it == active_orders.end())
    {
        return false;
    }
    const Side side{location_it->second.side};
    const int price{location_it->second.price};
    if (side == Side::sell)
    {
        auto level_it{sells.find(price)};
        if (level_it != sells.end())
        {
            auto& orders{level_it->second};
            auto order_it{std::find_if(orders.begin(),orders.end(),[order_id](const Order& order){return order.id == order_id;})};
            if (order_it != orders.end())
            {
                active_orders.erase(location_it);
                orders.erase(order_it);
                if (orders.empty())
                {
                    sells.erase(price);
                }
                return true;
            }
        }
    }
    else
    {
        auto level_it{buys.find(price)};
        if (level_it != buys.end())
        {
            auto& orders{level_it->second};
            auto order_it{std::find_if(orders.begin(),orders.end(),[order_id](const Order& order){return order.id == order_id;})};
            if (order_it != orders.end())
            {
                active_orders.erase(location_it);
                orders.erase(order_it);
                if (orders.empty())
                {
                    buys.erase(price);
                }
                return true;
            }
        }
    }
    return false;
}

void Order_book::print_orders() const
{
    std::cout << "BUYS\n";
    for (const auto& [price, orders] : buys)
    {
        for (const auto& order : orders)
        {
            std::cout << "#" << order.id << " " << order.quantity << " " << order.price << "p" << '\n';
        }
    }
    std::cout << '\n';
    std::cout << "SELLS\n";
    for (const auto& [price, orders] : sells)
    {
        for (const auto& order : orders)
        {
            std::cout << "#" << order.id << " " << order.quantity << " " << order.price << "p" << '\n';   
        }
    }
    std::cout << '\n';

}

int quantity_at_price_level(const PriceLevel& orders)
{
    int quantity{0};

    for (const auto& order : orders)
    {
        quantity += order.quantity;
    }

    return quantity;
}

void Order_book::print_top() const
{
    std::cout << "TOP OF BOOK" << '\n';
    std::cout << "BID ";
    if (buys.empty())
    {
        std::cout << "NONE" << '\n';
    }
    else 
    {
        std::cout << buys.begin()->first << "p x " << quantity_at_price_level(buys.begin()->second) << '\n';
    }
    std::cout << "ASK ";
    if (sells.empty())
    {
        std::cout << "NONE" << '\n';
    }
    else
    {
        std::cout << sells.begin()->first << "p x " << quantity_at_price_level(sells.begin()->second) << '\n';
    }
}

const BuyBook& Order_book::get_buys() const
{
    return buys;
}

const SellBook& Order_book::get_sells() const
{
    return sells;
}

const std::vector<Trade>& Order_book::get_trades() const
{
    return trades;
}
