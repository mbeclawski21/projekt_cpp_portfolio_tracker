#ifndef STOCK_H
#define STOCK_H

#include "Asset.h"

class Stock : public Asset {
    std::string stockExchange;
public:
    Stock(std::string n, std::string s, double p, std::string ex) 
        : Asset(n, s, p), stockExchange(ex) {}

    std::string getExchange() const { return stockExchange; }
};

#endif