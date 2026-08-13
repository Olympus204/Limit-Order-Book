#pragma once

#include <string>

#include "order.hpp"

Side parse_side(const std::string& text);
Order parse_add_command(const std::string& line);
int parse_cancel_command(const std::string& line);