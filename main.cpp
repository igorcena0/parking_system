#include "MiejsceParkingowe.h"
#include <iostream>

int main() {
    MiejsceParkingowe m(1);
    std::cout << "Miejsce ID: " << m.id << ", zajete: " << m.zajete << "\n";
    return 0;
}
