#ifndef ASSET_H
#define ASSET_H

#include <string>

class Asset {
protected:
    std::string name;
    std::string symbol;
    double currentPrice;
    double amount;

public:
    Asset(std::string n, std::string s, double p, double amt) 
        : name(n), symbol(s), currentPrice(p), amount(amt) {}

    virtual ~Asset() {}

    std::string getName() const { return name; }
    std::string getSymbol() const { return symbol; }
    double getPrice() const { return currentPrice; }
    double getAmount() const { return amount; }
    double getValue() const { return currentPrice * amount; }

    virtual void updatePrice(double newPrice) {
        currentPrice = newPrice;
    }
};

#endif