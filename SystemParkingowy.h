#pragma once
#include <vector>
#include <set>
#include <string>
#include "MiejsceParkingowe.h"
#include "Bilet.h"

class SystemParkingowy {
public:
    std::vector<MiejsceParkingowe> miejsca;
    std::vector<Bilet> bilety;
    std::set<std::string> abonenci;
    int licznikBiletow;
    double stawkaZaGodzine;

    SystemParkingowy(int iloscMiejsc, double stawka);

    bool walidujNrRej(const std::string& nr);
    int znajdzWolneMiejsce();
    void oznaczMiejsce(int id, bool zajete);
    void dodajBilet(const Bilet& b);
    Bilet* znajdzBilet(int id);
    Bilet* znajdzBiletPoNumerze(const std::string& nrRej);
    int wolneMiejsca();
    void wyswietlStan();
    void zapiszHistorieBiletowDoCSV(const std::string& nazwaPliku);
};

extern SystemParkingowy* globalnyParking;
