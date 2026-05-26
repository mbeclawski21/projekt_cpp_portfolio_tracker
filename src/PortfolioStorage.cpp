#include "PortfolioStorage.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace PortfolioStorage {
    void SavePortfolio(const Portfolio& p) {
        json j = json::array();
        for (const auto& a : p.getAssets()) {
            j.push_back({
                {"id", a.getName()},
                {"symbol", a.getSymbol()},
                {"amount", a.getAmount()},
                {"purchasePrice", a.getPurchasePrice()},
                {"currentPrice", a.getPrice()}
            });
        }
        std::ofstream file("portfolio.json");
        file << j.dump(4);
    }

    void LoadPortfolio(Portfolio& p) {
        std::ifstream file("portfolio.json");
        if (!file.is_open()) return;
        json j;
        file >> j;
        for (const auto& item : j) {
            p.addAsset(Crypto(
                item["id"], item["symbol"], item["currentPrice"], item["amount"], item["purchasePrice"]
            ));
        }
    }
}