# Portfolio Tracker
## Opis Projektu
Portfolio tracker to aplikacja służąca do śledzenia i zarządzania portfelem inwestycyjnym. System pozwala na monitorowanie różnych aktywów, wyliczenie całkowitej wartości portfela, zysków oraz strat.

## Tydzien 1 - Wprowadzanie danych: Użytkownik może ręcznie dodać do portfela dwa rodzaje aktywów: Akcje (Stock) oraz Kryptowaluty (Crypto). 
* Rozróżnienie typów: System rozróżnia dane specyficzne dla typu - dla akcji podajemy nazwę giełdy, dla krypto nazwę sieci (np. ERC20).
* Możliwość wyświetlenia listy wszystkich posiadanych walorów w formie czytelnego raportu w konsoli.
* Interfejs sterowania: Implementacja menu tekstowego pozwalającego na nawigację między funkcjami (Dodaj/Pokaż/Wyjdź).
* Program pozwala na dodawanie dowolnej liczby obiektów w trakcie działania
* Integracja z Live API (CoinGecko): Program automatycznie pobiera aktualne kursy kryptowalut z internetu. Użytkownik nie musi już ręcznie wpisywać cen.

## Tydzień 3 - Migracja do GUI i Rozbudowa Logiki Portfela.

* **Nowy silnik graficzny:** Porzucenie standardowego wejścia/wyjścia konsolowego (`std::cin`/`std::cout`) na rzecz biblioteki **Dear ImGui**. Aplikacja wykorzystuje nowoczesny interfejs okienkowy.
* **Wizualizacja danych:** Integracja biblioteki **ImPlot**, umożliwiającej rysowanie wykresów cen aktywów.
* **Integracja z API:** Wykorzystanie biblioteki **CPR** do pobierania kursów kryptowalut w czasie rzeczywistym z CoinGecko.
* **Obsługa ilości (Amount):** Dodanie funkcjonalności śledzenia liczby posiadanych jednostek danego aktywa. System automatycznie oblicza wartość każdego elementu oraz sumaryczną wartość całego portfela (Cena * Ilosc).
* **Refaktoryzacja i czystość kodu:** * Usunięcie zbędnych pól (np. "Sieć"), które nie wpływały na wycenę aktywów.
    * Przejście na inteligentne wskaźniki (`std::unique_ptr`) w zarządzaniu obiektami portfela.
    * Optymalizacja interfejsu pod kątem czytelności i kompatybilności (rezygnacja z polskich znaków na rzecz standardowego kodowania ASCII/UTF-8).
* **Automatyzacja CMake:** Pełna konfiguracja `CMakeLists.txt` z modułem `FetchContent`, co pozwala na automatyczne pobieranie wszystkich bibliotek przy pierwszym budowaniu projektu.
* **Dynamiczne wykresy historyczne:** Rozbudowa modułu wizualizacji o pobieranie realnych danych rynkowych z ostatnich 7 dni (endpoint `/market_chart`). Dane są pobierane asynchronicznie po kliknięciu przycisku, co zapewnia płynność działania interfejsu.
* **Automatyczne skalowanie okien:** Zastosowanie flagi `ImGuiWindowFlags_AlwaysAutoResize`, dzięki której okna aplikacji dynamicznie dopasowują swój rozmiar do liczby dodanych elementów, eliminując potrzebę ręcznego przewijania tabeli.
* **Walidacja i czyszczenie danych:** Implementacja funkcji automatycznie formatującej wpisy użytkownika (konwersja na małe litery, usuwanie zbędnych spacji), co zapobiega błędom `404` przy zapytaniach do API.
* **Unikalność interfejsu:** Rozwiązanie problemu konfliktów identyfikatorów w Dear ImGui poprzez generowanie unikalnych ID dla przycisków w tabeli, co pozwala na niezależne otwieranie wykresów dla każdego aktywa w portfelu.
* Implementacja systemu zapisu i odczytu stanu portfela przy użyciu biblioteki **nlohmann/json**. Wszystkie dane są automatycznie utrwalane w pliku `portfolio.json`, co pozwala na zachowanie historii inwestycji po restarcie aplikacji.
* **Tracking zysków (P/L):** Wprowadzenie logiki obliczania zysku/straty (Profit/Loss) dla każdego aktywa na podstawie wprowadzonej ceny zakupu. Wyniki są prezentowane kwotowo ($) oraz procentowo (%), z dynamiczną sygnalizacją kolorystyczną (zielony/czerwony).
* **Analityka sumaryczna portfela:** Dodanie globalnego licznika w nagłówku aplikacji, który w czasie rzeczywistym agreguje dane ze wszystkich aktywów, pokazując całkowitą wartość kapitału oraz łączny zysk/stratę całego portfela.
* **Synchronizacja i automatyczne odświeżanie:** * Przycisk ręcznej synchronizacji, pozwalający na natychmiastowe pobranie najświeższych kursów dla wszystkich posiadanych kryptowalut naraz (optymalizacja zapytań API).
    * System automatycznego odświeżania danych w tle co 5 minut.
* **Statystyki ekstremów cenowych:** Rozbudowa okna wykresów o analizę danych historycznych - wyznaczanie i wyświetlanie najniższej (MIN) oraz najwyższej (MAX) ceny z ostatniego tygodnia.

## Tydzień 5 - Zarządzanie Aktywami i Optymalizacja Interfejsu.

* **Zarządzanie aktywami (Usuwanie):** Implementacja funkcjonalności trwałego usuwania konkretnych pozycji z portfela.
* **Modyfikacja danych w czasie rzeczywistym (Edycja):** Dodanie interaktywnego okna wyskakującego z poziomu tabeli, pozwalającego na dynamiczną aktualizację posiadanej ilości aktywa oraz jego ceny zakupu. Zmiany natychmiastowo aktualizują kalkulacje zysków/strat i są automatycznie zapisywane w pliku JSON.
* **Sztywny układ i optymalizacja UX :** Przebudowa interfejsu graficznego poprzez zablokowanie możliwości przesuwania, zwijania i skalowania okien (`ImGuiWindowFlags_NoMove` | `NoResize` | `NoCollapse`). Zapewnia to spójny wygląd aplikacji niezależnie od działań użytkownika.
* **Poprawki wizualne formularzy:** Zastosowanie dyrektywy `ImGui::PushItemWidth` do precyzyjnego skalowania pól wejściowych (InputBox), co wyeliminowało błędy renderowania (ucinanie etykiet tekstowych) oraz nakładanie się na siebie elementów interfejsu.
* **Szybkie dodawanie aktywów (Quick Select):** Zaimplementowano interaktywne okno wyboru (Popup Modal) z funkcją wewnętrznego przewijania, zawierające listę najpopularniejszych kryptowalut. Wybór waluty z listy automatycznie uzupełnia pola formularza (ID oraz Symbol), co przyspiesza proces dodawania i eliminuje ryzyko literówki podczas zapytań do API.
* **Rozszerzenie bazy danych aktywów (Top 50):** Znaczne rozszerzenie listy szybkiego wyboru o 50 najpopularniejszych kryptowalut na rynku (zgodnie z aktualnym rankingiem CoinGecko). Każdy wpis został zoptymalizowany pod kątem unikalnych identyfikatorów API (np. `avalanche-2`, `binancecoin`), co zapewnia 100% skuteczności przy automatycznym pobieraniu cen dla szerokiego spektrum altcoinów.
* **Inteligentne łączenie aktywów:** System automatycznie rozpoznaje próby dodania waluty, która już znajduje się w portfelu. Zamiast tworzyć duplikaty, aplikacja aktualizuje istniejący wpis, przeliczając nową ilość monet oraz średnią ważoną cenę zakupu.
* **Interaktywna wizualizacja:** Dodano pełną interakcję z wykresem kołowym struktury portfela (zoom, przesuwanie), co pozwala na szczegółową analizę przy dużej liczbie drobnych aktywów.
* **Optymalizacja wyświetlania:** Uproszczono etykiety danych na wykresach do pełnych procentów, eliminując błędy renderowania czcionek pikselowych przy wartościach dziesiętnych.

## Tydzień 7 - Specjalizacja, Optymalizacja i Wielowalutowość.

* **Skupienie na rynku Crypto:** Aplikacja stała się dedykowanym narzędziem do śledzenia wyłącznie kryptowalut.
* **Dynamiczna wielowalutowość (USD/PLN):** Wprowadzono funkcję przeliczania wartości całego portfela w czasie rzeczywistym. Użytkownik może jednym kliknięciem zmienić widok z dolarów na złotówki (wraz z edycją kursu), a aplikacja automatycznie przelicza tabele, podsumowania oraz osie wykresów.
* **Korekta estetyki UI:** Dopracowano siatkę interfejsu graficznego, przywracając idealną symetrię i proporcje okien.

## Tydzień 9 - Refaktoryzacja, Architektura i Czysty Kod.

* **Refaktoryzacja architektury:** Rozbicie monolitycznego pliku interfejsu (`ui.cpp`) na mniejsze, wyspecjalizowane moduły.
* **Wydzielenie warstwy sieciowej:** Przeniesienie całej logiki komunikacji z API CoinGecko (obsługa biblioteki CPR, pobieranie cen i historii) do niezależnego modułu `CoinGeckoApi`. Interfejs graficzny został odizolowany od bezpośrednich zapytań HTTP.
* **Niezależny menedżer danych:** Wyodrębnienie logiki zapisu i odczytu stanu portfela (parsowanie plików JSON) do dedykowanego modułu `PortfolioStorage`.
* **Uporządkowanie komponentów UI:** Podział głównej pętli rysującej interfejs na mniejsze, czytelne sub-komponenty (osobne funkcje renderujące tabele, formularze i wykresy), co drastycznie ułatwia utrzymanie i rozwój kodu.
* **Rozwój systemu budowania:** Dostosowanie pliku `CMakeLists.txt` do nowej, wieloplikowej struktury projektu z podziałem na katalogi `src/` oraz `include/`.