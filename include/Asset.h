#ifndef ASSET_H
#define ASSET_H
#include <string>

class Asset {
protected:
    std::string name;
    std::string symbol;
    double currentPrice;
    double amount;
    double purchasePrice;

public:
    Asset(std::string n, std::string s, double p, double amt, double pp) 
        : name(n), symbol(s), currentPrice(p), amount(amt), purchasePrice(pp) {}

    virtual ~Asset() {}

    std::string getName() const { return name; }
    std::string getSymbol() const { return symbol; }
    double getPrice() const { return currentPrice; }
    double getAmount() const { return amount; }
    double getPurchasePrice() const { return purchasePrice; }

    void setAmount(double amt) { amount = amt; }
    void setPurchasePrice(double pp) { purchasePrice = pp; }
    
    double getTotalValue() const { return currentPrice * amount; }
    double getTotalCost() const { return purchasePrice * amount; }
    double getProfitLoss() const { return getTotalValue() - getTotalCost(); }
    double getProfitLossPercentage() const { 
        if (getTotalCost() == 0) return 0;
        return (getProfitLoss() / getTotalCost()) * 100.0; 
    }

    virtual void updatePrice(double newPrice) { currentPrice = newPrice; }
};
#endif