#include "CoinGeckoApi.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <cctype>

using json = nlohmann::json;

namespace CoinGeckoApi {
    std::vector<double> chartPrices;
    std::vector<double> chartDays;

    std::string ToLower(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
        s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
        return s;
    }

    void RefreshAllPrices(Portfolio& myPortfolio) {
        auto& assets = myPortfolio.getAssets();
        if (assets.empty()) return;

        std::string ids = "";
        for (size_t i = 0; i < assets.size(); ++i) {
            ids += assets[i].getName();
            if (i < assets.size() - 1) ids += ",";
        }

        std::string url = "https://api.coingecko.com/api/v3/simple/price?ids=" + ids + "&vs_currencies=usd";
        cpr::Response r = cpr::Get(cpr::Url{url});

        if (r.status_code == 200) {
            json data = json::parse(r.text);
            for (auto& asset : assets) {
                std::string id = asset.getName();
                if (data.contains(id)) {
                    asset.updatePrice(data[id]["usd"]);
                }
            }
        }
    }

    void FetchHistoricalData(const std::string& coinId) {
        chartPrices.clear();
        chartDays.clear();
        std::string cleanId = ToLower(coinId);
        std::string url = "https://api.coingecko.com/api/v3/coins/" + cleanId + "/market_chart?vs_currency=usd&days=7&interval=daily";
        cpr::Response r = cpr::Get(cpr::Url{url});
        if (r.status_code == 200) {
            json data = json::parse(r.text);
            if (data.contains("prices")) {
                int dayCounter = 1;
                for (auto& entry : data["prices"]) {
                    chartPrices.push_back(entry[1].get<double>());
                    chartDays.push_back((double)dayCounter++);
                }
            }
        }
    }

    bool AddCryptoWithLivePrice(Portfolio& myPortfolio, const std::string& coinId, const std::string& symbol, double amount, double purchasePrice) {
        std::string cleanId = ToLower(coinId);
        std::string url = "https://api.coingecko.com/api/v3/simple/price?ids=" + cleanId + "&vs_currencies=usd";
        cpr::Response r = cpr::Get(cpr::Url{url});
        
        if (r.status_code == 200) {
            json data = json::parse(r.text);
            if (data.contains(cleanId)) {
                double livePrice = data[cleanId]["usd"];
                bool found = false;
                auto& refAssets = myPortfolio.getAssets();
                for (auto& asset : refAssets) {
                    if (asset.getSymbol() == symbol) {
                        double oldAmount = asset.getAmount();
                        double oldPrice = asset.getPurchasePrice();
                        double newAmount = oldAmount + amount;
                        double newAvgPrice = ((oldAmount * oldPrice) + (amount * purchasePrice)) / newAmount;
                        asset.setAmount(newAmount);
                        asset.setPurchasePrice(newAvgPrice);
                        asset.setCurrentPrice(livePrice);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    myPortfolio.addAsset(Crypto(cleanId, symbol, livePrice, amount, purchasePrice));
                }
                return true;
            }
        }
        return false;
    }
}