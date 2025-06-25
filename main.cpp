#include "Bilet.h"
#include <iostream>

int main() {
    Bilet b(1, "ABC1234", 1, 5.0);
    std::cout << "Bilet ID: " << b.id << ", nr rej: " << b.nrRej << "\n";
    return 0;
}