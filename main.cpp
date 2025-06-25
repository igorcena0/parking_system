#include "SystemParkingowy.h"
#include "Kierowca.h"
#include <iostream>

//SystemParkingowy* globalnyParking = nullptr;

int main() {
    SystemParkingowy system(5, 5.0); // 5 miejsc, 5 z³ za godzinê
    globalnyParking = &system;

    Kierowca kierowca;
    kierowca.wjazdPojazdu("ABC1234");

    system.wyswietlStan();

    return 0;
}
