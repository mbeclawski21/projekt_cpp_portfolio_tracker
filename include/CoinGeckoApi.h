#ifndef COINGECKO_API_H
#define COINGECKO_API_H

#include <string>
#include <vector>
#include "Portfolio.h"

namespace CoinGeckoApi {
    extern std::vector<double> chartPrices;
    extern std::vector<double> chartDays;

    std::string ToLower(std::string s);
    void RefreshAllPrices(Portfolio& myPortfolio);
    void FetchHistoricalData(const std::string& coinId);
    bool AddCryptoWithLivePrice(Portfolio& myPortfolio, const std::string& coinId, const std::string& symbol, double amount, double purchasePrice);
}

#endif