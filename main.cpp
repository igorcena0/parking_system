#include "SystemParkingowy.h"
#include "Kierowca.h"
#include "AdministratorParkingu.h"
#include <iostream>

//SystemParkingowy* globalnyParking = nullptr;

int main() {
    SystemParkingowy parking(5, 6.5); // 5 miejsc, 6.5 z³ za godzinê
    globalnyParking = &parking;

    Kierowca kierowca;
    AdministratorParkingu admin;

    kierowca.wjazdPojazdu("TEST123");
    kierowca.wjazdPojazdu("ADMIN1");

    std::cout << "\n>>> Przejscie do panelu administratora:\n";
    admin.panelAdmina();

    return 0;
}
