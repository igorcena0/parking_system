#include "AdministratorParkingu.h"
#include "SystemParkingowy.h"
#include "utils.h"
#include <iostream>
#include <string>

extern SystemParkingowy* globalnyParking;

bool AdministratorParkingu::zaloguj() {
    std::string haslo;
    std::cout << "Podaj haslo admina: ";
    std::cin >> haslo;
    return haslo == "admin";
}

void AdministratorParkingu::panelAdmina() {
    if (!zaloguj()) {
        std::cout << "Nieudane logowanie.\n";
        return;
    }
    int wybor = 0;
    do {
        std::cout << "\nPanel Administratora:\n1. Pokaz zajete miejsca\n2. Pokaz wolne miejsca\n"
            << "3. Pokaz historie biletow\n4. Zmien stawke\n5. Dodaj abonenta\n6. Usun abonenta\n7. Wyjdz\nWybor: ";
        if (!bezpiecznyInput(wybor)) {
            std::cout << "Niepoprawny wybor.\n";
            continue;
        }
        switch (wybor) {
        case 1: pokazZajeteMiejsca(); break;
        case 2: pokazWolneMiejsca(); break;
        case 3: pokazHistorieBiletow(); break;
        case 4: zmienStawke(); break;
        case 5: dodajAbonenta(); break;
        case 6: usunAbonenta(); break;
        case 7: std::cout << "Wylogowano.\n"; break;
        default: std::cout << "Nieznana opcja.\n";
        }
    } while (wybor != 7);
}

void AdministratorParkingu::pokazZajeteMiejsca() {
    std::cout << "[Admin] Zajete miejsca:\n";
    for (const auto& m : globalnyParking->miejsca) {
        if (m.zajete) std::cout << " - Miejsce: " << m.id << "\n";
    }
}

void AdministratorParkingu::pokazWolneMiejsca() {
    std::cout << "[Admin] Wolne miejsca: " << globalnyParking->wolneMiejsca() << "\n";
}

void AdministratorParkingu::pokazHistorieBiletow() {
    std::cout << "[Admin] Historia biletow:\n";
    for (const auto& b : globalnyParking->bilety) {
        time_t czasWjazduT = std::chrono::system_clock::to_time_t(b.czasWjazdu);
        tm localWjazd{};
        localtime_s(&localWjazd, &czasWjazduT);
        char bufWjazd[20];
        strftime(bufWjazd, sizeof(bufWjazd), "%H:%M", &localWjazd);

        std::cout << "Bilet ID: " << b.id
            << ", Nr rej: " << b.nrRej
            << ", Miejsce: " << b.miejsceId
            << ", Wjazd: " << bufWjazd
            << ", Wyjazd: " << b.symulowanaGodzinaWyjazdu
            << ", Oplacony: " << (b.oplacony ? "tak" : "nie")
            << ", Wyjechal: " << (b.wyjechal ? "tak" : "nie") << "\n";
    }
}

void AdministratorParkingu::zmienStawke() {
    std::cout << "Podaj nowa stawke za godzine (obecna: " << globalnyParking->stawkaZaGodzine << "): ";
    double nowaStawka;
    if (bezpiecznyInput(nowaStawka) && nowaStawka > 0) {
        globalnyParking->stawkaZaGodzine = nowaStawka;
        std::cout << "Stawka zmieniona na " << nowaStawka << "\n";
    }
    else std::cout << "Niepoprawna wartosc.\n";
}

void AdministratorParkingu::dodajAbonenta() {
    std::cout << "Podaj numer rejestracyjny abonenta: ";
    std::string nr;
    std::cin >> nr;

    if (globalnyParking->abonenci.find(nr) != globalnyParking->abonenci.end()) {
        std::cout << "Ten numer jest juz abonentem.\n";
        return;
    }

    globalnyParking->abonenci.insert(nr);
    std::cout << "Dodano abonenta.\n";

    for (auto& b : globalnyParking->bilety) {
        if (b.nrRej == nr && !b.wyjechal) {
            b.abonament = true;
            b.oplacony = true;
            b.moznaWyjechac = true;
            std::cout << "Zaktualizowano bilet ID " << b.id << " jako abonamentowy (oplacony).\n";
        }
    }
}

void AdministratorParkingu::usunAbonenta() {
    std::cout << "Podaj numer rejestracyjny abonenta do usuniecia: ";
    std::string nr;
    std::cin >> nr;

    if (globalnyParking->abonenci.erase(nr)) {
        std::cout << "Usunieto abonenta.\n";

        for (auto& b : globalnyParking->bilety) {
            if (b.nrRej == nr && !b.wyjechal) {
                b.abonament = false;
                b.oplacony = false;
                b.moznaWyjechac = false;
                std::cout << "Bilet ID " << b.id << " wymaga teraz oplacenia.\n";
            }
        }
    }
    else {
        std::cout << "Nie znaleziono abonenta.\n";
    }
}
