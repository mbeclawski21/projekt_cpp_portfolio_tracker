#ifndef ASSET_H
#define ASSET_H

#include <string>

class Asset {
protected:
    std::string name;
    std::string symbol;
    double currentPrice;

public:
    Asset(std::string n, std::string s, double p) 
        : name(n), symbol(s), currentPrice(p) {}

    virtual ~Asset() {}

    std::string getName() const { return name; }
    std::string getSymbol() const { return symbol; }
    double getPrice() const { return currentPrice; }

    virtual void updatePrice(double newPrice) {
        currentPrice = newPrice;
    }
};

#endif