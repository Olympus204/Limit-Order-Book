#include "order_book.hpp"
#include "order.hpp"
#include "trades.hpp"

#include <cassert>
#include <iostream>
#include <vector>

void run_test(const char* name, void (*test)())
{
    std::cout << "Running " << name << "... ";
    test();
    std::cout << "passed\n";
}

void test_multi_fill()
{
    Order_book book;

    std::vector<Order> orders{
        {1, Side::sell, 10000, 50},
        {2, Side::sell, 10100, 100},
        {3, Side::buy,  10200, 120}
    };

    for (const Order& order : orders)
    {
        book.process_order(order);
    }

    assert(book.get_trades().size() == 2);

    assert(book.get_trades()[0].buy_order_id == 3);
    assert(book.get_trades()[0].sell_order_id == 1);
    assert(book.get_trades()[0].price == 10000);
    assert(book.get_trades()[0].quantity == 50);

    assert(book.get_trades()[1].buy_order_id == 3);
    assert(book.get_trades()[1].sell_order_id == 2);
    assert(book.get_trades()[1].price == 10100);
    assert(book.get_trades()[1].quantity == 70);

    assert(book.get_buys().empty());

    assert(book.get_sells().size() == 1);
    assert(book.get_sells().begin()->second.size() == 1);
    assert(book.get_sells().begin()->second.front().id == 2);
    assert(book.get_sells().begin()->first == 10100);
    assert(book.get_sells().begin()->second.front().quantity == 30);
}

void test_fifo()
{
    Order_book book;
    std::vector<Order> orders{
        {1, Side::sell, 10000, 30},
        {2, Side::sell, 10000, 40},
        {3, Side::sell, 10000, 50},
        {4, Side::buy, 10000, 60}
    };
    for (const Order& order : orders)
    {
        book.process_order(order);
    }

    assert(book.get_trades().size() == 2);

    assert(book.get_trades()[0].buy_order_id == 4);
    assert(book.get_trades()[0].sell_order_id == 1);
    assert(book.get_trades()[0].quantity == 30);
    assert(book.get_trades()[0].price == 10000);

    assert(book.get_trades()[1].buy_order_id == 4);
    assert(book.get_trades()[1].sell_order_id == 2);
    assert(book.get_trades()[1].quantity == 30);
    assert(book.get_trades()[1].price == 10000);

    assert(book.get_sells().size() == 1);
    assert(book.get_sells().begin()->first == 10000);
    assert(book.get_sells().begin()->second.size() == 2);
    assert(book.get_sells().begin()->second.front().id == 2);
    assert(book.get_sells().begin()->second.front().quantity == 10);
    assert(book.get_sells().begin()->second.back().id == 3);
    assert(book.get_sells().begin()->second.back().quantity == 50);

    assert(book.get_buys().empty());
}

void test_no_cross()
{
    Order_book book;
    std::vector<Order> orders{
        {1, Side::buy,  9900, 100},
        {2, Side::sell, 10000, 100}
    };
    for (const Order& order : orders)
    {
        book.process_order(order);
    }

    assert(book.get_trades().empty());

    assert(book.get_buys().size() == 1);
    assert(book.get_sells().size() == 1);

    assert(book.get_buys().begin()->first == 9900);
    assert(book.get_sells().begin()->first == 10000);

    assert(book.get_buys().begin()->second.front().id == 1);
    assert(book.get_buys().begin()->second.front().quantity == 100);
    
    assert(book.get_sells().begin()->second.front().id == 2);
    assert(book.get_sells().begin()->second.front().quantity == 100);
}

void test_cancel_active_buy()
{
    Order_book book;
    book.process_order({1, Side::buy, 9900, 100});
    assert(book.cancel_order(1));
    assert(book.get_buys().empty());
}

void test_cancel_active_sell()
{
    Order_book book;
    book.process_order({1, Side::sell, 10100, 75});
    assert(book.cancel_order(1));
    assert(book.get_sells().empty());
}

void test_cancel_shared_price()
{
    Order_book book;

    std::vector<Order> orders{
        {1, Side::sell, 10000, 30},
        {2, Side::sell, 10000, 40},
        {3, Side::sell, 10000, 50}
    };
    for (const Order& order : orders)
    {
        book.process_order(order);
    }
    assert(book.cancel_order(2));
    assert(book.get_sells().size() == 1);
    assert(book.get_sells().begin()->first == 10000);
    assert(book.get_sells().begin()->second.size() == 2);
    assert(book.get_sells().begin()->second.front().id == 1);
    assert(book.get_sells().begin()->second.back().id == 3);
}

void test_cancel_only_at_price()
{
    Order_book book;
    std::vector<Order> orders{
        {1, Side::sell, 10000, 30},
        {2, Side::sell, 10100, 40}
    };
    for (const Order& order : orders)
    {
        book.process_order(order);
    }
    assert(book.cancel_order(1));
    assert(book.get_sells().size() == 1);
    assert(book.get_sells().begin()->first == 10100);
    assert(book.get_sells().begin()->second.front().id == 2);
}

void test_cancel_wrong_id()
{
    Order_book book;
    book.process_order({1, Side::buy, 9900, 100});

    assert(!book.cancel_order(999));
    assert(book.get_buys().begin()->second.front().id == 1);
    assert(book.get_buys().begin()->first == 9900);
}

void test_cancel_after_complete()
{
    Order_book book;
    std::vector<Order> orders{
        {1, Side::sell, 10000, 50},
        {2, Side::buy, 10000, 50}
    };
    for (const Order& order : orders)
    {
        book.process_order(order);
    }

    assert(!book.cancel_order(1));
    assert(book.get_trades().size() == 1);
    assert(book.get_trades()[0].quantity == 50);
    assert(book.get_buys().empty());
    assert(book.get_sells().empty());
}

void test_cancel_twice()
{
    Order_book book;
    book.process_order({1, Side::sell, 10000, 50});

    assert(book.cancel_order(1));
    assert(!book.cancel_order(1));
    assert(book.get_sells().empty());
}

void test_reuse_id_after_cancel()
{
    Order_book book;

    book.process_order({1, Side::buy, 9900, 100});
    assert(book.cancel_order(1));

    bool threw{false};

    try
    {
        book.process_order({1, Side::sell, 10100, 20});
    }
    catch (const std::invalid_argument&)
    {
        threw = true;
    }

    assert(threw);
    assert(book.get_buys().empty());
    assert(book.get_sells().empty());
}

void test_reuse_id_after_fill()
{
    Order_book book;

    book.process_order({1, Side::sell, 10000, 50});
    book.process_order({2, Side::buy, 10000, 50});

    bool threw{false};

    try
    {
        book.process_order({1, Side::sell, 10100, 20});
    }
    catch (const std::invalid_argument&)
    {
        threw = true;
    }

    assert(threw);
    assert(book.get_buys().empty());
    assert(book.get_sells().empty());
}

int main()
{
    run_test("multi fill", test_multi_fill);
    run_test("FIFO", test_fifo);
    run_test("no cross", test_no_cross);
    run_test("cancel active buy", test_cancel_active_buy);
    run_test("cancel active sell", test_cancel_active_sell);
    run_test("cancel at shared price", test_cancel_shared_price);
    run_test("cancel only at price", test_cancel_only_at_price);
    run_test("cancel the wrong id", test_cancel_wrong_id);
    run_test("cancel after trade is complete", test_cancel_after_complete);
    run_test("cancel twice", test_cancel_twice);
    run_test("reuse id after cancel", test_reuse_id_after_cancel);
    run_test("reuse id after trade is complete", test_reuse_id_after_fill);
    std::cout << "\nAll tests passed.\n";
}