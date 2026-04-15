#include "ui.h"
#include "imgui.h"
#include "implot.h"
#include "Crypto.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

static bool showChartWindow = false;
static std::string activeChartSymbol = "";
static std::string activeChartId = "";
static std::vector<double> chartPrices;
static std::vector<double> chartDays;

static char inputCoinId[64] = "";
static char inputSymbol[16] = "";
static float inputAmount = 1.0f;

std::string ToLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
    s.erase(std::remove_if(s.begin(), s.end(), isspace), s.end());
    return s;
}

void FetchHistoricalData(std::string coinId) {
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

void RenderUI(Portfolio& myPortfolio) {
    ImGui::Begin("Moj Portfel Inwestycyjny", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    
    ImGui::Text("Calkowita wartosc portfela: $%.2f", myPortfolio.calculateTotalValue());
    ImGui::Separator();

    if (ImGui::BeginTable("TabelaAktywow", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {
        ImGui::TableSetupColumn("Symbol");
        ImGui::TableSetupColumn("Nazwa");
        ImGui::TableSetupColumn("Ilosc");
        ImGui::TableSetupColumn("Cena");
        ImGui::TableSetupColumn("Akcje");
        ImGui::TableHeadersRow();

        int n = 0;
        for (const auto& asset : myPortfolio.getAssets()) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("%s", asset->getSymbol().c_str());
            ImGui::TableSetColumnIndex(1); ImGui::Text("%s", asset->getName().c_str());
            ImGui::TableSetColumnIndex(2); ImGui::Text("%.4f", asset->getAmount());
            ImGui::TableSetColumnIndex(3); ImGui::Text("$%.2f", asset->getPrice());
            ImGui::TableSetColumnIndex(4);
            
            std::string buttonLabel = "Wykres##" + std::to_string(n++);
            if (ImGui::Button(buttonLabel.c_str())) {
                showChartWindow = true;
                activeChartSymbol = asset->getSymbol();
                activeChartId = asset->getName();
                FetchHistoricalData(activeChartId);
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();

    ImGui::Begin("Dodaj Krypto (CoinGecko API)", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::InputText("ID (np. bitcoin)", inputCoinId, IM_ARRAYSIZE(inputCoinId));
    ImGui::InputText("Symbol (np. BTC)", inputSymbol, IM_ARRAYSIZE(inputSymbol));
    ImGui::InputFloat("Ilosc", &inputAmount);

    if (ImGui::Button("Pobierz cene i dodaj")) {
        if (strlen(inputCoinId) > 0 && strlen(inputSymbol) > 0) {
            std::string coinId = ToLower(std::string(inputCoinId));
            std::string symbol = std::string(inputSymbol);
            std::string url = "https://api.coingecko.com/api/v3/simple/price?ids=" + coinId + "&vs_currencies=usd";
            cpr::Response r = cpr::Get(cpr::Url{url});
            if (r.status_code == 200) {
                json data = json::parse(r.text);
                if (data.contains(coinId)) {
                    double livePrice = data[coinId]["usd"];
                    myPortfolio.addAsset(std::make_unique<Crypto>(coinId, symbol, livePrice, (double)inputAmount));
                    inputCoinId[0] = '\0';
                    inputSymbol[0] = '\0';
                    inputAmount = 1.0f;
                }
            }
        }
    }
    ImGui::End();

    if (showChartWindow) {
        ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Appearing);
        std::string windowTitle = "Historia Ceny (7d) - " + activeChartSymbol;
        if (ImGui::Begin(windowTitle.c_str(), &showChartWindow)) {
            if (!chartPrices.empty()) {
                if (ImPlot::BeginPlot("Kurs ostatnie 7 dni")) {
                    ImPlot::SetupAxes("Dzien", "USD");
                    ImPlot::PlotLine(activeChartSymbol.c_str(), chartDays.data(), chartPrices.data(), (int)chartPrices.size());
                    ImPlot::EndPlot();
                }
            } else {
                ImGui::Text("Pobieranie danych...");
            }
        }
        ImGui::End();
    }
}