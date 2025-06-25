#pragma once
#include <string>

class Kierowca {
public:
    int wjazdPojazdu(const std::string& nrRej);
    void wyjazdPojazdu(int idBiletu);
    void dokonajPlatnosci(int idBiletu);
};
