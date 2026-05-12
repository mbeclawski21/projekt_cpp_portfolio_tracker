#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <vector>
#include <algorithm>
#include <string>
#include "Crypto.h"

class Portfolio {
private:
    std::vector<Crypto> assets;

public:
    void addAsset(const Crypto& asset) {
        assets.push_back(asset);
    }

    void removeAsset(const std::string& symbol) {
        auto it = std::remove_if(assets.begin(), assets.end(),
            [&symbol](const Crypto& asset) {
                return asset.getSymbol() == symbol;
            });

        if (it != assets.end()) {
            assets.erase(it, assets.end());
        }
    }

    void removeAssetByIndex(size_t index) {
        if (index < assets.size()) {
            assets.erase(assets.begin() + index);
        }
    }

    void updateAsset(size_t index, double newAmount, double newPurchasePrice) {
        if (index < assets.size()) {
            assets[index].setAmount(newAmount);
            assets[index].setPurchasePrice(newPurchasePrice);
        }
    }

    double calculateTotalValue() const {
        double total = 0.0;
        for (const auto& asset : assets) {
            total += asset.getTotalValue(); 
        }
        return total;
    }

    std::vector<Crypto>& getAssets() { return assets; }
    const std::vector<Crypto>& getAssets() const { return assets; }
};

#endif