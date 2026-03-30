#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include "Asset.h"

class Portfolio {
private:
    std::vector<std::unique_ptr<Asset>> assets;

public:
    //dodawanie aktywa
    void addAsset(std::unique_ptr<Asset> asset) {
        assets.push_back(std::move(asset));
    }

    //Usuwanie aktywa po symbolu (np. "BTC")
    void removeAsset(const std::string& symbol) {
        auto it = std::remove_if(assets.begin(), assets.end(),
            [&symbol](const std::unique_ptr<Asset>& asset) {
                return asset->getSymbol() == symbol;
            });

        if (it != assets.end()) {
            assets.erase(it, assets.end());
            std::cout << "Usunieto " << symbol << " z portfela.\n";
        } else {
            std::cout << "Nie znaleziono aktywa o symbolu " << symbol << ".\n";
        }
    }

    //Zliczanie całkowitej wartości
    double calculateTotalValue() const {
        double total = 0.0;
        for (const auto& asset : assets) {
            total += asset->getPrice(); 
        }
        return total;
    }

    // Wyświetlanie
    void displayPortfolio() const {
        std::cout << "\n=== Twoj Portfel Inwestycyjny ===\n";
        if (assets.empty()) {
            std::cout << "Portfel jest pusty. Dodaj jakies aktywa!\n";
            std::cout << "=================================\n";
            return;
        }

        for (const auto& asset : assets) {
            asset->displayInfo();
        }
        
        std::cout << "---------------------------------\n";
        std::cout << "Calkowita wartosc portfela: $" << calculateTotalValue() << "\n";
        std::cout << "=================================\n";
    }
};

#endif