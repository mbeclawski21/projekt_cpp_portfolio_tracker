#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <vector>
#include <memory>
#include <algorithm>
#include "Asset.h"

class Portfolio {
private:
    std::vector<std::unique_ptr<Asset>> assets;

public:
    void addAsset(std::unique_ptr<Asset> asset) {
        assets.push_back(std::move(asset));
    }

    void removeAsset(const std::string& symbol) {
        auto it = std::remove_if(assets.begin(), assets.end(),
            [&symbol](const std::unique_ptr<Asset>& asset) {
                return asset->getSymbol() == symbol;
            });

        if (it != assets.end()) {
            assets.erase(it, assets.end());
        }
    }

    double calculateTotalValue() const {
        double total = 0.0;
        for (const auto& asset : assets) {
            total += asset->getPrice(); 
        }
        return total;
    }

    const std::vector<std::unique_ptr<Asset>>& getAssets() const {
        return assets;
    }
};

#endif