# Limit Order Book

A C++20 limit order book built as a learning project while developing my C++ skills.

I chose a limit order book because it requires more than basic syntax: it involves
data-structure design, price-time priority, state management, parsing, testing,
and careful handling of partial fills and cancellations.

The implementation uses ordered maps for price levels, FIFO queues within each
price level, and a secondary active-order index to narrow cancellation searches.
Prices are stored as integer pence rather than floating-point values.

## What I learned

This project was built while learning C++, and was used to practise:

- separating interfaces and implementations across headers and source files;
- choosing STL containers based on access and ordering requirements;
- maintaining invariants across multiple data structures;
- using iterators, references, maps, deques and unordered maps;
- designing command parsing and validation;
- writing automated behavioural tests;
- building a multi-file project with CMake.
