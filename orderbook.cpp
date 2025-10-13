#include "orderbook.h"

#include <iostream>
#include <nlohmann/json.hpp>

using JSON = nlohmann::json;

Orderbook::Orderbook(const std::string& filename): file(filename){
    if (!this->file.is_open())throw std::runtime_error("Cannot open file");
}

void Orderbook::loadFromJSON(){
    JSON j_obj;
    this->file >> j_obj; // j_obj ontains all JSON file content

    for(const auto &bid : j_obj["bids"]){
        this->bids.push_back({bid[0], bid[1]});
    }

    for(const auto &ask : j_obj["asks"]){
        this->asks.push_back({ask[0], ask[1]});
    }
}

// >> : read from stream into variable
// << : write variable into stream

// std::cout << something;
// send something to the console

// file << something;
// send something to the file

// file >> something;
// read from the file into your variable

// std::cin >> something;
// read from console into the variable
