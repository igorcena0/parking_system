#include "Kierowca.h"
#include "SystemParkingowy.h"
#include "utils.h"
#include <iostream>
#include <cmath>
#include <limits>

extern SystemParkingowy* globalnyParking;

int Kierowca::wjazdPojazdu(const std::string& nrRej) {
    if (!globalnyParking->walidujNrRej(nrRej)) {
        std::cout << "[Kierowca] Niepoprawny numer rejestracyjny (tylko duze litery i cyfry, max 10 znakow).\n";
        return -1;
    }

    int miejsce = globalnyParking->znajdzWolneMiejsce();
    if (miejsce == -1) {
        std::cout << "[Kierowca] Brak wolnych miejsc!\n";
        return -1;
    }

    if (globalnyParking->znajdzBiletPoNumerze(nrRej) != nullptr) {
        std::cout << "[Kierowca] Pojazd z takim numerem juz jest na parkingu.\n";
        return -1;
    }

    globalnyParking->oznaczMiejsce(miejsce, true);

    Bilet b(globalnyParking->licznikBiletow++, nrRej, miejsce, globalnyParking->stawkaZaGodzine);

    if (globalnyParking->abonenci.find(nrRej) != globalnyParking->abonenci.end()) {
        b.abonament = true;
        b.oplacony = true;
        b.moznaWyjechac = true;
    }

    globalnyParking->dodajBilet(b);
    std::cout << "[Kierowca] Wjazd zatwierdzony. Bilet ID: " << b.id << ", miejsce: " << miejsce << "\n";

    return b.id;
}

void Kierowca::wyjazdPojazdu(int idBiletu) {
    Bilet* b = globalnyParking->znajdzBilet(idBiletu);
    if (!b) {
        std::cout << "[Kierowca] Bilet nie znaleziony.\n";
        return;
    }
    if (b->wyjechal) {
        std::cout << "[Kierowca] Ten pojazd juz wyjechal z parkingu.\n";
        return;
    }
    if (!b->oplacony && !b->abonament) {
        std::cout << "[Kierowca] Prosze oplacic bilet przed wyjazdem!\n";
        return;
    }
    if (!b->moznaWyjechac && !b->abonament) {
        std::cout << "[Kierowca] Minelo wiecej niz 10 minut od platnosci. Wyjazd niedozwolony.\n";
        return;
    }

    globalnyParking->oznaczMiejsce(b->miejsceId, false);
    b->wyjechal = true;
    std::cout << "[Kierowca] Wyjazd zatwierdzony. Dziekujemy!\n";
}

void Kierowca::dokonajPlatnosci(int idBiletu) {
    Bilet* b = globalnyParking->znajdzBilet(idBiletu);
    if (!b) {
        std::cout << "[Kierowca] Bilet nie znaleziony.\n";
        return;
    }
    if (b->oplacony) {
        std::cout << "[Kierowca] Bilet juz oplacony.\n";
        return;
    }
    if (b->abonament) {
        std::cout << "[Kierowca] Abonament - brak oplat.\n";
        b->oplacony = true;
        b->moznaWyjechac = true;
        return;
    }

    std::cout << "Podaj symulowana godzine wyjazdu (format HH:MM): ";
    std::string symulowanaGodzina;
    std::getline(std::cin, symulowanaGodzina);
    if (symulowanaGodzina.empty()) std::getline(std::cin, symulowanaGodzina);

    int h = 0, m = 0;
    size_t pos = symulowanaGodzina.find(':');
    if (pos == std::string::npos) {
        std::cout << "Niepoprawny format czasu.\n";
        return;
    }
    try {
        h = std::stoi(symulowanaGodzina.substr(0, pos));
        m = std::stoi(symulowanaGodzina.substr(pos + 1));
    }
    catch (...) {
        std::cout << "Niepoprawny format czasu.\n";
        return;
    }

    if (h < 0 || h >= 24 || m < 0 || m >= 60) {
        std::cout << "Niepoprawna godzina.\n";
        return;
    }
    b->symulowanaGodzinaWyjazdu = symulowanaGodzina;

    time_t czasWjazduT = std::chrono::system_clock::to_time_t(b->czasWjazdu);
    tm localWjazd{};
    localtime_s(&localWjazd, &czasWjazduT);

    localWjazd.tm_hour = h;
    localWjazd.tm_min = m;
    localWjazd.tm_sec = 0;
    std::chrono::system_clock::time_point czasWyjazduSymulowany = std::chrono::system_clock::from_time_t(mktime(&localWjazd));

    if (czasWyjazduSymulowany < b->czasWjazdu) {
        localWjazd.tm_mday += 1;
        czasWyjazduSymulowany = std::chrono::system_clock::from_time_t(mktime(&localWjazd));
    }
    b->czasWyjazduSymulowany = czasWyjazduSymulowany;

    std::chrono::duration<double> roznica = czasWyjazduSymulowany - b->czasWjazdu;
    double godziny = std::ceil(roznica.count() / 3600.0);
    if (godziny <= 0) godziny = 1;

    double kwota = godziny * b->stawkaZaGodzine;

    std::cout << "Wybierz metode platnosci:\n1. Gotowka\n2. Karta\nWybor: ";
    int metoda = 0;
    if (!bezpiecznyInput(metoda) || (metoda != 1 && metoda != 2)) {
        std::cout << "Niepoprawna metoda platnosci.\n";
        return;
    }

    if (metoda == 1) {
        std::cout << "Platnosc gotowka przyjeta.\n";
    }
    else {
        std::cout << "Platnosc karta przyjeta.\n";
    }

    b->oplacony = true;
    b->moznaWyjechac = true;

    std::cout << "[Kierowca] Bilet ID " << idBiletu << " zostal oplacony. Czas postoju: " << godziny
        << " godz., kwota: " << kwota << " zl.\n";
}
