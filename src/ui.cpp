#include "ui.h"
#include "imgui.h"
#include "implot.h"
#include "Crypto.h"
#include <string>
#include <iostream>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

static bool showChartWindow = false;
static std::string activeChartSymbol = "";

static char inputCoinId[64] = "";
static char inputSymbol[16] = "";
static char inputNetwork[32] = "";

void RenderUI(Portfolio& myPortfolio) {
    ImGui::Begin("Mój Portfel Inwestycyjny");
    
    ImGui::Text("Całkowita wartość: $%.2f", myPortfolio.calculateTotalValue());
    ImGui::Separator();

    if (ImGui::BeginTable("TabelaAktywow", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Symbol");
        ImGui::TableSetupColumn("Nazwa");
        ImGui::TableSetupColumn("Cena");
        ImGui::TableSetupColumn("Akcje");
        ImGui::TableHeadersRow();

        for (const auto& asset : myPortfolio.getAssets()) {
            ImGui::TableNextRow();
            
            ImGui::TableSetColumnIndex(0); 
            ImGui::Text("%s", asset->getSymbol().c_str());
            
            ImGui::TableSetColumnIndex(1); 
            ImGui::Text("%s", asset->getName().c_str());
            
            ImGui::TableSetColumnIndex(2); 
            ImGui::Text("$%.2f", asset->getPrice());

            ImGui::TableSetColumnIndex(3);
            std::string buttonLabel = "Wykres##" + asset->getSymbol();
            
            if (ImGui::Button(buttonLabel.c_str())) {
                showChartWindow = true;
                activeChartSymbol = asset->getSymbol();
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();

    ImGui::Begin("Dodaj Krypto (CoinGecko API)");
    
    ImGui::InputText("ID (np. ethereum)", inputCoinId, IM_ARRAYSIZE(inputCoinId));
    ImGui::InputText("Symbol (np. ETH)", inputSymbol, IM_ARRAYSIZE(inputSymbol));
    ImGui::InputText("Sieć (np. ERC20)", inputNetwork, IM_ARRAYSIZE(inputNetwork));

    if (ImGui::Button("Pobierz cenę i dodaj")) {
        if (strlen(inputCoinId) > 0 && strlen(inputSymbol) > 0) {
            std::string coinId(inputCoinId);
            std::string symbol(inputSymbol);
            std::string network(inputNetwork);

            std::string url = "https://api.coingecko.com/api/v3/simple/price?ids=" + coinId + "&vs_currencies=usd";
            cpr::Response r = cpr::Get(cpr::Url{url});

            double livePrice = 0.0;

            if (r.status_code == 200) {
                json data = json::parse(r.text);
                
                if (data.contains(coinId)) {
                    livePrice = data[coinId]["usd"];
                    myPortfolio.addAsset(std::make_unique<Crypto>(coinId, symbol, livePrice, network));
                    
                    inputCoinId[0] = '\0';
                    inputSymbol[0] = '\0';
                    inputNetwork[0] = '\0';
                } else {
                    std::cerr << "Nie znaleziono w API ceny dla: " << coinId << "\n";
                }
            } else {
                std::cerr << "Błąd połączenia z API. Kod: " << r.status_code << "\n";
            }
        }
    }
    ImGui::End();

    if (showChartWindow) {
        ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_Appearing);
        
        std::string windowTitle = "Historia Ceny - " + activeChartSymbol;
        if (ImGui::Begin(windowTitle.c_str(), &showChartWindow)) {
            if (ImPlot::BeginPlot("Ostatnie 5 dni")) {
                static double dni[5] = {1, 2, 3, 4, 5};
                static double cena[5] = {60000, 61500, 60200, 63000, 64200};
                
                std::string plotLabel = activeChartSymbol + "/USD";
                ImPlot::PlotLine(plotLabel.c_str(), dni, cena, 5);
                ImPlot::EndPlot();
            }
        }
        ImGui::End();
    }
}