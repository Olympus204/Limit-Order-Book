#pragma once
#include <vector>

enum class Side
{
    buy,
    sell
};

struct Order
{
    int id;
    Side side;
    int price;
    int quantity;
};