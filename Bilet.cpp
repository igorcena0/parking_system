#include "Bilet.h"

Bilet::Bilet(int id, const std::string& nr, int miejsce, double stawka)
    : id(id), nrRej(nr), oplacony(false),
    moznaWyjechac(false), miejsceId(miejsce), abonament(false),
    wyjechal(false), stawkaZaGodzine(stawka), symulowanaGodzinaWyjazdu("-") {
    czasWjazdu = std::chrono::system_clock::now();
    czasWyjazduSymulowany = czasWjazdu;
}
