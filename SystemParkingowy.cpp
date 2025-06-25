#include "SystemParkingowy.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

SystemParkingowy* globalnyParking = nullptr;

SystemParkingowy::SystemParkingowy(int iloscMiejsc, double stawka)
    : licznikBiletow(1), stawkaZaGodzine(stawka) {
    for (int i = 1; i <= iloscMiejsc; ++i)
        miejsca.emplace_back(i);
}

bool SystemParkingowy::walidujNrRej(const std::string& nr) {
    if (nr.empty() || nr.size() > 10) return false;
    for (char c : nr) {
        if (!((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')))
            return false;
    }
    return true;
}

int SystemParkingowy::znajdzWolneMiejsce() {
    for (auto& m : miejsca) {
        if (!m.zajete) return m.id;
    }
    return -1;
}

void SystemParkingowy::oznaczMiejsce(int id, bool zajete) {
    for (auto& m : miejsca) {
        if (m.id == id) {
            m.zajete = zajete;
            break;
        }
    }
}

void SystemParkingowy::dodajBilet(const Bilet& b) {
    bilety.push_back(b);
}

Bilet* SystemParkingowy::znajdzBilet(int id) {
    for (auto& b : bilety) {
        if (b.id == id) return &b;
    }
    return nullptr;
}

Bilet* SystemParkingowy::znajdzBiletPoNumerze(const std::string& nrRej) {
    for (auto& b : bilety) {
        if (b.nrRej == nrRej && !b.wyjechal) return &b;
    }
    return nullptr;
}

int SystemParkingowy::wolneMiejsca() {
    int count = 0;
    for (auto& m : miejsca)
        if (!m.zajete) count++;
    return count;
}

void SystemParkingowy::wyswietlStan() {
    std::cout << "Stan parkingu:\n";
    std::cout << "Miejsca (id: zajete):\n";
    for (auto& m : miejsca) {
        std::cout << " " << m.id << ": " << (m.zajete ? "X" : "O") << "\n";
    }
    std::cout << "Aktualna stawka za godzine: " << stawkaZaGodzine << " zl\n";
}

void SystemParkingowy::zapiszHistorieBiletowDoCSV(const std::string& nazwaPliku) {
    bool plikPusty = false;
    std::ifstream testPlik(nazwaPliku, std::ios::binary);
    plikPusty = !testPlik.good() || testPlik.peek() == std::ifstream::traits_type::eof();
    testPlik.close();

    std::ofstream plik(nazwaPliku, std::ios::app);
    if (!plik.is_open()) {
        std::cout << "Nie mozna otworzyc pliku do zapisu historii biletow.\n";
        return;
    }

    if (plikPusty) {
        plik << "ID Biletu;Nr rejestracyjny;ID miejsca;Data i godzina wjazdu;Data i godzina wyjazdu;Oplacony;Wyjechal\n";
    }

    for (const auto& b : bilety) {
        time_t czasWjazduT = std::chrono::system_clock::to_time_t(b.czasWjazdu);
        tm localWjazd{};
        localtime_s(&localWjazd, &czasWjazduT);
        char bufWjazd[20];
        strftime(bufWjazd, sizeof(bufWjazd), "%Y-%m-%d %H:%M", &localWjazd);

        char bufWyjazd[20] = "-";
        if (b.symulowanaGodzinaWyjazdu != "-") {
            time_t czasWyjazduT = std::chrono::system_clock::to_time_t(b.czasWyjazduSymulowany);
            tm localWyjazd{};
            localtime_s(&localWyjazd, &czasWyjazduT);
            strftime(bufWyjazd, sizeof(bufWyjazd), "%Y-%m-%d %H:%M", &localWyjazd);
        }

        plik << b.id << ";"
            << b.nrRej << ";"
            << b.miejsceId << ";"
            << bufWjazd << ";"
            << bufWyjazd << ";"
            << (b.oplacony ? "tak" : "nie") << ";"
            << (b.wyjechal ? "tak" : "nie") << "\n";
    }

    plik.close();
}
