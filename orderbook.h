#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <string>
#include "order.h"
#include <fstream>

class Orderbook
{
    std::ifstream file;
public:
    std::vector<Order> bids;
    std::vector<Order> asks;

    Orderbook(const std::string& filename);
    void loadFromJSON();
};

#endif // ORDERBOOK_H
