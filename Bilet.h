#pragma once
#include <string>
#include <chrono>

class Bilet {
public:
    int id;
    std::string nrRej;
    std::chrono::system_clock::time_point czasWjazdu;
    std::chrono::system_clock::time_point czasWyjazduSymulowany;
    std::string symulowanaGodzinaWyjazdu;
    bool oplacony;
    bool moznaWyjechac;
    int miejsceId;
    bool abonament;
    bool wyjechal;
    double stawkaZaGodzine;

    Bilet(int id, const std::string& nr, int miejsce, double stawka);
};
