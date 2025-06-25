#include "menu.h"
#include "SystemParkingowy.h"
#include "Kierowca.h"
#include "AdministratorParkingu.h"
#include "utils.h"
#include <iostream>
#include <string>

extern SystemParkingowy* globalnyParking;

void menuInteraktywne(SystemParkingowy& system) {
    globalnyParking = &system;

    Kierowca kierowca;
    AdministratorParkingu admin;

    int wybor = 0;
    do {
        std::cout << "\n--- System Parkingowy ---\n"
            << "1. Wjazd pojazdu\n2. Wyjazd pojazdu\n3. Platnosc\n4. Panel administratora\n5. Stan parkingu\n6. Koniec\n"
            << "Wybor: ";
        if (!bezpiecznyInput(wybor)) {
            std::cout << "Niepoprawny wybor.\n";
            continue;
        }
        switch (wybor) {
        case 1: {
            std::cout << "Podaj numer rejestracyjny pojazdu: ";
            std::string nrRej;
            std::cin >> nrRej;
            kierowca.wjazdPojazdu(nrRej);
            break;
        }
        case 2: {
            std::cout << "Podaj ID biletu: ";
            int id;
            if (bezpiecznyInput(id))
                kierowca.wyjazdPojazdu(id);
            else
                std::cout << "Niepoprawne ID biletu.\n";
            break;
        }
        case 3: {
            std::cout << "Podaj ID biletu do platnosci: ";
            int id;
            if (bezpiecznyInput(id))
                kierowca.dokonajPlatnosci(id);
            else
                std::cout << "Niepoprawne ID biletu.\n";
            break;
        }
        case 4:
            admin.panelAdmina();
            break;
        case 5:
            system.wyswietlStan();
            break;
        case 6:
            std::cout << "Koniec programu.\n";
            break;
        default:
            std::cout << "Nieznana opcja.\n";
        }
    } while (wybor != 6);
}
